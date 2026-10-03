// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma warning(disable : 6333)
#pragma warning(disable : 26819)
#pragma warning(disable : 28160)

#include "vmapper.h"
#include "vmmmain.h"
#include "vmmpool.h"

#include <atomic>
#include <type_traits>
#include <Windows.h>

#include <limits.h>
#include <string.h>


namespace voltek
{
	namespace core
	{
		// Инициализация систем.
		void initialize();
	}

	namespace memory_manager
	{
		memory_manager* global_memory_manager = nullptr;

		// Detailed class statistics cost a few nanoseconds per call, so they run only while telemetry reads them.
		std::atomic<bool> statistics_enabled{ false };

		constexpr static auto SIZE_LIMIT = std::numeric_limits<int32_t>::max();

		struct cache_bin
		{
			block_base* slots[64];
			uint8_t count;
		};

		struct thread_cache
		{
			cache_bin bins[cached_pool_count];
			uint8_t state;
		};

		static_assert(std::is_trivially_destructible_v<thread_cache>);
		static constinit thread_local thread_cache local_cache{};
		using shutdown_check = BOOLEAN (NTAPI*)();
		static shutdown_check dll_shutdown_in_progress = nullptr;

		struct cache_sentinel
		{
			~cache_sentinel()
			{
				if ((!dll_shutdown_in_progress || !dll_shutdown_in_progress()) && global_memory_manager)
					global_memory_manager->flush_thread_cache();
				local_cache.state = 2;
			}
		};

		[[nodiscard]] static thread_cache* current_cache() noexcept
		{
			if (local_cache.state == 2)
				return nullptr;
			if (!local_cache.state)
			{
				thread_local cache_sentinel sentinel;
				(void)sentinel;
				local_cache.state = 1;
			}
			return &local_cache;
		}

		[[nodiscard]] static pool_t* loaded_pool(pool_t* const* pools, size_t id) noexcept
		{
			return std::atomic_ref<pool_t*>(const_cast<pool_t*&>(pools[id])).load(std::memory_order_acquire);
		}

		static void flush_bin(cache_bin& bin, pool_t& pool, size_t count) noexcept
		{
			pool.flush(bin.slots, count, statistics_enabled.load(std::memory_order_relaxed));
			bin.count -= static_cast<uint8_t>(count);
			memmove(bin.slots, bin.slots + count, bin.count * sizeof(bin.slots[0]));
		}

		void memory_manager::flush_thread_cache() noexcept
		{
			for (size_t id = 0; id < cached_pool_count; ++id)
			{
				auto& bin = local_cache.bins[id];
				if (bin.count)
					flush_bin(bin, *loaded_pool(pools, id), bin.count);
			}
		}

		// Address space per pool class; a class that fills its slots falls back to large blocks.
		inline constexpr size_t pool_class_budget = 8ull * 1024 * 1024 * 1024;
		// Address space per large class; a full class spills into the next larger one.
		inline constexpr size_t large_class_budget = 16ull * 1024 * 1024 * 1024;

		[[nodiscard]] constexpr size_t round_up(size_t value, size_t alignment) noexcept
		{
			return (value + alignment - 1) & ~(alignment - 1);
		}

		[[nodiscard]] constexpr size_t pool_slot_size(const class_geometry& geometry) noexcept
		{
			return round_up(geometry.body_bytes + core::region::readable_tail, core::region::granularity);
		}

		static_assert([] {
			for (const auto& geometry : class_geometries)
				if (pool_class_budget / pool_slot_size(geometry) > UINT16_MAX)
					return false;
			return true;
		}(), "page counts must fit the block header");

		[[nodiscard]] constexpr size_t large_slot_size(size_t index) noexcept
		{
			return large_slot_minimum << index;
		}

		[[nodiscard]] constexpr size_t large_slot_count(size_t index) noexcept
		{
			return large_class_budget / large_slot_size(index) < 4 ? 4 : large_class_budget / large_slot_size(index);
		}

		// Two threads can create a lazy pool at once: publish exactly one, or a block gets released through the wrong pool.
		static pool_t* acquire_pool(pool_t** pools, size_t id, core::mapper* mapper) noexcept
		{
			std::atomic_ref<pool_t*> slot(pools[id]);
			if (auto* existing = slot.load(std::memory_order_acquire))
				return existing;

			auto* created = new pool_t(mapper->slot_count(), class_geometries[id], mapper);
			pool_t* expected = nullptr;
			if (slot.compare_exchange_strong(expected, created,
				std::memory_order_acq_rel, std::memory_order_acquire))
				return created;

			delete created;
			return expected;
		}

