// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "vbits.h"
#include "vmmgeometry.h"
#include "vmmpage.h"
#include "vsimplelock.h"
#include <algorithm>
#include <atomic>
#include <utility>
#include <vector>

namespace voltek
{
	namespace memory_manager
	{
		// Cache hits update user counters without the pool lock.
		struct pool_counters
		{
			std::atomic<uint64_t> live_blocks{ 0 };
			std::atomic<uint64_t> requested_bytes{ 0 };
			// In-place realloc growth, changed without the lock; live requested bytes are the sum of both.
			std::atomic<int64_t> resized_bytes{ 0 };
			std::atomic<uint64_t> allocations{ 0 };
			std::atomic<uint64_t> allocated_bytes{ 0 };
			std::atomic<uint64_t> pages_created{ 0 };
			std::atomic<uint64_t> pages_released{ 0 };
			voltek::core::_internal::lock_counters lock{};

			static void add(std::atomic<uint64_t>& a_counter, uint64_t a_value) noexcept
			{
				a_counter.fetch_add(a_value, std::memory_order_relaxed);
			}
			static void bump(std::atomic<uint64_t>& a_counter) noexcept { add(a_counter, 1); }

			void allocated(size_t a_requested) noexcept
			{
				add(live_blocks, 1);
				add(allocations, 1);
				add(allocated_bytes, a_requested);
				add(requested_bytes, a_requested);
			}

			[[nodiscard]] uint64_t live_requested_bytes() const noexcept
			{
				return requested_bytes.load(std::memory_order_relaxed) +
					static_cast<uint64_t>(resized_bytes.load(std::memory_order_relaxed));
			}

			void released(size_t a_requested) noexcept
			{
				add(live_blocks, static_cast<uint64_t>(-1));
				add(requested_bytes, static_cast<uint64_t>(0) - a_requested);
			}
		};

