// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma warning(disable : 6333)
#pragma warning(disable : 26819)
#pragma warning(disable : 28160)

#include "vmapper.h"
#include "vmmmain.h"
#include "vmmpool.h"

#include <mutex>
#include <atomic>
#include <chrono>
#include <condition_variable>

#include <limits.h>
#include <string.h>

using namespace std::literals;

#define USE_MULTITHREADS 0

#if USE_MULTITHREADS
#	if (defined(_WIN32) || defined(_WIN64))
#		include <windows.h>
#	endif
#endif

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

		constexpr size_t MAX_BLOCK_SIZE = 1ull * 1024 * 1024 * 1024;

		typedef page_t<block8_t> page8_t;
		typedef page_t<block16_t> page16_t;
		typedef page_t<block32_t> page32_t;
		typedef page_t<block64_t> page64_t;
		typedef page_t<block128_t> page128_t;
		typedef page_t<block256_t> page256_t;
		typedef page_t<block512_t> page512_t;
		typedef page_t<block1024_t> page1024_t;
		typedef page_t<block4096_t> page4096_t;
		typedef page_t<block8192_t> page8192_t;
		typedef page_t<block16384_t> page16384_t;
		typedef page_t<block32768_t> page32768_t;
		typedef page_t<block65536_t> page65536_t;
		typedef page_t<block131072_t> page131072_t;
		typedef pool_t<block8_t, page8_t> pool8_t;
		typedef pool_t<block16_t, page16_t> pool16_t;
		typedef pool_t<block32_t, page32_t> pool32_t;
		typedef pool_t<block64_t, page64_t> pool64_t;
		typedef pool_t<block128_t, page128_t> pool128_t;
		typedef pool_t<block256_t, page256_t> pool256_t;
		typedef pool_t<block512_t, page512_t> pool512_t;
		typedef pool_t<block1024_t, page1024_t> pool1024_t;
		typedef pool_t<block4096_t, page4096_t> pool4096_t;
		typedef pool_t<block8192_t, page8192_t> pool8192_t;
		typedef pool_t<block16384_t, page16384_t> pool16384_t;
		typedef pool_t<block32768_t, page32768_t> pool32768_t;
		typedef pool_t<block65536_t, page65536_t> pool65536_t;
		typedef pool_t<block131072_t, page131072_t> pool131072_t;

		template<typename Pool>
		static void accumulate_pool_stats(void*& slot, scalable_pool_stats& out) noexcept
		{
			auto* pool = reinterpret_cast<Pool*>(
				std::atomic_ref<void*>(slot).load(std::memory_order_acquire));
			if (!pool)
				return;
			// Unlocked maintained counters may produce a marginally stale sample.
			out.pool_count += !pool->empty();
			out.page_capacity += pool->count();
			out.pages_busy += pool->busy_count();
		}

		template<typename... Pools> static void accumulate_pool_stats(
			void** pools, scalable_pool_stats& out) noexcept
		{
			size_t index = 0;
			(accumulate_pool_stats<Pools>(pools[index++], out), ...);
		}

		// Проверка на допустимость памяти
		// Только Windows: Если произошло исключение, то вернёт false, иначе true.
		static bool is_valid_pointer(const void* ptr)
		{
			if (!ptr) return false;

#if (defined(_WIN32) || defined(_WIN64))	
			__try
			{
				// Попытка что-то разыменовать и записать...
				if (*((char*)(ptr)) != 0)
					*(char*)(ptr) = *(char*)(ptr);

				return true;
			}
			__except (1)
			{
				return false;
			}
#else
			return true;
#endif
		}

#if USE_MULTITHREADS
		class std_event
		{
			std::mutex mtx;
			std::condition_variable cv;
			bool signaled = false;
		public:
			void set() noexcept
			{
				{
					std::lock_guard<std::mutex> lock(mtx);
					signaled = true;
				}

				// Wakes all waiting threads
				cv.notify_all();
			}

			void reset() noexcept
			{
				std::lock_guard<std::mutex> lock(mtx);
				signaled = false; 
			}

			void wait() noexcept
			{
				std::unique_lock<std::mutex> lock(mtx);
				return cv.wait(lock, [this] { return signaled; });
			}

			[[nodiscard]] bool wait(uint32_t timeout) noexcept 
			{
				std::unique_lock<std::mutex> lock(mtx);
				return cv.wait_for(lock, std::chrono::milliseconds(timeout), [this] { return signaled; });
			}
		};