		memory_manager::memory_manager()
		{
			core::initialize();
			// Resolve before taking pool locks; thread-detach cleanup never enters the loader.
			static const auto shutdown = reinterpret_cast<shutdown_check>(
				GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "RtlDllShutdownInProgress"));
			dll_shutdown_in_progress = shutdown;

			size_t reserve = core::region::granularity + pool_count * pool_class_budget;
			for (size_t index = 0; index < large_class_count; ++index)
				reserve += large_slot_size(index) * large_slot_count(index);
			auto* cursor = core::region::reserve(reserve);
			if (!cursor || !core::region::commit(cursor, sizeof(block_base)))
			{
				_vassert(!cursor);
				return;
			}
			// The zero-size block sits in the region too, so every pointer handed out passes the range check.
			zero_size_request_block = create_default_block(reinterpret_cast<block_base*>(cursor), 0);
			cursor += core::region::granularity;
			for (size_t index = 0; index < page_sources.size(); ++index)
			{
				const auto slot = pool_slot_size(class_geometries[index]);
				page_sources[index].assign(cursor, slot, pool_class_budget / slot);
				cursor += pool_class_budget;
			}
			for (size_t index = 0; index < large_sources.size(); ++index)
			{
				large_sources[index].assign(cursor, large_slot_size(index), large_slot_count(index), &large_retention);
				cursor += large_slot_size(index) * large_slot_count(index);
			}

			pools = voltek::core::_internal::aligned_talloc<pool_t*>(pool_count, 0x10);
			if (pools)
			{
				for (size_t id = 0; id < pool_count && pool_limits[id] <= 64; ++id)
					(void)acquire_pool(pools, id, &page_sources[id]);
			}
		}

		// The manager lives for the whole process; its region and pools are never torn down.
		memory_manager::~memory_manager() = default;

		bool memory_manager::owns(const void* ptr) noexcept
		{
			const auto* header = get_block_handle_from_ptr(ptr);
			return core::region::contains(header) && is_valid_block(header);
		}

		block_base* memory_manager::large_alloc(size_t size) noexcept
		{
			for (size_t index = 0; index < large_sources.size(); ++index)
			{
				if (large_slot_size(index) - core::region::readable_tail < size)
					continue;
				if (auto* block = large_sources[index].allocate(size))
				{
					large_counters.live_blocks.fetch_add(1, std::memory_order_relaxed);
					large_counters.allocations.fetch_add(1, std::memory_order_relaxed);
					large_counters.allocated_bytes.fetch_add(size, std::memory_order_relaxed);
					large_counters.requested_bytes.fetch_add(size, std::memory_order_relaxed);
					return static_cast<block_base*>(block);
				}
			}
			return nullptr;
		}

		void memory_manager::large_free(block_base* block, size_t size) noexcept
		{
			for (auto& source : large_sources)
			{
				if (source.contains(block))
				{
					source.release(block);
					large_counters.live_blocks.fetch_sub(1, std::memory_order_relaxed);
					large_counters.requested_bytes.fetch_sub(size, std::memory_order_relaxed);
					return;
				}
			}
		}

		void* memory_manager::alloc(size_t size) noexcept
		{
			if (!size)
				return zero_size_request_block ? get_ptr_from_block_handle(zero_size_request_block) : nullptr;

			if (pools && size <= pool_limit_maximum)
			{
				const auto id = pool_class_of(size);
				auto* pool = acquire_pool(pools, id, &page_sources[id]);
				block_base* block = nullptr;
				const auto counted = statistics_enabled.load(std::memory_order_relaxed);
				auto* cache = id < cached_pool_count ? current_cache() : nullptr;
				if (pool && cache)
				{
					auto& bin = cache->bins[id];
					if (!bin.count)
						bin.count = static_cast<uint8_t>(pool->refill(bin.slots, class_geometries[id].cache_cap,
							static_cast<uint8_t>(id), counted));
					if (bin.count)
					{
						block = bin.slots[--bin.count];
						block->size = static_cast<uint32_t>(size);
						block_lifecycle::uncache(block, counted);
						if (counted)
							pool->cache_allocated(size);
					}
				}
				else if (pool)
					block = pool->allocate(size, static_cast<uint8_t>(id), counted);
				if (block)
				{
					return get_ptr_from_block_handle(block);
				}
				// A pool that cannot grow falls through to a large block.
			}

			if (size > (SIZE_LIMIT - sizeof(block_base)))
				return nullptr;
			if (auto* block = large_alloc(size + sizeof(block_base)))
			{
				create_default_block(block, size);
				return get_ptr_from_block_handle(block);
			}
			return nullptr;
		}

