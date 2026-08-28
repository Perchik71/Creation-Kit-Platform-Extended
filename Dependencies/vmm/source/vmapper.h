// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "vbits.h"

namespace voltek
{
	namespace core
	{
		// Карта памяти, заранее зарезервированная, для конкретного пула, 
		// чтобы быстро получить нужную память для страницы.
		// Факт: ускорение 2%... заброшено.
		class mapper : public base
		{
		public:
			// Размер по умолчанию.
			inline static constexpr size_t DEFAULT_SIZE = 256 * 1024 * 1024;
			// Размер одного блока по умолчанию.
			inline static constexpr size_t DEFAULT_BLOCKSIZE = DEFAULT_SIZE / 65536;
			// Конструктор по умолчанию.
			mapper();
			// Конструктор.
			// В качестве параметра указывается желаемый размер в мб.
			// И размер одного блока в байтах.
			mapper(size_t size, size_t blocksize);
			// Конструктор копий.
			mapper(const bits& ob);
			// Деструктор.
			virtual ~mapper();
			// Оператор присвоения
			mapper& operator=(const mapper& ob);
			// Функция возвращает если объект класса пуст и нет памяти.
			inline bool empty() const { return !_size; }
			// Освобождение памяти.
			inline void clear() { resize(0, 0); }
			// Возвращает размер допустимой памяти.
			inline size_t size() const { return _size; }
			// Возвращает размер свободной памяти.
			inline size_t free_size() const { return _freesize; }
			// Возвращает размер одного блока.
			inline size_t block_size() const { return _blocksize; }
			// Возвращает кол-во всего допустимых блоков.
			inline size_t count_blocks() const { return _size / _blocksize; }
			// Возвращает кол-во свободных блоков.
			inline size_t free_blocks() const { return _freesize / _blocksize; }
			// Возвращает указатель на память, константа.
			inline const char* c_data() const { return _mem; }
			// Возвращает указатель на память, не константа.
			inline char* data() { return _mem; }
			// Возвращает указатель на память свободного блока.
			void* block_alloc();
			// Освобождает память блока по указателю на память.
			bool block_free(const void* ptr);
		private:
			// Изменяет размер памяти.
			// В качестве параметра указывается желаемый размер в байтах.
			// Если указать 0, то будет освобождена память.
			// И размер одного блока в байтах.
			void resize(size_t size, size_t blocksize);
			// Возвращает истину если указатель принадлежит классу. 
			bool is_valid_ptr(const void* ptr) const;
		private:
			// Память.
			char* _mem;
			// Допустимый размер.
			size_t _size;
			// Свободно.
			size_t _freesize;
			// Размер одного блока.
			size_t _blocksize;
			// Битовая карта для быстрого поиска.
			bits* _mask;
		};
	}
}