		class pool_t : public voltek::core::base
		{
		public:
			// Тип страницы.
			using pageobj_t = page_t;
			// Тип указателя на страницу.
			using pageptr_t = pageobj_t*;
			// Конструктор.
			pool_t(size_t count, const class_geometry& geometry, voltek::core::mapper* mapper) noexcept :
				_mapper(mapper), _geometry(geometry)
			{
				set_size(count);
			}
			// Деструктор
			virtual ~pool_t() noexcept
			{
				map.clear();
				if (_pages)
				{
					voltek::core::_internal::aligned_free(_pages);
					_pages = nullptr;
					_count = 0;
				}
			}
			// Задаёт кол-во допустимых страниц для пула.
			// Одноразовая, не для расширения пула.
			void set_size(size_t count)
			{
				// Блокируем. Снятие блокировки будет заботить компилятор.
				voltek::core::_internal::simple_scope_lock scope_lock(lock);

				map.clear();
				// Only the bitmap needs padding; directories track the mapper's actual slot count.
				map.resize((std::max)(size_t{ 65536 }, (count + 255) & ~size_t{ 255 }));

				_pages = voltek::core::_internal::aligned_talloc<pageptr_t>(count, 0x10);
				if (!_pages)
				{
					map.clear();
					_vassert(!_pages);
				}
				else
				{
					_count = count;
					// A set bit marks an existing page with room; slots without a page stay clear.
					map.all_unset();
					_free_indices.reserve(count);
				}
			}
			// Возвращает допольнительную информацию, что привязана к пулу.
			[[nodiscard]] inline uintptr_t get_user_data() const noexcept { return _user_data; }
			// Устанавливает дополнительную информацию к пулу.
			inline void set_user_data(uintptr_t user_data) noexcept { _user_data = user_data; }
			// Возвращает истину, если пул не инициализирован.
			[[nodiscard]] inline bool empty() const noexcept { return !_count; }
			// Возвращает истину, если все страницы свободны.
			[[nodiscard]] inline bool is_all_pages_free() const noexcept { return map.is_all_sets(); }
			// Возвращает истину, если все страницы заняты.
			[[nodiscard]] inline bool is_all_pages_busy() const noexcept { return map.is_all_unsets(); }
			// Возвращает истину, если страница за указанным индексом - свободна.
			[[nodiscard]] inline bool is_page_free(size_t index) const noexcept { return map.is_set(index); }
			// Возвращает истину, если страница за указанным индексом - занята.
			[[nodiscard]] inline bool is_page_busy(size_t index) const noexcept { return map.is_unset(index); }
			// Помечает, что данная страница за указанным индексом - свободна.
			inline bool set_page_free(size_t index) noexcept { return map.set(index); }
			// Помечает, что данная страница за указанным индексом - занята.
			inline bool set_page_busy(size_t index) noexcept { return map.unset(index); }
			// Возвращает истину, в случае, нахождения первого попавшейся свободной страницы.
			// Его индекс будет передан в "index".
			[[nodiscard]] inline bool get_first_free_page_index(size_t& index) const noexcept { return map.find_first_set_bit(index); }
			// Возвращает кол-во допустимых страниц.
			[[nodiscard]] inline size_t count() const noexcept { return _count; }
			// Возвращает кол-во свободных страниц.
			[[nodiscard]] inline size_t free_count() const noexcept { return map.get_sets_count(); }
			// Existing pages with no room left.
			[[nodiscard]] inline size_t full_count() const noexcept { return _page_count - map.get_sets_count(); }
			// Возвращает страницу за указанным индексом. Константа.
			[[nodiscard]] inline pageptr_t c_at(size_t index) const noexcept { return _pages[index]; }
			// Возвращает страницу за указанным индексом.
			[[nodiscard]] inline pageptr_t& at(size_t index) noexcept { return _pages[index]; }
			// Оператор обращения к объекту класса в качестве массива.
			// Возвращает страницу за указанным индексом. Константа.
			[[nodiscard]] inline pageptr_t operator[](size_t index) const noexcept { return c_at(index); }
			// Оператор обращения к объекту класса в качестве массива.
			// Возвращает страницу за указанным индексом.
			[[nodiscard]] inline pageptr_t& operator[](size_t index) noexcept { return at(index); }
			// Вывод дампа битовой карты страницы в файл.
			inline void dump_map(const char* filename) const noexcept { map.dump(filename); }
			// Вывод дампа памяти массива страниц в файл.
			void dump(const char* filename) const noexcept
			{
				// Блокируем. Снятие блокировки будет заботить компилятор.
				voltek::core::_internal::simple_scope_lock scope_lock(lock);

#ifndef VMMDLL_EXPORTS
				voltek::core::_internal::memory_to_file(filename, (void*)_pages,
					voltek::core::_internal::aligned_msize(_pages), _geometry.count >> 3);
#endif // !VMMDLL_EXPORTS
			}
			// Возвращает истину в случаи нахождения свободного блока.
			// Передаёт сам блок, страницу где был найден блок и его индекс.
			// Эти данные понадобиться для освобождения блока у пула.
			// Блок указывается как занятый в последствии.
			// Counts the block only when asked, so disabled statistics cost one branch.
			[[nodiscard]] block_base* allocate(size_t requested, uint8_t pool_id, bool counted) noexcept
			{
				voltek::core::_internal::measured_scope_lock scope_lock(lock, counted ? &_counters.lock : nullptr);
				auto* block = prepare_block_locked(requested, pool_id,
					flag_block_pool_used | (counted ? flag_block_counted : 0));
				if (!block)
					return nullptr;
				++_held_blocks;
				if (counted)
					_counters.allocated(requested);
				return block;
			}
			size_t refill(block_base** blocks, size_t count, uint8_t pool_id, bool measured) noexcept
			{
				voltek::core::_internal::measured_scope_lock scope_lock(lock, measured ? &_counters.lock : nullptr);
				size_t filled = 0;
				while (filled < count)
				{
					auto* block = prepare_block_locked(0, pool_id, flag_block_pool_used | flag_block_cached);
					if (!block)
						break;
					blocks[filled++] = block;
				}
				_held_blocks += filled;
				return filled;
			}

			void flush(block_base* const* blocks, size_t count, bool measured) noexcept
			{
				voltek::core::_internal::measured_scope_lock scope_lock(lock, measured ? &_counters.lock : nullptr);
				size_t released_count = 0;
				for (size_t index = 0; index < count; ++index)
				{
					const auto* block = blocks[index];
					const bool released = release_block_locked(_pages[block->page_id], block->block_id, true);
					_vassert(released);
					released_count += released;
				}
				_held_blocks -= released_count;
			}

