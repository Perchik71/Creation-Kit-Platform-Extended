// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "vsimplelock.h"
#include "vbits.h"

#include <atomic>
#include <array>
#include <stddef.h>
#include <stdint.h>
#include <vector>

namespace voltek
{
	namespace core
	{
		// One reservation holds every block the manager hands out, so ownership is a range check.
		class region
		{
		public:
			inline static constexpr size_t granularity = 64 * 1024;
			inline static constexpr size_t commit_granularity = 4 * 1024;
			// The engine over-reads block ends (previs, Havok cloth).
			inline static constexpr size_t readable_tail = 16;

			[[nodiscard]] static char* reserve(size_t size) noexcept;
			[[nodiscard]] static bool contains(const void* ptr) noexcept
			{
				return reinterpret_cast<uintptr_t>(ptr) - _base < _size;
			}
			[[nodiscard]] static size_t reserved() noexcept { return _size; }
			[[nodiscard]] static bool commit(void* ptr, size_t size) noexcept;
			static void decommit(void* ptr, size_t size) noexcept;
		private:
			inline static uintptr_t _base{ 0 };
			inline static size_t _size{ 0 };
		};

		class retention_budget
		{
		public:
			explicit retention_budget(size_t limit) noexcept : _limit(limit) {}
			[[nodiscard]] bool try_acquire(size_t bytes) noexcept
			{
				auto retained = _retained.load(std::memory_order_relaxed);
				while (bytes <= _limit - retained)
					if (_retained.compare_exchange_weak(retained, retained + bytes, std::memory_order_relaxed))
						return true;
				return false;
			}
			void release(size_t bytes) noexcept { _retained.fetch_sub(bytes, std::memory_order_relaxed); }
		private:
			const size_t _limit;
			std::atomic<size_t> _retained{ 0 };
		};

		// Equal-size slots over a slice of the region, optionally retaining committed extents for reuse.
		class mapper
		{
		public:
			mapper() = default;
			mapper(const mapper&) = delete;
			mapper& operator=(const mapper&) = delete;

			void assign(char* base, size_t slot_size, size_t slot_count, retention_budget* retention = nullptr);
			// Also commits readable_tail bytes past commit_size.
			[[nodiscard]] void* allocate(size_t commit_size) noexcept;
			void release(const void* slot) noexcept;
			[[nodiscard]] bool contains(const void* ptr) const noexcept
			{
				return reinterpret_cast<uintptr_t>(ptr) - reinterpret_cast<uintptr_t>(_base) < _slot_size * _slot_count;
			}
			[[nodiscard]] size_t slot_size() const noexcept { return _slot_size; }
			[[nodiscard]] size_t slot_count() const noexcept { return _slot_count; }
			[[nodiscard]] size_t used_count() const noexcept { return _used_count; }
			[[nodiscard]] uint64_t committed_bytes() const noexcept { return _committed.load(std::memory_order_relaxed); }
		private:
			char* _base{ nullptr };
			size_t _slot_size{ 0 };
			size_t _slot_count{ 0 };
			size_t _used_count{ 0 };
			bits _available;
			// Retained slots stay unavailable in the bitmap until popped, so scans cannot claim them.
			std::array<size_t, 8> _retained_indices{};
			size_t _retained_count{ 0 };
			retention_budget* _retention{ nullptr };
			// Committed extent per slot, including retained slots, in commit pages.
			std::vector<uint32_t> _committed_pages;
			std::atomic<uint64_t> _committed{ 0 };
			_internal::simple_lock _lock;
		};
	}
}
