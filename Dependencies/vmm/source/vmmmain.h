// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "Voltek.MemoryManager.h"
#include "vbase.h"
#include "vmapper.h"
#include "vmmclasses.h"
#include "vmmpool.h"
#include "vsimplelock.h"
#include <array>
#include <stddef.h>

namespace voltek
{
	namespace memory_manager
	{
		// Blocks above the largest pool live in power-of-two slots from 256 KiB to 4 GiB.
		inline constexpr size_t large_slot_minimum = 256ull * 1024;
		inline constexpr size_t large_class_count = 15;
		inline constexpr size_t large_retention_budget_bytes = 64ull * 1024 * 1024;

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
			[[nodiscard]] void* alloc(size_t size) noexcept;
			[[nodiscard]] void* aligned_alloc(size_t size, size_t alignment) noexcept;
			[[nodiscard]] void* aligned_realloc(const void* ptr, size_t size, size_t alignment) noexcept;
			// Выделяет память требуемого размера из предыдущего указателя на память.
			// Память всегда выровнена.
			// Вернёт nullptr, если память физически закончилась.
			// Также если размер требуемый объявлен как 0.
			// Адрес памяти может быть изменён.
			[[nodiscard]] void* realloc(const void* ptr, size_t size) noexcept;
			// Освобождает память.
			// Вернёт ложь, если указатель не пренадлежит менеджеру.
			bool free(const void* ptr) noexcept;
			// Возвращает размер выделенной памяти под указатель.
			// Вернёт 0, что значит ошибка.
			[[nodiscard]] size_t msize(const void* ptr) const noexcept;
			[[nodiscard]] bool ready() const noexcept { return zero_size_request_block && pools; }
			void pool_stats(scalable_pool_stats& out) const noexcept;
			void flush_thread_cache() noexcept;
			// Fills one entry per pool class, then one for large blocks; returns the count written.
			size_t class_stats(scalable_class_stats* out, size_t capacity) const noexcept;
			// Вывод дампа битовой карты указанного пула
			void dump_map(size_t pool_id, const char* filename) const noexcept;
			// Вывод дампа памяти указанного пула
			void dump(size_t pool_id, const char* filename) const noexcept;
		private:
			memory_manager(const memory_manager&) = delete;
			memory_manager(memory_manager&&) = delete;
			memory_manager& operator=(memory_manager&&) = delete;
			memory_manager& operator=(const memory_manager&) = delete;
		private:
			// Принадлежит ли указатель менеджеру: заголовок внутри зарезервированного диапазона и прошёл проверку.
			[[nodiscard]] static bool owns(const void* ptr) noexcept;
			[[nodiscard]] block_base* large_alloc(size_t size) noexcept;
			void large_free(block_base* block, size_t size) noexcept;
		private:
			// Блок памяти, если запрашивают 0 размер.
			block_base* zero_size_request_block{ nullptr };
			core::retention_budget large_retention{ large_retention_budget_bytes };
			// Источники страниц пулов и крупных блоков внутри зарезервированного диапазона.
			std::array<core::mapper, pool_count> page_sources;
			std::array<core::mapper, large_class_count> large_sources;
			// Массив пулов.
			pool_t** pools{ nullptr };
			// Large blocks share one counter set; only the counters pools also keep are used.
			pool_counters large_counters{};
		};

		// Глобальный менеджер памяти, который требует инициализации.
		extern memory_manager* global_memory_manager;
	}
}