		void* memory_manager::aligned_alloc(size_t size, size_t alignment) noexcept
		{
			if (!alignment)
				alignment = 16;
			if ((alignment & (alignment - 1)) != 0)
				return nullptr;
			if (alignment <= 16)
				return alloc(size);

			if (alignment - 1 > SIZE_LIMIT - sizeof(aligned_block))
				return nullptr;
			const auto overhead = alignment - 1 + sizeof(aligned_block);
			if (size > SIZE_LIMIT - overhead)
				return nullptr;

			void* base = alloc(size + overhead);
			if (!base)
				return nullptr;

			const auto address = (reinterpret_cast<uintptr_t>(base) +
				sizeof(aligned_block) + alignment - 1) & ~(alignment - 1);
			auto* block = reinterpret_cast<aligned_block*>(address - sizeof(aligned_block));
			block->base = base;
			block->alignment = alignment;
			create_default_block(&block->header, size);
			block_lifecycle::store(&block->header, flag_block_default_used | flag_block_aligned);
			return reinterpret_cast<void*>(address);
		}

		void* memory_manager::aligned_realloc(const void* ptr, size_t size, size_t alignment) noexcept
		{
			if (!alignment)
				alignment = 16;
			if ((alignment & (alignment - 1)) != 0)
				return nullptr;
			if (!ptr)
				return aligned_alloc(size, alignment);
			if (!owns(ptr) || !block_lifecycle::live(get_block_handle_from_ptr(ptr)))
				return nullptr;
			if (!size)
			{
				free(ptr);
				return nullptr;
			}

			auto* block = get_block_handle_from_ptr(ptr);
			if (!is_used_aligned_block(block) && alignment <= 16)
				return realloc(ptr, size);

			void* replacement = aligned_alloc(size, alignment);
			if (!replacement)
				return nullptr;
			const auto old_size = get_size_from_block(block);
			memcpy(replacement, ptr, old_size < size ? old_size : size);
			free(ptr);
			return replacement;
		}

		void* memory_manager::realloc(const void* ptr, size_t size) noexcept
		{
			if (!ptr || !owns(ptr) || !block_lifecycle::live(get_block_handle_from_ptr(ptr)))
				return nullptr;
			if (!size)
			{
				free(ptr);
				return nullptr;
			}

			auto* block = get_block_handle_from_ptr(ptr);
			if (is_used_aligned_block(block))
				return aligned_realloc(ptr, size, get_aligned_block(block)->alignment);

			// A pooled block keeps its slot while the new size still fits its class.
			if (is_used_pool_block(block) && block->pool_id < pool_count && size <= pool_limits[block->pool_id] && pools)
			{
				const size_t id = block->pool_id;
				if (block_lifecycle::load(block) & flag_block_counted)
				{
					if (auto* pool = loaded_pool(pools, id))
						pool->resize_requested(block->size, size);
				}
				block->size = static_cast<uint32_t>(size);
				return const_cast<void*>(ptr);
			}

			const auto old_size = get_size_from_block(block);
			void* replacement = alloc(size);
			if (!replacement)
				return nullptr;
			if (old_size)
				memcpy(replacement, ptr, old_size < size ? old_size : size);
			free(ptr);
			return replacement;
		}

		bool memory_manager::free(const void* ptr) noexcept
		{
			if (!ptr || !owns(ptr))
				return false;

			auto* block = get_block_handle_from_ptr(ptr);
			if (is_used_aligned_block(block))
				return free(get_aligned_block(block)->base);

			if (is_used_default_block(block))
			{
				// The zero-size block is never released.
				if (block == zero_size_request_block)
					return true;
				const auto size = get_size_from_block(block);
				if (!size || !block_lifecycle::try_free_default(block))
					return false;
				large_free(block, size + sizeof(block_base));
				return true;
			}

			if (!pools || block->pool_id >= pool_count)
				return false;
			const size_t id = block->pool_id;
			auto* pool = loaded_pool(pools, id);
			if (!pool)
				return false;
			if (id < cached_pool_count)
			{
				if (auto* cache = current_cache())
				{
					const auto flags = block_lifecycle::try_cache(block);
					if (!flags)
						return false;
					if (flags & flag_block_counted)
						pool->cache_released(block->size);
					auto& bin = cache->bins[id];
					const auto& geometry = class_geometries[id];
					// Flush before the push so the fixed stack never overflows.
					if (bin.count == geometry.cache_cap)
						flush_bin(bin, *pool, geometry.cache_batch);
					bin.slots[bin.count++] = block;
					return true;
				}
			}
			const auto flags = block_lifecycle::load(block);
			if (flags & (flag_block_free | flag_block_cached))
				return false;
			return pool->release_block(block->page_id, block->block_id, block->size,
				(flags & flag_block_counted) != 0);
		}

