// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "vmmgeometry.h"
#include "vmmpage.h"
#include "vsimplelock.h"
#include <algorithm>
#include <utility>
#include <vector>

#define USE_MULTITHREADS 0

#define __VMM_POOL_CONFIG_CACHE_SIZE 8ull * 1024

namespace voltek
{
	namespace memory_manager
	{
		// Шаблонный класс пула страниц памяти.
		template<typename _type, typename _page, size_t _blocks_in_page = blocks_per_page<_type>>
		class pool_t : public voltek::core::base
		{
		public:
			// Битовые карты не имеют хвостового прохода — кратность 256 обязана быть точной.
			static_assert((_blocks_in_page & 255) == 0,
				"_blocks_in_page must be a multiple of 256");
			// A partial 2048-bit chunk lets the AVX2 scan read past the bitmap and report an index outside the page.
			static_assert(is_valid_block_count(_blocks_in_page),
				"_blocks_in_page must be under 2048 or a multiple of 2048");
			// Тип страницы.
			using pageobj_t = _page;
			// Тип указателя на страницу.
			using pageptr_t = pageobj_t*;
			// Конструктор по умолчанию.
			pool_t() noexcept
			{
#if USE_MULTITHREADS
				free_stack_blocks.reserve(__VMM_POOL_CONFIG_CACHE_SIZE);
#endif
			}
			// Конструктор.
			// Внимание кол-во допустимых страниц будет округлено до кратности 256.
			pool_t(size_t count) noexcept
			{
#if USE_MULTITHREADS
				free_stack_blocks.reserve(__VMM_POOL_CONFIG_CACHE_SIZE);
#endif
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

#ifdef MAPPER_USE
					delete _mapper;
#endif
				}
			}
			// Задаёт кол-во допустимых страниц для пула.
			// Одноразовая, не для расширения пула.
			// Внимание кол-во допустимых страниц будет округлено до кратности 256.
			void set_size(size_t count)
			{
				// Блокируем. Снятие блокировки будет заботить компилятор.
				voltek::core::_internal::simple_scope_lock scope_lock(lock);

				// Число должно быть кратное 256, округляем в меньшую сторону.
				// Это необходимо, учитывая, что bits_regions размер минимум от 65536.
				// Использование SIMD инструкций является приоритетом, а "хвоста" должно 
				// быть немного, а лучше небыло вовсе.
				count = (count >> 8) << 8;

				map.clear();
				map.resize(count);

				_pages = voltek::core::_internal::aligned_talloc<pageptr_t>(count, 0x10);
				if (!_pages)
				{
					map.clear();
					_vassert(!_pages);
				}
				else
				{
					_count = count;
					// Размещаем везде единицы, 1 - свободная страница, 0 - занят.
					map.all_set();

#ifdef MAPPER_USE
					size_t blocksize = sizeof(_type) * _blocks_in_page;
					_mapper = new voltek::core::mapper(blocksize << 4, blocksize);
					_vassert(!_mapper);
#endif
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
			// Возвращает кол-во знятых страниц.
			[[nodiscard]] inline size_t busy_count() const noexcept { return map.get_unsets_count(); }
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
					voltek::core::_internal::aligned_msize(_pages), _blocks_in_page >> 3);
#endif // !VMMDLL_EXPORTS
			}
			// Возвращает истину в случаи нахождения свободного блока.
			// Передаёт сам блок, страницу где был найден блок и его индекс.
			// Эти данные понадобиться для освобождения блока у пула.
			// Блок указывается как занятый в последствии.
			[[nodiscard]] bool get_free_block(_type*& block, pageptr_t& page, size_t& index_block) noexcept
			{
				// Блокируем. Снятие блокировки будет заботить компилятор.
				voltek::core::_internal::simple_scope_lock scope_lock(lock);

#if USE_MULTITHREADS
				if (!free_stack_blocks.empty())
				{
					// Получить из стека
					auto& item = free_stack_blocks.back();
					// Передаём индекс блока
					index_block = item.second;
					// Передаём страницу
					page = item.first;
					// Получаем блок по текущему индексу
					block = &(page->at(index_block));
					// Удалить из стека
					free_stack_blocks.pop_back();

					return true;
				}
#endif

				if (!_current)
				{
				find_free_page_label:
					// Поиск свободной страницы

					size_t index = 0;
					if (!get_first_free_page_index(index))
					{
						_vassert_msg(true, "No free page");
						return false;
					}

					// Если страницы за таким индексом не существует, надо создать.
					if (!_pages[index])
					{
						// Объём одной страницы _blocks_in_page
#ifdef MAPPER_USE
						_current = new pageobj_t(_blocks_in_page, _mapper);
#else
						_current = new pageobj_t(_blocks_in_page);
#endif
						if (!_current)
						{
							_vassert_msg(true, "Failed new free page");
							return false;
						}
						if (_current->empty())
						{
							delete _current;
							_current = nullptr;
							return false;
						}
						// Привязываем индекс, как дополнитульную информацию.
						_current->set_user_data(static_cast<uintptr_t>(index));

						_pages[index] = _current;
					}
					else
						_current = _pages[index];
				}

				index_block = 0;
				// Если страница закончилась, то надо искать новую.
				if (!_current->get_first_free_block_index(index_block))
				{
					// Получаем индекс страницы и занимаем её.
					set_page_busy(static_cast<size_t>(_current->get_user_data()));
					_current = nullptr;

					goto find_free_page_label;
				}

				// Получаем блок по текущему индексу
				block = &(_current->at(index_block));
				// Передаём страницу
				page = _current;
				// Занять индекс блока, более он не доступен.
				_current->set_block_busy(index_block);

				return true;
			}
			// Освобождает блок. Возвращает истину, если всё успешно освободилось.
			bool release_block(pageptr_t page, size_t index_block) noexcept
			{
				// Блокируем. Снятие блокировки будет заботить компилятор.
				voltek::core::_internal::simple_scope_lock scope_lock(lock);

				// Фактический размер страницы, а не константа шаблона: он округляется вниз.
				if (!page || (index_block >= page->count()))
					return false;

				// Попытка освободить в этой странице блок.
				if (page->set_block_free(index_block))
				{
					// Освободить страницу, так как один блок в ней свободен.
					auto index_page = static_cast<size_t>(page->get_user_data());
					set_page_free(index_page);

					// Если страница пуста и она не первая, освободить память.
					if (page->is_all_blocks_free() && (index_page > 0))
					{
						if (_current == page)
							_current = nullptr;

#if USE_MULTITHREADS
						std::erase_if(free_stack_blocks, [page](const auto& e) { return e.first == page; });
#endif

						delete page;

						_pages[index_page] = nullptr;
					}
#if USE_MULTITHREADS
					// Only worth reserving blocks while a consumer pops them; otherwise the cache pins pages forever.
					else if (free_stack_blocks.size() < __VMM_POOL_CONFIG_CACHE_SIZE)
					{
						// Занять индекс блока, более он не доступен.
						page->set_block_busy(index_block);
						// добавить в стэк
						free_stack_blocks.push_back({ page, static_cast<uint32_t>(index_block) });
					}
#endif

					return true;
				}

				return false;
			}

#if USE_MULTITHREADS
			bool push_free_block_to_cache() noexcept
			{
				if (free_stack_blocks.size() < __VMM_POOL_CONFIG_CACHE_SIZE)
				{
					// Блокируем. Снятие блокировки будет заботить компилятор.
					voltek::core::_internal::simple_scope_lock scope_lock(lock);

					pageptr_t page = nullptr;
					_type* block = nullptr;
					size_t index_block = 0;

					if (get_free_block(block, page, index_block))
					{
						// Занять индекс блока, более он не доступен.
						page->set_block_busy(index_block);
						// добавить в стэк
						free_stack_blocks.push_back({ page, static_cast<uint32_t>(index_block) });
						return true;
					}
				}

				return false;
			}
#endif
		private:
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
			// Стек свободных блоков
			std::vector<std::pair<pageptr_t, uint32_t>> free_stack_blocks{};
			// Дополнительная информация.
			uintptr_t _user_data{ 0 };
			// Битовая карта.
			voltek::core::bits_regions map{};
			// Блокировщик для работы с множеством потоков.
			voltek::core::_internal::simple_lock lock{};
#ifdef MAPPER_USE
			// Карта памяти.
			voltek::core::mapper* _mapper;
#endif
		};
	}
}