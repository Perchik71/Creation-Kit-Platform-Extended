// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "valloc.h"
#include "vassert.h"
#include "vmmgeometry.h"
#include "vmapper.h"
#include "vio.h"

namespace voltek
{
	namespace memory_manager
	{
		// Equal-size blocks served from an index free list, then a bump pointer, so allocation is O(1); callers hold the pool lock.
		class page_t : public voltek::core::base
		{
		public:
			inline static constexpr uint32_t none = UINT32_MAX;

			page_t() = default;
			// Блоки берутся из слота карты, чтобы принадлежность проверялась по диапазону адресов.
			page_t(const class_geometry& geometry, voltek::core::mapper* mapper) :
				_stride(geometry.stride), _mapper(mapper)
			{
				const auto new_size = geometry.count;
				if (!new_size || new_size > none || !_mapper || !_stride || new_size > SIZE_MAX / _stride)
					return;
				_blocks = static_cast<char*>(_mapper->allocate(new_size * _stride));
				if (_blocks)
					_size = static_cast<uint32_t>(new_size);
			}
			virtual ~page_t()
			{
				if (_blocks)
					_mapper->release(_blocks);
			}
			page_t(const page_t&) = delete;
			page_t& operator=(const page_t&) = delete;

			inline uintptr_t get_user_data() const { return _user_data; }
			inline void set_user_data(uintptr_t user_data) { _user_data = user_data; }
			inline bool empty() const { return !_size; }
			inline bool is_all_blocks_free() const { return !_busy; }
			inline bool is_all_blocks_busy() const { return _free_head == none && _bump == _size; }
			inline size_t count() const { return _size; }
			inline size_t busy_count() const { return _busy; }
			inline size_t free_count() const { return _size - _busy; }
			inline block_base& at(size_t index) { return *reinterpret_cast<block_base*>(_blocks + index * _stride); }

			// Hands out one free block, preferring recently freed ones for locality.
			[[nodiscard]] bool get_first_free_block_index(size_t& index, uint32_t requested, uint8_t pool_id, uint8_t flags)
			{
				if (_free_head != none)
				{
					auto& block = at(_free_head);
					const auto next = *static_cast<const uint32_t*>(get_ptr_from_block_handle(&block));
					// A use-after-free write can corrupt the link; abandon the list rather than follow it.
					if ((next != none && next >= _bump) || !block_lifecycle::pop_under_lock(&block, requested, flags))
						_free_head = none;
					else
					{
						index = _free_head;
						_free_head = next;
						++_busy;
						return true;
					}
				}
				if (_bump == _size)
					return false;
				index = _bump++;
				auto& block = at(index);
				// Empty-page reset reuses headers; never rewrite identity fields read by concurrent frees.
				if (index == _initialized)
				{
					create_pool_block(&block, requested, static_cast<uint16_t>(_user_data),
						static_cast<uint32_t>(index), pool_id);
					++_initialized;
				}
				else
					block.size = requested;
				block_lifecycle::store(&block, flags);
				++_busy;
				return true;
			}

			// Returns false for a block that is already free, so a double free changes nothing.
			[[nodiscard]] bool set_block_free(size_t index, bool cached = false)
			{
				if (index >= _bump)
					return false;
				auto& block = at(index);
				if (!(cached ? block_lifecycle::free_cached_under_lock(&block) : block_lifecycle::try_free(&block)))
					return false;
				if (!--_busy)
				{
					// Empty again: forget the list and hand blocks out from the start.
					_free_head = none;
					_bump = 0;
					return true;
				}
				*static_cast<uint32_t*>(get_ptr_from_block_handle(&block)) = _free_head;
				_free_head = static_cast<uint32_t>(index);
				return true;
			}

			// Вывод дампа памяти страницы в файл.
			void dump(const char* filename) const
			{
#ifndef VMMDLL_EXPORTS
				voltek::core::_internal::memory_to_file(filename, (void*)_blocks,
					_size * _stride, _size >> 3);
#endif // !VMMDLL_EXPORTS
			}
		private:
			char* _blocks{ nullptr };
			size_t _stride{ 0 };
			uint32_t _size{ 0 };
			uint32_t _busy{ 0 };
			uint32_t _bump{ 0 };
			uint32_t _initialized{ 0 };
			uint32_t _free_head{ none };
			uintptr_t _user_data{ 0 };
			voltek::core::mapper* _mapper{ nullptr };
		public:
			// Empty and kept committed by its pool; the pool owns this flag.
			bool retained{ false };
		};
	}
}
