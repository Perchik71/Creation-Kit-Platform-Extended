// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "valloc.h"
#include "vassert.h"
#include "vmapper.h"
#include "vio.h"
#include "vstack.h"

#define __VMM_PAGE_CONFIG_NORMAL_SIZE voltek::core::bits_regions
#define __VMM_PAGE_CONFIG_SMALL_SIZE voltek::core::bits
#define __VMM_PAGE_CONFIG_LOW_SIZE __VMM_PAGE_CONFIG_SMALL_SIZE

namespace voltek
{
	namespace memory_manager
	{
		// Шаблонный класс страницы памяти
		// Является хранилищем блоков, но при этом нет жёсткой зависимости от каких блоков.
		// Служит только в качестве помечания свободный или занятый блок, а также быстрого
		// нахождения свободного блока, при помощи битовой карты.
		template<typename _type, typename _map = __VMM_PAGE_CONFIG_NORMAL_SIZE>
		class page_t : public voltek::core::base
		{
		public:
			// Конструктор по умолчанию.
			page_t() : _blocks(nullptr), _size(0), _user_data(0)
			{}
			// Конструктор.
			// Внимание размер будет округлён до кратности 256.
#ifdef MAPPER_USE
			page_t(size_t new_size, voltek::core::mapper* mapper) : _blocks(nullptr), _size(0), _user_data(0), _mapper(mapper)
#else
			page_t(size_t new_size) : _blocks(nullptr), _size(0), _user_data(0)
#endif
			{
				set_size(new_size);
			}
			// Деструктор
			virtual ~page_t()
			{
				map.clear();
				if (_blocks)
				{
#ifdef MAPPER_USE
					if (!_mapper->block_free(_blocks))
						voltek::core::_internal::aligned_free(_blocks);
#else
					voltek::core::_internal::aligned_free(_blocks);
#endif

					_blocks = nullptr;
					_size = 0;
				}
			}
			// Задаёт размер страницы.
			// Одноразовая, не для расширения страницы.
			// Внимание размер будет округлён до кратности 256.
			void set_size(size_t new_size)
			{
				// Число должно быть кратное 256, округляем в меньшую сторону.
				// Это необходимо, учитывая, что bits_regions размер минимум от 65536.
				// Использование SIMD инструкций является приоритетом, а "хвоста" должно 
				// быть немного, а лучше небыло вовсе.
				new_size = (new_size << 8) >> 8;

				map.clear();
				map.resize(new_size);

#ifdef MAPPER_USE
				_blocks = (_type*)_mapper->block_alloc();
				if (!_blocks) _blocks = voltek::core::_internal::aligned_talloc<_type>(new_size, 0x10);
#else
				_blocks = voltek::core::_internal::aligned_talloc<_type>(new_size, 0x10);
#endif

				if (!_blocks)
				{
					map.clear();
					_vassert(!_blocks);
				}
				else
				{
					_size = new_size;
					// Размещаем везде единицы, 1 - свободных блок, 0 - занят.
					map.all_set();
				}
			}
			// Возвращает допольнительную информацию, что привязана к странице.
			inline uintptr_t get_user_data() const { return _user_data; }
			// Устанавливает дополнительную информацию к странице.
			inline void set_user_data(uintptr_t user_data) { _user_data = user_data; }
			// Возвращает истину, если страница не инициализирована.
			inline bool empty() const { return !_size; }
			// Возвращает истину, если все блоки свободны.
			inline bool is_all_blocks_free() const { return map.is_all_sets(); }
			// Возвращает истину, если все блоки заняты.
			inline bool is_all_blocks_busy() const { return map.is_all_unsets(); }
			// Возвращает истину, если блок за указанным индексом - свободен.
			inline bool is_block_free(size_t index) const { return map.is_set(index); }
			// Возвращает истину, если блок за указанным индексом - занят.
			inline bool is_block_busy(size_t index) const { return map.is_unset(index); }
			// Помечает, что данный блок за указанным индексом - свободен.
			inline bool set_block_free(size_t index) { return map.set(index); }
			// Помечает, что данный блок за указанным индексом - занят.
			inline bool set_block_busy(size_t index) { return map.unset(index); }
			// Возвращает истину, в случае, нахождения первого попавшегося свободного блока.
			// Его индекс будет передан в "index".
			inline bool get_first_free_block_index(size_t& index) { return map.find_first_set_bit(index); }
			// Возвращает кол-во допустимых блоков.
			inline size_t count() const { return _size; }
			// Возвращает кол-во свободных блоков.
			inline size_t free_count() const { return map.get_sets_count(); }
			// Возвращает кол-во знятых блоков.
			inline size_t busy_count() const { return map.get_unsets_count(); }
			// Возвращает блок за указанным индексом. Константа.
			inline const _type c_at(size_t index) const { return _blocks[index]; }
			// Возвращает блок за указанным индексом.
			inline _type& at(size_t index) { return _blocks[index]; }
			// Оператор обращения к объекту класса в качестве массива.
			// Возвращает блок за указанным индексом. Константа.
			inline const _type operator[](size_t index) const { return c_at(index); }
			// Оператор обращения к объекту класса в качестве массива.
			// Возвращает блок за указанным индексом.
			inline _type& operator[](size_t index) { return at(index); }
			// Вывод дампа битовой карты страницы в файл.
			inline void dump_map(const char* filename) const { map.dump(filename); }
			// Вывод дампа памяти страницы в файл.
			void dump(const char* filename) const 
			{ 
#ifndef VMMDLL_EXPORTS
				voltek::core::_internal::memory_to_file(filename, (void*)_blocks, 
					voltek::core::_internal::aligned_msize(_blocks), _size >> 3);
#endif // !VMMDLL_EXPORTS
			}
		private:
			// Конструктор копий - НЕДОСТУПЕН.
			// Страница одна и уникальна.
			page_t(const page_t& page) : _blocks(nullptr), _size(0), _user_data(0)
			{}
			// Оператор присвоения - НЕДОСТУПЕН.
			// Страница одна и уникальна.
			page_t& operator=(const page_t& page)
			{}
		private:
			// Блоки.
			_type* _blocks;
			// Кол-во доступных блоков.
			size_t _size;
			// Дополнительная информация.
			uintptr_t _user_data;
			// Битовая карта.
			_map map;
#ifdef MAPPER_USE
			// Карта памяти.
			voltek::core::mapper* _mapper;
#endif
		};


