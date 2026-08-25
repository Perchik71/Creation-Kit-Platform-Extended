// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include "vbase.h"
#include "vassert.h"
#include "valloc.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <iostream>

#pragma once

namespace voltek
{
	namespace core
	{
		// Класс для хранения адрессов свободных блоков.
		// Требует больше памяти, нежели чем битовая карта.
		// Но полностью исчезает потребность в поиске.
		template<typename _type>
		class stack : public base
		{
		public:
			// Тип элемента.
			using item_t = _type;
			// Тип указателя на элемент.
			using itemptr_t = item_t*;
			// Конструктор по умолчанию.
			stack() : base(), _mem(nullptr), _caret(SIZE_MAX), _size(0)
			{}
			// Конструктор.
			// В качестве параметра указывается кол-во желаемых блоков.
			stack(size_t count) : base(), _mem(nullptr), _caret(SIZE_MAX), _size(0)
			{
				resize(count);
			}
			// Конструктор копий.
			stack(const stack& ob) : base(), _mem(nullptr), _caret(SIZE_MAX), _size(0)
			{
				*this = ob;
			}
			// Деструктор.
			virtual ~stack()
			{
				clear();
			}
			// Оператор присвоения
			stack& operator=(const stack& ob)
			{
				resize(ob._size);
				if (_mem)
				{
					memcpy(_mem, ob._mem, ob._size);
					_caret = ob._caret;
				}
				return *this;
			}
			// Изменяет кол-во допустимых блоков.
			// В качестве параметра указывается кол-во желаемых блоков.
			// Если указать 0, то будет освобождена память.
			void resize(size_t count)
			{
				if (!count)
				{
					if (_mem)
					{
						voltek::core::_internal::aligned_free(_mem);
						_mem = nullptr;
						_caret = SIZE_MAX;
						_size = 0;
					}
					return;
				}

				if (_mem)
					_mem = voltek::core::_internal::aligned_retalloc<itemptr_t>(_mem, count, 0x10);
				else
					_mem = voltek::core::_internal::aligned_talloc<itemptr_t>(count, 0x10);

				_vassert(_mem);
				if (_mem)
				{
					_size = count;
					if (!empty())
						_caret = _size < _caret ? _size : _caret;
				}
			}
			// Освобождение памяти.
			inline void clear() { resize(0); }
			// Стек переполнен.
			inline bool overflowing() const { return _caret == _size; }
			// Стек пуст.
			inline bool empty() const { return _caret == SIZE_MAX; }
			// Вернёт позицию в стеке.
			inline size_t caret() const { return _caret; }
			// Добавить в стек указатель на массив.
			void push_array(const itemptr_t item, size_t size)
			{
				if (!size || !item)
					return;

				if (empty())
					_caret = 0;
				
				auto count = _size < size ? _size : size;
				for (size_t i = 0; i < count; i++)
					_mem[_caret++] = (itemptr_t)(((char*)item) + (i * sizeof(item_t)));
			}
			// Добавить в стек.
			void push(const itemptr_t item)
			{
				if (overflowing())
					return;

				if (empty())
					_caret = 0;

				_mem[_caret++] = item;
			}
			// Взять из стека.
			// Вернёт nullptr если уже пуст.
			itemptr_t pop()
			{
				if (empty())
					return nullptr;

				stack::itemptr_t item = _mem[--_caret];

				if (!_caret)
					_caret = SIZE_MAX;

				return item;
			}
		private:
			// Память.
			itemptr_t* _mem;
			// Текущая позиция в стеке.
			size_t _caret;
			// Размер стека.
			size_t _size;
		};
	}
}

