// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "vbase.h"
#include "vmmblock.h"
#include "vsimplelock.h"
#include <stddef.h>
#include <thread>

namespace voltek
{
	namespace memory_manager
	{
		constexpr static size_t POOL_8 = 0;
		constexpr static size_t POOL_16 = 1;
		constexpr static size_t POOL_32 = 2;
		constexpr static size_t POOL_64 = 3;
		constexpr static size_t POOL_128 = 4;
		constexpr static size_t POOL_256 = 5;
		constexpr static size_t POOL_512 = 6;
		constexpr static size_t POOL_1024 = 7;
		constexpr static size_t POOL_4096 = 8;
		constexpr static size_t POOL_8192 = 9;
		constexpr static size_t POOL_16384 = 10;
		constexpr static size_t POOL_32768 = 11;
		constexpr static size_t POOL_65536 = 12;
		constexpr static size_t POOL_131072 = 13;
		constexpr static size_t POOL_MAX = POOL_131072 + 1;

		// Менеджер памяти.
		class memory_manager : public voltek::core::base
		{
		public:
			// Конструктор по умолчанию.
			memory_manager();
			// Деструктор.
			virtual ~memory_manager();
			// Выделяет память требуемого размера.
			// Память всегда выровнена.
			// Вернёт nullptr, если память физически закончилась.
			// Также если размер требуемый объявлен как 0.
			void* alloc(size_t size);
			// Выделяет память требуемого размера из предыдущего указателя на память.
			// Память всегда выровнена.
			// Вернёт nullptr, если память физически закончилась.
			// Также если размер требуемый объявлен как 0.
			// Адрес памяти может быть изменён.
			void* realloc(const void* ptr, size_t size);
			// Освобождает память.
			// Вернёт ложь, если указатель не пренадлежит менеджеру.
			bool free(const void* ptr);
			// Возвращает размер выделенной памяти под указатель.
			// Вернёт 0, что значит ошибка.
			size_t msize(const void* ptr) const;
			// Вывод дампа битовой карты указанного пула
			void dump_map(size_t pool_id, const char* filename) const;
			// Вывод дампа памяти указанного пула
			void dump(size_t pool_id, const char* filename) const;
		private:
			// Конструктор копий - НЕДОСТУПЕН.
			// Менеджер один и уникален.
			memory_manager(const memory_manager& ob);
			// Оператор присвоения - НЕДОСТУПЕН.
			// Менеджер один и уникален.
			memory_manager& operator=(const memory_manager& ob);
		private:
			// Блок памяти, если запрашивают 0 размер.
			block8_t zero_size_request_block;
			// Массив пулов.
			void** pools;
			// Блокировщик для работы с множеством потоков.
			voltek::core::_internal::simple_lock lock;
			// События для потока кеширования, чтобы можно выйти
			void* event_close;
			void* event_close_w;
			// Поток для кеширования
			std::thread* thread;
		};

		// Глобальный менеджер памяти, который требует инициализации.
		extern memory_manager* global_memory_manager;
	}
}