			void cache_allocated(size_t requested) noexcept { _counters.allocated(requested); }
			void cache_released(size_t requested) noexcept { _counters.released(requested); }
			// Освобождает блок. Возвращает истину, если всё успешно освободилось.
			bool release_block(size_t page_id, size_t index_block, size_t requested, bool counted) noexcept
			{
				voltek::core::_internal::measured_scope_lock scope_lock(lock, counted ? &_counters.lock : nullptr);
				if (!release_block_locked(_pages[page_id], index_block))
					return false;
				--_held_blocks;
				if (counted)
					_counters.released(requested);
				return true;
			}
			// In-place realloc changes a live block's size outside the pool lock.
			void resize_requested(size_t old_size, size_t new_size) noexcept
			{
				_counters.resized_bytes.fetch_add(static_cast<int64_t>(new_size) - static_cast<int64_t>(old_size), std::memory_order_relaxed);
			}
			[[nodiscard]] const pool_counters& counters() const noexcept { return _counters; }
			[[nodiscard]] uint64_t held_blocks() noexcept
			{
				voltek::core::_internal::simple_scope_lock scope_lock(lock);
				return _held_blocks;
			}
		private:
			block_base* prepare_block_locked(size_t requested, uint8_t pool_id, uint8_t flags) noexcept
			{
				if (!_current)
				{
				find_free_page_label:
					// Existing pages with room come first, so a pool reuses what it holds before growing.
					size_t index = 0;
					if (get_first_free_page_index(index))
						_current = _pages[index];
					else
					{
						if (!_free_indices.empty())
						{
							index = _free_indices.back();
							_free_indices.pop_back();
						}
						else if (_next_index < _count)
							index = _next_index++;
						else
						{
							_vassert_msg(true, "No free page");
							return nullptr;
						}
						_current = new pageobj_t(_geometry, _mapper);
						if (!_current || _current->empty())
						{
							delete _current;
							_current = nullptr;
							_free_indices.push_back(static_cast<uint32_t>(index));
							return nullptr;
						}
						// Привязываем индекс, как дополнитульную информацию.
						_current->set_user_data(static_cast<uintptr_t>(index));
						_pages[index] = _current;
						set_page_free(index);
						++_page_count;
						pool_counters::bump(_counters.pages_created);
					}
				}
				size_t index_block = 0;
				const bool retained = _current->retained;
				// Если страница закончилась, то надо искать новую.
				if (!_current->get_first_free_block_index(index_block, static_cast<uint32_t>(requested), pool_id, flags))
				{
					// Получаем индекс страницы и занимаем её.
					set_page_busy(static_cast<size_t>(_current->get_user_data()));
					_current = nullptr;

					goto find_free_page_label;
				}

				if (retained)
				{
					_current->retained = false;
					--_retained_pages;
				}

				return &_current->at(index_block);
			}

			[[nodiscard]] bool release_block_locked(pageptr_t page, size_t index_block, bool cached = false) noexcept
			{
				if (!page || (index_block >= page->count()))
					return false;

				// Попытка освободить в этой странице блок.
				if (!page->set_block_free(index_block, cached))
					return false;

				// Освободить страницу, так как один блок в ней свободен.
				auto index_page = static_cast<size_t>(page->get_user_data());
				set_page_free(index_page);

				if (!page->is_all_blocks_free())
					return true;
				// Keep the page object and free-list state to avoid recreation and lock churn at page boundaries.
				if (_retained_pages < _geometry.retained_pages)
				{
					page->retained = true;
					++_retained_pages;
				}
				else
				{
					if (_current == page)
						_current = nullptr;

					delete page;

					_pages[index_page] = nullptr;
					set_page_busy(index_page);
					--_page_count;
					_free_indices.push_back(static_cast<uint32_t>(index_page));
					pool_counters::bump(_counters.pages_released);
				}

				return true;
			}
			pool_t(const pool_t&) = delete;
			pool_t(pool_t&&) = delete;
			pool_t& operator=(pool_t&&) = delete;
			pool_t& operator=(const pool_t&) = delete;
		
			// Страницы, массив указателей, необязательно инициализированы.
			// Но сам массив должен.
			pageptr_t* _pages{ nullptr };
			// Текущая страница.
			pageptr_t _current{ nullptr };
			// Кол-во доступных страниц.
			size_t _count{ 0 };
			// Дополнительная информация.
			uintptr_t _user_data{ 0 };
			// Битовая карта.
			voltek::core::bits_regions map{};
			// Блокировщик для работы с множеством потоков.
			voltek::core::_internal::simple_lock lock{};
			// Карта памяти, из которой берутся страницы.
			voltek::core::mapper* _mapper{ nullptr };
			const class_geometry _geometry;
			pool_counters _counters{};
			uint64_t _held_blocks{ 0 };
			// Empty pages kept committed; bounded by the class's retention budget.
			size_t _retained_pages{ 0 };
			size_t _page_count{ 0 };
			// Page indices never used, then indices of released pages for reuse.
			size_t _next_index{ 0 };
			std::vector<uint32_t> _free_indices;
		};
	}
}