		// Шаблонный класс страницы памяти (на базе стека)
		// Является хранилищем блоков, но при этом нет жёсткой зависимости от каких блоков.
		// Служит только в качестве помечания свободный или занятый блок, а также быстрого
		// нахождения свободного блока, при помощи битовой карты.
		template<typename _type, typename _map = __VMM_PAGE_CONFIG_NORMAL_SIZE>
		class page2_t : public voltek::core::base
		{
		public:
			// Конструктор по умолчанию.
			page2_t() : _blocks(nullptr), _size(0), _user_data(0)
			{}
			// Конструктор.
			// Внимание размер будет округлён до кратности 256.
			page2_t(size_t new_size) : _blocks(nullptr), _size(0), _user_data(0)
			{
				set_size(new_size);
			}
			// Деструктор
			virtual ~page2_t()
			{
				if (_blocks)
				{
					voltek::core::_internal::aligned_free(_blocks);

					_blocks = nullptr;
					_size = 0;
				}
			}
			// Задаёт размер страницы.
			// Одноразовая, не для расширения страницы.
			// Внимание размер будет округлён до кратности 256.
			void set_size(size_t new_size)
			{
				// Число должно быть кратное 256, округляем в меньшую сторону.
				// Это необходимо, учитывая, что bits_regions размер минимум от 65536.
				// Использование SIMD инструкций является приоритетом, а "хвоста" должно 
				// быть немного, а лучше небыло вовсе.
				new_size = (new_size << 8) >> 8;

				_stack.clear();
				_stack.resize(new_size);
				_blocks = voltek::core::_internal::aligned_talloc<_type>(new_size, 0x10);

				if (!_blocks)
				{
					_stack.clear();
					_vassert(!_blocks);
				}
				else
				{
					_size = new_size;
					// Заполняем стек указателями на свободные блоки.
					_stack.push_array(_blocks, _size);
				}
			}
			// Возвращает допольнительную информацию, что привязана к странице.
			inline uintptr_t get_user_data() const { return _user_data; }
			// Устанавливает дополнительную информацию к странице.
			inline void set_user_data(uintptr_t user_data) { _user_data = user_data; }
			// Возвращает истину, если страница не инициализирована.
			inline bool empty() const { return !_size; }
			// Возвращает истину, если все блоки свободны.
			inline bool is_all_blocks_free() const { return _stack.overflowing(); }
			// Возвращает истину, если все блоки заняты.
			inline bool is_all_blocks_busy() const { return _stack.empty(); }
			// Помечает, что данный блок за указанным индексом - свободен.
			inline bool set_block_free(size_t index)
			{
				if (index >= _size) return false;
				_stack.push((_type*)(((char*)_blocks) + (index * (sizeof(_type)))));
				return true;
			}
			// Помечает, что данный блок за указанным индексом - занят.
			inline bool set_block_busy(size_t index) 
			{ 
				// Нет возможности это сделать.
				return true; 
			}
			// Возвращает истину, в случае, нахождения первого попавшегося свободного блока.
			// Его индекс будет передан в "index".
			inline bool get_first_free_block_index(size_t& index) 
			{ 
				if (_stack.empty()) return false;
				auto addr = _stack.pop();
				index = ((size_t)(addr - _blocks)) / sizeof(_type);
				return true;
			}
			// Возвращает кол-во допустимых блоков.
			inline size_t count() const { return _size; }
			// Возвращает кол-во свободных блоков.
			inline size_t free_count() const { return _stack.empty() ? 0 : _stack.caret(); }
			// Возвращает кол-во знятых блоков.
			inline size_t busy_count() const { return _stack.empty() ? _size : _size - _stack.caret(); }
			// Возвращает блок за указанным индексом. Константа.
			inline const _type c_at(size_t index) const { return _blocks[index]; }
			// Возвращает блок за указанным индексом.
			inline _type& at(size_t index) { return _blocks[index]; }
			// Оператор обращения к объекту класса в качестве массива.
			// Возвращает блок за указанным индексом. Константа.
			inline const _type operator[](size_t index) const { return c_at(index); }
			// Оператор обращения к объекту класса в качестве массива.
			// Возвращает блок за указанным индексом.
			inline _type& operator[](size_t index) { return at(index); }
			// Вывод дампа битовой карты страницы в файл.
			inline void dump_map(const char* filename) const {}
			// Вывод дампа памяти страницы в файл.
			void dump(const char* filename) const {}
		private:
			// Конструктор копий - НЕДОСТУПЕН.
			// Страница одна и уникальна.
			page2_t(const page2_t& page) : _blocks(nullptr), _size(0), _user_data(0)
			{}
			// Оператор присвоения - НЕДОСТУПЕН.
			// Страница одна и уникальна.
			page2_t& operator=(const page2_t& page)
			{}
		private:
			// Блоки.
			_type* _blocks;
			// Кол-во доступных блоков.
			size_t _size;
			// Дополнительная информация.
			uintptr_t _user_data;
			// Стек.
			core::stack<_type> _stack;
		};
	}
}