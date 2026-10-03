// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include "vmapper.h"
#include "vassert.h"

#if (defined(_WIN32) || defined(_WIN64))
#	include <windows.h>
#endif

namespace voltek
{
	namespace core
	{
		char* region::reserve(size_t size) noexcept
		{
			if (_size)
				return nullptr;
			auto* base = static_cast<char*>(VirtualAlloc(nullptr, size, MEM_RESERVE, PAGE_READWRITE));
			if (!base)
				return nullptr;
			_base = reinterpret_cast<uintptr_t>(base);
			_size = size;
			return base;
		}

		bool region::commit(void* ptr, size_t size) noexcept
		{
			return VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE) != nullptr;
		}

		void region::decommit(void* ptr, size_t size) noexcept
		{
			VirtualFree(ptr, size, MEM_DECOMMIT);
		}

		void mapper::assign(char* base, size_t slot_size, size_t slot_count, retention_budget* retention)
		{
			_internal::simple_scope_lock scope_lock(_lock);
			_base = base;
			_slot_size = slot_size;
			_slot_count = slot_count;
			_retention = retention;
			_available.resize((slot_count + 2047) & ~size_t{ 2047 });
			_available.all_unset();
			// Padding stays unavailable so SIMD scans cannot hand out a nonexistent slot.
			for (size_t index = 0; index < slot_count; ++index)
				_available.set(index);
			_committed_pages.assign(slot_count, 0);
		}

		void* mapper::allocate(size_t commit_size) noexcept
		{
			if (!commit_size || commit_size > _slot_size || _slot_size - commit_size < region::readable_tail)
				return nullptr;

			size_t index = 0;
			_internal::simple_scope_lock scope_lock(_lock);
			const bool retained = _retained_count != 0;
			if (retained)
				index = _retained_indices[_retained_count - 1];
			else if (!_available.find_first_set_bit(index))
				return nullptr;

			auto* slot = _base + index * _slot_size;
			const auto pages = (commit_size + region::readable_tail + region::commit_granularity - 1) / region::commit_granularity;
			const auto previous_pages = _committed_pages[index];
			if (pages > previous_pages)
			{
				const auto extra = (pages - previous_pages) * region::commit_granularity;
				if (!region::commit(slot + previous_pages * region::commit_granularity, extra))
					return nullptr;
				_committed_pages[index] = static_cast<uint32_t>(pages);
				_committed.fetch_add(extra, std::memory_order_relaxed);
			}
			else if (pages < previous_pages)
			{
				const auto excess = (previous_pages - pages) * region::commit_granularity;
				region::decommit(slot + pages * region::commit_granularity, excess);
				_committed_pages[index] = static_cast<uint32_t>(pages);
				_committed.fetch_sub(excess, std::memory_order_relaxed);
			}
			if (retained)
			{
				--_retained_count;
				_retention->release(previous_pages * region::commit_granularity);
			}
			else
				_available.unset(index);
			++_used_count;
			return slot;
		}

		void mapper::release(const void* slot) noexcept
		{
			_internal::simple_scope_lock scope_lock(_lock);
			const auto index = static_cast<size_t>(static_cast<const char*>(slot) - _base) / _slot_size;
			const auto bytes = static_cast<size_t>(_committed_pages[index]) * region::commit_granularity;
			if (_retention && _retained_count < _retained_indices.size() && _retention->try_acquire(bytes))
			{
				_retained_indices[_retained_count++] = index;
				--_used_count;
				return;
			}
			// Decommit before the slot becomes reusable, or a new owner's commit could be undone.
			region::decommit(_base + index * _slot_size, _slot_size);
			_committed.fetch_sub(bytes, std::memory_order_relaxed);
			_committed_pages[index] = 0;

			_available.set(index);
			--_used_count;
		}
	}
}