		size_t memory_manager::msize(const void* ptr) const noexcept
		{
			if (!ptr || !owns(ptr)) return 0;
			// Получение размера.
			return (size_t)get_size_from_ptr(ptr);
		}

		void memory_manager::dump_map(size_t pool_id, const char* filename) const noexcept
		{
#ifndef VMMDLL_EXPORTS
			if (pool_id >= pool_count || !filename || !pools)
				return;
			if (auto* pool = loaded_pool(pools, pool_id))
				pool->dump_map(filename);
#endif // !VMMDLL_EXPORTS
		}

		void memory_manager::dump(size_t pool_id, const char* filename) const noexcept
		{
#ifndef VMMDLL_EXPORTS
			if (pool_id >= pool_count || !filename || !pools)
				return;
			if (auto* pool = loaded_pool(pools, pool_id))
				pool->dump(filename);
#endif // !VMMDLL_EXPORTS
		}

		void memory_manager::pool_stats(scalable_pool_stats& out) const noexcept
		{
			out = {};
			if (!pools)
				return;
			for (size_t index = 0; index < pool_count; ++index)
			{
				auto* pool = loaded_pool(pools, index);
				if (!pool)
					continue;
				// Unlocked maintained counters may produce a marginally stale sample.
				out.pool_count += !pool->empty();
				out.page_capacity += pool->count();
				out.pages_busy += pool->full_count();
			}
		}

		size_t memory_manager::class_stats(scalable_class_stats* out, size_t capacity) const noexcept
		{
			const auto read = [](const std::atomic<uint64_t>& a_counter) { return a_counter.load(std::memory_order_relaxed); };
			size_t count = 0;
			for (size_t index = 0; index < pool_count && count < capacity; ++index, ++count)
			{
				auto& entry = out[count];
				entry = {};
				entry.request_limit = pool_limits[index];
				entry.committed_bytes = page_sources[index].committed_bytes();
				if (!pools)
					continue;
				auto* pool = loaded_pool(pools, index);
				if (!pool)
					continue;
				const auto& counters = pool->counters();
				entry.block_stride = class_geometries[index].stride;
				entry.live_blocks = read(counters.live_blocks);
				entry.held_blocks = pool->held_blocks();
				entry.requested_bytes = counters.live_requested_bytes();
				entry.allocations = read(counters.allocations);
				entry.allocated_bytes = read(counters.allocated_bytes);
				entry.pages_created = read(counters.pages_created);
				entry.pages_released = read(counters.pages_released);
				entry.lock_contended = read(counters.lock.contended);
				entry.lock_wait_ticks = read(counters.lock.wait_ticks);
			}
			if (count < capacity)
			{
				auto& entry = out[count++];
				entry = {};
				for (const auto& source : large_sources)
					entry.committed_bytes += source.committed_bytes();
				entry.live_blocks = read(large_counters.live_blocks);
				entry.held_blocks = entry.live_blocks;
				entry.requested_bytes = read(large_counters.requested_bytes);
				entry.allocations = read(large_counters.allocations);
				entry.allocated_bytes = read(large_counters.allocated_bytes);
			}
			return count;
		}
	}

	VOLTEK_MM_API void scalable_get_pool_stats(scalable_pool_stats* out)
	{
		if (!out)
			return;
		*out = {};
		if (auto* manager = memory_manager::global_memory_manager)
			manager->pool_stats(*out);
	}

	VOLTEK_MM_API void scalable_enable_statistics()
	{
		memory_manager::statistics_enabled.store(true, std::memory_order_relaxed);
	}

	VOLTEK_MM_API size_t scalable_get_class_stats(scalable_class_stats* out, size_t capacity)
	{
		auto* manager = memory_manager::global_memory_manager;
		return manager && out ? manager->class_stats(out, capacity) : 0;
	}

	VOLTEK_MM_API void scalable_get_memory_stats(scalable_memory_stats* out)
	{
		if (!out)
			return;
		*out = {};
		out->reserved_bytes = core::region::reserved();
		auto* manager = memory_manager::global_memory_manager;
		if (!manager)
			return;
		scalable_class_stats classes[memory_manager::pool_count + 1]{};
		const auto count = manager->class_stats(classes, std::size(classes));
		for (size_t index = 0; index < count; ++index)
		{
			out->committed_bytes += classes[index].committed_bytes;
			out->live_blocks += classes[index].live_blocks;
			out->requested_bytes += classes[index].requested_bytes;
		}
	}
}

#pragma warning(default : 26819)