#endif

		static size_t POOL_SIZE = 64 * 1024;

		// Two threads can create a lazy pool at once: publish exactly one, or a block gets released through the wrong pool.
		template<typename _type>
		static _type* acquire_pool(void** pools, pool_type id) noexcept
		{
			std::atomic_ref<void*> slot(pools[std::to_underlying(id)]);
			if (auto* existing = slot.load(std::memory_order_acquire))
				return reinterpret_cast<_type*>(existing);

			auto* created = new _type(POOL_SIZE);
			void* expected = nullptr;
			if (slot.compare_exchange_strong(expected, reinterpret_cast<void*>(created),
				std::memory_order_acq_rel, std::memory_order_acquire))
				return created;

			delete created;
			return reinterpret_cast<_type*>(expected);
		}

		memory_manager::memory_manager() : pools(nullptr), thread(nullptr)
		{
			core::initialize();
			create_default_block(&zero_size_request_block, 0);
			
#if USE_MULTITHREADS
			event_close = new std_event();
			event_close_w = new std_event();
			if (!event_close || !event_close_w)
				return;
			
			reinterpret_cast<std_event*>(event_close)->reset();
			reinterpret_cast<std_event*>(event_close_w)->reset();
#endif

			// Вся технология ускорения зависит от новых инструкций, если их нет, незачем
			// это создавать.
			pools = voltek::core::_internal::aligned_talloc<void*>(std::to_underlying(pool_type::MAX), 0x10);
			if (pools)
			{
				pools[std::to_underlying(pool_type::pool_8)] = reinterpret_cast<void*>(new pool8_t(POOL_SIZE));
				pools[std::to_underlying(pool_type::pool_16)] = reinterpret_cast<void*>(new pool16_t(POOL_SIZE));
				pools[std::to_underlying(pool_type::pool_32)] = reinterpret_cast<void*>(new pool32_t(POOL_SIZE));
				pools[std::to_underlying(pool_type::pool_64)] = reinterpret_cast<void*>(new pool64_t(POOL_SIZE));
			}

#if USE_MULTITHREADS
			thread = new std::thread([](std_event* ev_close, std_event* ev_close_w, void** pools) {
				while (true)
				{
					{
						if (pools[std::to_underlying(pool_type::pool_8)]) { reinterpret_cast<pool8_t*>((pool8_t*)pools[std::to_underlying(pool_type::pool_8)])->push_free_block_to_cache(); }
						if (pools[std::to_underlying(pool_type::pool_16)]) { reinterpret_cast<pool16_t*>(pools[std::to_underlying(pool_type::pool_16)])->push_free_block_to_cache(); }
						if (pools[std::to_underlying(pool_type::pool_32)]) { reinterpret_cast<pool32_t*>(pools[std::to_underlying(pool_type::pool_32)])->push_free_block_to_cache(); }
						if (pools[std::to_underlying(pool_type::pool_64)]) { reinterpret_cast<pool64_t*>(pools[std::to_underlying(pool_type::pool_64)])->push_free_block_to_cache(); }
						if (pools[std::to_underlying(pool_type::pool_128)]) { reinterpret_cast<pool128_t*>(pools[std::to_underlying(pool_type::pool_128)])->push_free_block_to_cache(); }
						if (pools[std::to_underlying(pool_type::pool_256)]) { reinterpret_cast<pool256_t*>(pools[std::to_underlying(pool_type::pool_256)])->push_free_block_to_cache(); }
						if (pools[std::to_underlying(pool_type::pool_512)]) { reinterpret_cast<pool512_t*>(pools[std::to_underlying(pool_type::pool_512)])->push_free_block_to_cache(); }
						if (pools[std::to_underlying(pool_type::pool_1024)]) { reinterpret_cast<pool1024_t*>(pools[std::to_underlying(pool_type::pool_1024)])->push_free_block_to_cache(); }
						if (pools[std::to_underlying(pool_type::pool_4096)]) { reinterpret_cast<pool4096_t*>(pools[std::to_underlying(pool_type::pool_4096)])->push_free_block_to_cache(); }
						if (pools[std::to_underlying(pool_type::pool_8192)]) { reinterpret_cast<pool8192_t*>(pools[std::to_underlying(pool_type::pool_8192)])->push_free_block_to_cache(); }
						if (pools[std::to_underlying(pool_type::pool_16384)]) { reinterpret_cast<pool16384_t*>(pools[std::to_underlying(pool_type::pool_16384)])->push_free_block_to_cache(); }
						if (pools[std::to_underlying(pool_type::pool_32768)]) { reinterpret_cast<pool32768_t*>(pools[std::to_underlying(pool_type::pool_32768)])->push_free_block_to_cache(); }
						if (pools[std::to_underlying(pool_type::pool_65536)]) { reinterpret_cast<pool65536_t*>(pools[std::to_underlying(pool_type::pool_65536)])->push_free_block_to_cache(); }
						if (pools[std::to_underlying(pool_type::pool_131072)]) { reinterpret_cast<pool131072_t*>(pools[std::to_underlying(pool_type::pool_131072)])->push_free_block_to_cache(); }
					}

					if (ev_close->wait(10))
					{
						ev_close_w->set();
						break;
					}

					std::this_thread::yield();
				}
			}, reinterpret_cast<std_event*>(event_close), reinterpret_cast<std_event*>(event_close_w), pools);
			_vassert(!thread);

#if (defined(_WIN32) || defined(_WIN64))
			SetThreadPriority(thread->native_handle(), THREAD_PRIORITY_BELOW_NORMAL);
			const auto cores = std::thread::hardware_concurrency();
			if (cores > 0 && cores <= 64)
				SetThreadAffinityMask(thread->native_handle(), 1ull << (cores - 1));
#endif

			thread->detach();
#endif
		}

		memory_manager::~memory_manager()
		{
#if USE_MULTITHREADS
			if (thread)
			{
				reinterpret_cast<std_event*>(event_close)->set();
				reinterpret_cast<std_event*>(event_close_w)->wait();

				if (pools)
				{
					if (pools[std::to_underlying(pool_type::pool_8)]) delete reinterpret_cast<pool8_t*>(pools[std::to_underlying(pool_type::pool_8)]);
					if (pools[std::to_underlying(pool_type::pool_16)]) delete reinterpret_cast<pool16_t*>(pools[std::to_underlying(pool_type::pool_16)]);
					if (pools[std::to_underlying(pool_type::pool_32)]) delete reinterpret_cast<pool32_t*>(pools[std::to_underlying(pool_type::pool_32)]);
					if (pools[std::to_underlying(pool_type::pool_64)]) delete reinterpret_cast<pool64_t*>(pools[std::to_underlying(pool_type::pool_64)]);
					if (pools[std::to_underlying(pool_type::pool_128)]) delete reinterpret_cast<pool128_t*>(pools[std::to_underlying(pool_type::pool_128)]);
					if (pools[std::to_underlying(pool_type::pool_256)]) delete reinterpret_cast<pool256_t*>(pools[std::to_underlying(pool_type::pool_256)]);
					if (pools[std::to_underlying(pool_type::pool_512)]) delete reinterpret_cast<pool512_t*>(pools[std::to_underlying(pool_type::pool_512)]);
					if (pools[std::to_underlying(pool_type::pool_1024)]) delete reinterpret_cast<pool1024_t*>(pools[std::to_underlying(pool_type::pool_1024)]);
					if (pools[std::to_underlying(pool_type::pool_4096)]) delete reinterpret_cast<pool4096_t*>(pools[std::to_underlying(pool_type::pool_4096)]);
					if (pools[std::to_underlying(pool_type::pool_8192)]) delete reinterpret_cast<pool8192_t*>(pools[std::to_underlying(pool_type::pool_8192)]);
					if (pools[std::to_underlying(pool_type::pool_16384)]) delete reinterpret_cast<pool16384_t*>(pools[std::to_underlying(pool_type::pool_16384)]);
					if (pools[std::to_underlying(pool_type::pool_32768)]) delete reinterpret_cast<pool32768_t*>(pools[std::to_underlying(pool_type::pool_32768)]);
					if (pools[std::to_underlying(pool_type::pool_65536)]) delete reinterpret_cast<pool65536_t*>(pools[std::to_underlying(pool_type::pool_65536)]);
					if (pools[std::to_underlying(pool_type::pool_131072)]) delete reinterpret_cast<pool131072_t*>(pools[std::to_underlying(pool_type::pool_131072)]);

					voltek::core::_internal::aligned_free(pools);
					pools = nullptr;
				}

				delete thread;
				thread = nullptr;
			}
#endif
		}

		void* memory_manager::alloc(size_t size) noexcept
		{
			//if (ULONG_MAX < size)
			//	return nullptr;

			if (!size)
				return get_ptr_from_block_handle(&zero_size_request_block);

			// Проблемы с пулами? или размер больше фиксируемых блоков?
			// Тогда выделим память простым способом.
			if (!pools || (size > 131072))
			{
			alloc_default_ptr_label:
				block_base* new_block;

				if (size > SIZE_MAX - sizeof(block_base))
					return nullptr;
				
				if (size >= MAX_BLOCK_SIZE)
					new_block = (block_base*)voltek::core::_internal::page_alloc(size + sizeof(block_base));
				else
					new_block = (block_base*)voltek::core::_internal::aligned_malloc(size + sizeof(block_base), 0x10);
	
				if (new_block)
				{
					create_default_block(new_block, size);
					return get_ptr_from_block_handle(new_block);
				}

				_vassert(!new_block);
				return nullptr;
			}

			void* new_ptr = nullptr;
	
			if (size > 65536)
			{
				auto pool = acquire_pool<pool131072_t>(pools, pool_type::pool_131072);
				page131072_t* page = nullptr;
				block131072_t* block = nullptr;
				size_t index_block = 0;

				if (pool && pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, static_cast<uint32_t>(size), static_cast<uint16_t>(page->get_user_data()),
						static_cast<uint32_t>(index_block), static_cast<uint16_t>(std::to_underlying(pool_type::pool_131072)));
					new_ptr = get_ptr_from_block_handle(block);
				}
			}
			else if (size > 32768)
			{
				auto pool = acquire_pool<pool65536_t>(pools, pool_type::pool_65536);
				page65536_t* page = nullptr;
				block65536_t* block = nullptr;
				size_t index_block = 0;

				if (pool && pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, static_cast<uint32_t>(size), static_cast<uint16_t>(page->get_user_data()),
						static_cast<uint32_t>(index_block), static_cast<uint16_t>(std::to_underlying(pool_type::pool_65536)));
					new_ptr = get_ptr_from_block_handle(block);
				}
			}
			else if (size > 16384)
			{
				auto pool = acquire_pool<pool32768_t>(pools, pool_type::pool_32768);
				page32768_t* page = nullptr;
				block32768_t* block = nullptr;
				size_t index_block = 0;

				if (pool && pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, static_cast<uint32_t>(size), static_cast<uint16_t>(page->get_user_data()),
						static_cast<uint32_t>(index_block), static_cast<uint16_t>(std::to_underlying(pool_type::pool_32768)));
					new_ptr = get_ptr_from_block_handle(block);
				}
			}
			else if (size > 8192)
			{
				auto pool = acquire_pool<pool16384_t>(pools, pool_type::pool_16384);
				page16384_t* page = nullptr;
				block16384_t* block = nullptr;
				size_t index_block = 0;

				if (pool && pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, static_cast<uint32_t>(size), static_cast<uint16_t>(page->get_user_data()),
						static_cast<uint32_t>(index_block), static_cast<uint16_t>(std::to_underlying(pool_type::pool_16384)));
					new_ptr = get_ptr_from_block_handle(block);
				}
			}
			else if (size > 4096)
			{
				auto pool = acquire_pool<pool8192_t>(pools, pool_type::pool_8192);
				page8192_t* page = nullptr;
				block8192_t* block = nullptr;
				size_t index_block = 0;

				if (pool && pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, static_cast<uint32_t>(size), static_cast<uint16_t>(page->get_user_data()),
						static_cast<uint32_t>(index_block), static_cast<uint16_t>(std::to_underlying(pool_type::pool_8192)));
					new_ptr = get_ptr_from_block_handle(block);
				}
			}
			else if (size > 1024)
			{
				auto pool = acquire_pool<pool4096_t>(pools, pool_type::pool_4096);
				page4096_t* page = nullptr;
				block4096_t* block = nullptr;
				size_t index_block = 0;

				if (pool && pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, static_cast<uint32_t>(size), static_cast<uint16_t>(page->get_user_data()),
						static_cast<uint32_t>(index_block), static_cast<uint16_t>(std::to_underlying(pool_type::pool_4096)));
					new_ptr = get_ptr_from_block_handle(block);
				}
			}
			else if (size > 512)
			{
				auto pool = acquire_pool<pool1024_t>(pools, pool_type::pool_1024);
				page1024_t* page = nullptr;
				block1024_t* block = nullptr;
				size_t index_block = 0;

				if (pool && pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, static_cast<uint32_t>(size), static_cast<uint16_t>(page->get_user_data()),
						static_cast<uint32_t>(index_block), static_cast<uint16_t>(std::to_underlying(pool_type::pool_1024)));
					new_ptr = get_ptr_from_block_handle(block);
				}
			}
			else if (size > 256)
			{
				auto pool = acquire_pool<pool512_t>(pools, pool_type::pool_512);
				page512_t* page = nullptr;
				block512_t* block = nullptr;
				size_t index_block = 0;

				if (pool && pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, static_cast<uint32_t>(size), static_cast<uint16_t>(page->get_user_data()),
						static_cast<uint32_t>(index_block), static_cast<uint16_t>(std::to_underlying(pool_type::pool_512)));
					new_ptr = get_ptr_from_block_handle(block);
				}
			}
			else if (size > 128)
			{
				auto pool = acquire_pool<pool256_t>(pools, pool_type::pool_256);
				page256_t* page = nullptr;
				block256_t* block = nullptr;
				size_t index_block = 0;

				if (pool && pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, static_cast<uint32_t>(size), static_cast<uint16_t>(page->get_user_data()),
						static_cast<uint32_t>(index_block), static_cast<uint16_t>(std::to_underlying(pool_type::pool_256)));
					new_ptr = get_ptr_from_block_handle(block);
				}
			}
			else if (size > 64)
			{
				auto pool = acquire_pool<pool128_t>(pools, pool_type::pool_128);
				page128_t* page = nullptr;
				block128_t* block = nullptr;
				size_t index_block = 0;

				if (pool && pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, static_cast<uint32_t>(size), static_cast<uint16_t>(page->get_user_data()),
						static_cast<uint32_t>(index_block), static_cast<uint16_t>(std::to_underlying(pool_type::pool_128)));
					new_ptr = get_ptr_from_block_handle(block);
				}
			}
			else if (size > 32)
			{
				auto pool = acquire_pool<pool64_t>(pools, pool_type::pool_64);
				page64_t* page = nullptr;
				block64_t* block = nullptr;
				size_t index_block = 0;

				if (pool && pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, static_cast<uint32_t>(size), static_cast<uint16_t>(page->get_user_data()),
						static_cast<uint32_t>(index_block), static_cast<uint16_t>(std::to_underlying(pool_type::pool_64)));
					new_ptr = get_ptr_from_block_handle(block);
				}
			}
			else if (size > 16)
			{
				auto pool = acquire_pool<pool32_t>(pools, pool_type::pool_32);
				page32_t* page = nullptr;
				block32_t* block = nullptr;
				size_t index_block = 0;

				if (pool && pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, static_cast<uint32_t>(size), static_cast<uint16_t>(page->get_user_data()),
						static_cast<uint32_t>(index_block), static_cast<uint16_t>(std::to_underlying(pool_type::pool_32)));
					new_ptr = get_ptr_from_block_handle(block);
				}
			}
			else if (size > 8)
			{
				auto pool = acquire_pool<pool16_t>(pools, pool_type::pool_16);
				page16_t* page = nullptr;
				block16_t* block = nullptr;
				size_t index_block = 0;

				if (pool && pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, static_cast<uint32_t>(size), static_cast<uint16_t>(page->get_user_data()),
						static_cast<uint32_t>(index_block), static_cast<uint16_t>(std::to_underlying(pool_type::pool_16)));
					new_ptr = get_ptr_from_block_handle(block);
				}
			}
			else
			{
				auto pool = acquire_pool<pool8_t>(pools, pool_type::pool_8);
				page8_t* page = nullptr;
				block8_t* block = nullptr;
				size_t index_block = 0;

				if (pool && pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, static_cast<uint32_t>(size), static_cast<uint16_t>(page->get_user_data()),
						static_cast<uint32_t>(index_block), static_cast<uint16_t>(std::to_underlying(pool_type::pool_8)));
					new_ptr = get_ptr_from_block_handle(block);
				}
			}

			// Если каким-то чудом память не выделена, то выделим память простым способом.
			if (!new_ptr)
				goto alloc_default_ptr_label;
			
			return new_ptr;
		}

		void* memory_manager::realloc(const void* ptr, size_t size) noexcept
		{
			if (!ptr || !is_valid_ptr(ptr) || !is_valid_pointer(ptr) /*|| (ULONG_MAX < size)*/)
				return nullptr;
			if (!size)
			{
				free(ptr);
				return nullptr;
			}

			void* new_ptr = nullptr;	

			// Если память выделена ранее как обычный, то тут выделение новой памяти неизбежно.
			if (is_used_default_ptr(ptr))
			{
			realloc_def_label:
				size_t old_size = msize(ptr);
				new_ptr = alloc(size);
				if (!new_ptr)
					return nullptr;
				if (old_size > 0) memcpy(new_ptr, ptr, old_size > size ? size : old_size);
				free(ptr);
			}
			else
			{
				new_ptr = const_cast<void*>(ptr);
				block_base* block = get_block_handle_from_ptr(new_ptr);
				uint8_t pool_id = block->pool_id;

				switch (static_cast<pool_type>(pool_id))
				{
				case pool_type::pool_131072:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 131072)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = static_cast<uint32_t>(size);
				}
				break;
				case pool_type::pool_65536:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 65536)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = static_cast<uint32_t>(size);
				}
				break;
				case pool_type::pool_32768:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 32768)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = static_cast<uint32_t>(size);
				}
				break;
				case pool_type::pool_16384:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 16384)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = static_cast<uint32_t>(size);
				}
				break;
				case pool_type::pool_8192:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 8192)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = static_cast<uint32_t>(size);
				}
				break;
				case pool_type::pool_4096:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 4096)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = static_cast<uint32_t>(size);
				}
				break;
				case pool_type::pool_1024:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 1024)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = static_cast<uint32_t>(size);
				}
				break;
				case pool_type::pool_512:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 512)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = static_cast<uint32_t>(size);
				}
				break;
				case pool_type::pool_256:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 256)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = static_cast<uint32_t>(size);
				}
				break;
				case pool_type::pool_128:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 128)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = static_cast<uint32_t>(size);
				}
				break;
				case pool_type::pool_64:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 64)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = static_cast<uint32_t>(size);
				}
				break;
				case pool_type::pool_32:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 32)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = static_cast<uint32_t>(size);
				}
				break;
				case pool_type::pool_16:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 16)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = static_cast<uint32_t>(size);
				}
				break;
				default:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 8)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = static_cast<uint32_t>(size);
				}
				break;
				}
			}

			return new_ptr;
		}

		bool memory_manager::free(const void* ptr) noexcept
		{
			if (!ptr || !is_valid_ptr(ptr) || !is_valid_pointer(ptr))
				return false;

			bool ret = true;

			if (is_used_default_ptr(ptr))
			{
				auto block = get_block_handle_from_ptr(ptr);
				size_t block_size = get_size_from_block(block);
				if (block_size > 0)
				{
					if (block_size >= MAX_BLOCK_SIZE)
						voltek::core::_internal::page_free(block);
					else 
						voltek::core::_internal::aligned_free(block);
				}
			}
			else
			{
				block_base* block = get_block_handle_from_ptr(ptr);
				uint8_t pool_id = block->pool_id;
				uint16_t page_id = block->page_id;
				uint32_t block_id = block->block_id;

				switch (static_cast<pool_type>(pool_id))
				{
				case pool_type::pool_131072:
				{
					auto pool = reinterpret_cast<pool131072_t*>(pools[pool_id]);
					ret = pool->release_block((*pool)[page_id], block_id);
				}
				break;
				case pool_type::pool_65536:
				{
					auto pool = reinterpret_cast<pool65536_t*>(pools[pool_id]);
					ret = pool->release_block((*pool)[page_id], block_id);
				}
				break;
				case pool_type::pool_32768:
				{
					auto pool = reinterpret_cast<pool32768_t*>(pools[pool_id]);
					ret = pool->release_block((*pool)[page_id], block_id);
				}
				break;
				case pool_type::pool_16384:
				{
					auto pool = reinterpret_cast<pool16384_t*>(pools[pool_id]);
					ret = pool->release_block((*pool)[page_id], block_id);
				}
				break;
				case pool_type::pool_8192:
				{
					auto pool = reinterpret_cast<pool8192_t*>(pools[pool_id]);
					ret = pool->release_block((*pool)[page_id], block_id);
				}
				break;
				case pool_type::pool_4096:
				{
					auto pool = reinterpret_cast<pool4096_t*>(pools[pool_id]);
					ret = pool->release_block((*pool)[page_id], block_id);
				}
				break;
				case pool_type::pool_1024:
				{
					auto pool = reinterpret_cast<pool1024_t*>(pools[pool_id]);
					ret = pool->release_block((*pool)[page_id], block_id);
				}
				break;
				case pool_type::pool_512:
				{
					auto pool = reinterpret_cast<pool512_t*>(pools[pool_id]);
					ret = pool->release_block((*pool)[page_id], block_id);
				}
				break;
				case pool_type::pool_256:
				{
					auto pool = reinterpret_cast<pool256_t*>(pools[pool_id]);
					ret = pool->release_block((*pool)[page_id], block_id);
				}
				break;
				case pool_type::pool_128:
				{
					auto pool = reinterpret_cast<pool128_t*>(pools[pool_id]);
					ret = pool->release_block((*pool)[page_id], block_id);
				}
				break;
				case pool_type::pool_64:
				{
					auto pool = reinterpret_cast<pool64_t*>(pools[pool_id]);
					ret = pool->release_block((*pool)[page_id], block_id);
				}
				break;
				case pool_type::pool_32:
				{
					auto pool = reinterpret_cast<pool32_t*>(pools[pool_id]);
					ret = pool->release_block((*pool)[page_id], block_id);
				}
				break;
				case pool_type::pool_16:
				{
					auto pool = reinterpret_cast<pool16_t*>(pools[pool_id]);
					ret = pool->release_block((*pool)[page_id], block_id);
				}
				break;
				default:
				{
					auto pool = reinterpret_cast<pool8_t*>(pools[pool_id]);
					ret = pool->release_block((*pool)[page_id], block_id);
				}
				break;
				}
			}

			return ret;
		}

		size_t memory_manager::msize(const void* ptr) const noexcept
		{
			if (!ptr || !is_valid_pointer(ptr)) return 0;
			// Получение размера.
			return (size_t)get_size_from_ptr(ptr);
		}

		void memory_manager::dump_map(size_t pool_id, const char* filename) const noexcept
		{
#ifndef VMMDLL_EXPORTS
			if ((std::to_underlying(pool_type::MAX) >= pool_id) || !filename)
				return;

			switch (static_cast<pool_type>(pool_id))
			{
			case pool_type::pool_131072:
			{
				auto pool = reinterpret_cast<pool131072_t*>(pools[pool_id]);
				pool->dump_map(filename);
			}
			break;
			case pool_type::pool_65536:
			{
				auto pool = reinterpret_cast<pool65536_t*>(pools[pool_id]);
				pool->dump_map(filename);
			}
			break;
			case pool_type::pool_32768:
			{
				auto pool = reinterpret_cast<pool32768_t*>(pools[pool_id]);
				pool->dump_map(filename);
			}
			break;
			case pool_type::pool_16384:
			{
				auto pool = reinterpret_cast<pool16384_t*>(pools[pool_id]);
				pool->dump_map(filename);
			}
			break;
			case pool_type::pool_8192:
			{
				auto pool = reinterpret_cast<pool8192_t*>(pools[pool_id]);
				pool->dump_map(filename);
			}
			break;
			case pool_type::pool_4096:
			{
				auto pool = reinterpret_cast<pool4096_t*>(pools[pool_id]);
				pool->dump_map(filename);
			}
			break;
			case pool_type::pool_1024:
			{
				auto pool = reinterpret_cast<pool1024_t*>(pools[pool_id]);
				pool->dump_map(filename);
			}
			break;
			case pool_type::pool_512:
			{
				auto pool = reinterpret_cast<pool512_t*>(pools[pool_id]);
				pool->dump_map(filename);
			}
			break;
			case pool_type::pool_256:
			{
				auto pool = reinterpret_cast<pool256_t*>(pools[pool_id]);
				pool->dump_map(filename);
			}
			break;
			case pool_type::pool_128:
			{
				auto pool = reinterpret_cast<pool128_t*>(pools[pool_id]);
				pool->dump_map(filename);
			}
			break;
			case pool_type::pool_64:
			{
				auto pool = reinterpret_cast<pool64_t*>(pools[pool_id]);
				pool->dump_map(filename);
			}
			break;
			case pool_type::pool_32:
			{
				auto pool = reinterpret_cast<pool32_t*>(pools[pool_id]);
				pool->dump_map(filename);
			}
			break;
			case pool_type::pool_16:
			{
				auto pool = reinterpret_cast<pool16_t*>(pools[pool_id]);
				pool->dump_map(filename);
			}
			break;
			default:
			{
				auto pool = reinterpret_cast<pool8_t*>(pools[pool_id]);
				pool->dump_map(filename);
			}
			break;
			}
#endif // !VMMDLL_EXPORTS
		}

		void memory_manager::dump(size_t pool_id, const char* filename) const noexcept
		{
#ifndef VMMDLL_EXPORTS
			if ((std::to_underlying(pool_type::MAX) >= pool_id) || !filename)
				return;

			switch (static_cast<pool_type>(pool_id))
			{
			case pool_type::pool_131072:
			{
				auto pool = reinterpret_cast<pool131072_t*>(pools[pool_id]);
				pool->dump(filename);
			}
			break;
			case pool_type::pool_65536:
			{
				auto pool = (pool65536_t*)reinterpret_cast<pool131072_t*>(pools[pool_id]);
				pool->dump(filename);
			}
			break;
			case pool_type::pool_32768:
			{
				auto pool = (pool32768_t*)reinterpret_cast<pool131072_t*>(pools[pool_id]);
				pool->dump(filename);
			}
			break;
			case pool_type::pool_16384:
			{
				auto pool = (pool16384_t*)reinterpret_cast<pool131072_t*>(pools[pool_id]);
				pool->dump(filename);
			}
			break;
			case pool_type::pool_8192:
			{
				auto pool = (pool8192_t*)reinterpret_cast<pool131072_t*>(pools[pool_id]);
				pool->dump(filename);
			}
			break;
			case pool_type::pool_4096:
			{
				auto pool = (pool4096_t*)reinterpret_cast<pool131072_t*>(pools[pool_id]);
				pool->dump(filename);
			}
			break;
			case pool_type::pool_1024:
			{
				auto pool = (pool1024_t*)reinterpret_cast<pool131072_t*>(pools[pool_id]);
				pool->dump(filename);
			}
			break;
			case pool_type::pool_512:
			{
				auto pool = (pool512_t*)reinterpret_cast<pool131072_t*>(pools[pool_id]);
				pool->dump(filename);
			}
			break;
			case pool_type::pool_256:
			{
				auto pool = (pool256_t*)reinterpret_cast<pool131072_t*>(pools[pool_id]);
				pool->dump(filename);
			}
			break;
			case pool_type::pool_128:
			{
				auto pool = (pool128_t*)reinterpret_cast<pool131072_t*>(pools[pool_id]);
				pool->dump(filename);
			}
			break;
			case pool_type::pool_64:
			{
				auto pool = (pool64_t*)reinterpret_cast<pool131072_t*>(pools[pool_id]);
				pool->dump(filename);
			}
			break;
			case pool_type::pool_32:
			{
				auto pool = (pool32_t*)reinterpret_cast<pool131072_t*>(pools[pool_id]);
				pool->dump(filename);
			}
			break;
			case pool_type::pool_16:
			{
				auto pool = (pool16_t*)reinterpret_cast<pool131072_t*>(pools[pool_id]);
				pool->dump(filename);
			}
			break;
			default:
			{
				auto pool = (pool8_t*)reinterpret_cast<pool131072_t*>(pools[pool_id]);
				pool->dump(filename);
			}
			break;
			}
#endif // !VMMDLL_EXPORTS
		}
	}

	VOLTEK_MM_API void scalable_get_pool_stats(scalable_pool_stats* out)
	{
		if (!out)
			return;
		*out = {};
		auto* manager = memory_manager::global_memory_manager;
		if (!manager || !manager->pools)
			return;
		memory_manager::accumulate_pool_stats<
			memory_manager::pool8_t, memory_manager::pool16_t, memory_manager::pool32_t,
			memory_manager::pool64_t, memory_manager::pool128_t, memory_manager::pool256_t,
			memory_manager::pool512_t, memory_manager::pool1024_t, memory_manager::pool4096_t,
			memory_manager::pool8192_t, memory_manager::pool16384_t, memory_manager::pool32768_t,
			memory_manager::pool65536_t, memory_manager::pool131072_t>(manager->pools, *out);
	}
}

#pragma warning(default : 26819)