// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <atomic>

#define VOLTEK_MM_BLOCK_VERSION 1

namespace voltek
{

#if VOLTEK_MM_BLOCK_VERSION == 1
#elif VOLTEK_MM_BLOCK_VERSION == 2
#endif
	namespace memory_manager
	{
#if VOLTEK_MM_BLOCK_VERSION == 1
#pragma pack(push, 1)
		// Заголовок блока
		struct block_base
		{
			// Для проверки на валидность блока, от иной памяти выделенной, чем-то иным.
			uint32_t prologue;

			union
			{
				struct
				{
					// Размер полезных данных.
					uint32_t size;
					// Номер страницы.
					uint16_t page_id;
					// Номер блока в странице.
					uint32_t block_id;
				};

				struct ssize_union
				{
					// Размер полезных данных.
					uint64_t size;
				} default_block;
			};

			// Флаги (состояния, доп. инфа).
			uint8_t flags;
			// Номер пула.
			uint8_t pool_id;
		};
#pragma pack(pop)

		static_assert(sizeof(block_base) == 0x10, "sizeof(block_base) == 0x10");
#elif VOLTEK_MM_BLOCK_VERSION == 2
#pragma pack(push, 1)
		// Заголовок блока
		struct block_base
		{
			// Для проверки на валидность блока, от иной памяти выделенной, чем-то иным.
			uint32_t prologue;
			// Номер страницы.
			uint32_t page_id;
			// Номер блока в странице.
			uint32_t block_id;
			// Флаги (состояния, доп. инфа).
			uint16_t flags;
			// Номер пула.
			uint16_t pool_id;
			// Размер полезных данных.
			uint64_t size;
			// Зарезервировано
			uint64_t reserved;
		};
#pragma pack(pop)

		static_assert(sizeof(block_base) == 0x20, "sizeof(block_base) == 0x20");
#endif

		struct aligned_block
		{
			void* base;
			size_t alignment;
			block_base header;
		};

		static_assert(offsetof(aligned_block, header) + sizeof(block_base) == sizeof(aligned_block));

		// Для проверки на валидность блока, от иной памяти выделенной, чем-то иным.
		static constexpr uint32_t prologue_block = 0xdadafead;
#if VOLTEK_MM_BLOCK_VERSION == 1
		// Флаг, который говорит, что блок используется каким-то пулом.
		static constexpr uint8_t flag_block_pool_used = 0x1;
		// Флаг, который говорит, что блок выделен просто и его нет в пулах.
		static constexpr uint8_t flag_block_default_used = 0x2;
#elif VOLTEK_MM_BLOCK_VERSION == 2
		// Флаг, который говорит, что блок используется каким-то пулом.
		static constexpr uint16_t flag_block_pool_used = 0x1;
		// Флаг, который говорит, что блок выделен просто и его нет в пулах.
		static constexpr uint16_t flag_block_default_used = 0x2;
#endif

		static constexpr uint8_t flag_block_aligned = 0x4;
		// Allocated while statistics were on, so its release is counted too.
		static constexpr uint8_t flag_block_counted = 0x8;
		// Released to its page's free list; a second free is ignored.
		static constexpr uint8_t flag_block_free = 0x10;
		static constexpr uint8_t flag_block_cached = 0x20;

		static_assert(std::atomic_ref<uint8_t>::required_alignment == 1);
		static_assert(offsetof(block_base, flags) == 14);

		struct block_lifecycle
		{
			static uint8_t load(const block_base* block) noexcept
			{
				return std::atomic_ref<uint8_t>(const_cast<uint8_t&>(block->flags)).load(std::memory_order_acquire);
			}

			static void store(block_base* block, uint8_t flags) noexcept
			{
				std::atomic_ref<uint8_t>(block->flags).store(flags, std::memory_order_release);
			}

			static bool live(const block_base* block) noexcept
			{
				return !(load(block) & (flag_block_free | flag_block_cached));
			}

			static uint8_t try_cache(block_base* block) noexcept
			{
				auto flags = load(block);
				if (!(flags & flag_block_pool_used) || (flags & (flag_block_free | flag_block_cached)))
					return 0;
				const auto old = flags;
				return std::atomic_ref<uint8_t>(block->flags).compare_exchange_strong(flags,
					static_cast<uint8_t>((flags & ~flag_block_counted) | flag_block_cached),
					std::memory_order_acq_rel, std::memory_order_acquire) ? old : 0;
			}

			static void uncache(block_base* block, bool counted) noexcept
			{
				store(block, flag_block_pool_used | (counted ? flag_block_counted : 0));
			}

			static bool try_free(block_base* block) noexcept
			{
				auto flags = load(block);
				if (!(flags & flag_block_pool_used) || (flags & flag_block_free) || (flags & flag_block_cached))
					return false;
				// An allocated block can still race a lock-free cache admission.
				return std::atomic_ref<uint8_t>(block->flags).compare_exchange_strong(flags,
					flag_block_pool_used | flag_block_free, std::memory_order_acq_rel, std::memory_order_acquire);
			}

			static bool free_cached_under_lock(block_base* block) noexcept
			{
				const auto flags = load(block);
				if (!(flags & flag_block_pool_used) || !(flags & flag_block_cached) || (flags & flag_block_free))
					return false;
				// Only the owning bin can return a cached block, under the pool lock.
				store(block, flag_block_pool_used | flag_block_free);
				return true;
			}

			static bool pop_under_lock(block_base* block, uint32_t requested, uint8_t target_flags) noexcept
			{
				auto flags = load(block);
				if (!(flags & flag_block_free))
					return false;
				block->size = requested;
				// Free blocks cannot be cached until the pool-lock holder publishes their final state.
				store(block, target_flags);
				return true;
			}

			static bool try_free_default(block_base* block) noexcept
			{
				uint8_t flags = flag_block_default_used;
				return std::atomic_ref<uint8_t>(block->flags).compare_exchange_strong(flags,
					flag_block_default_used | flag_block_free, std::memory_order_acq_rel, std::memory_order_acquire);
			}
		};

		// Возвращает истину, если блок правильный и пренадлежит менеджеру.
		inline static bool is_valid_block(const block_base* block)
		{
			return block->prologue == prologue_block;
		}

		// Возвращает истину, если блок используется каким-то пулом.
		inline static bool is_used_pool_block(const block_base* block)
		{
			return (block_lifecycle::load(block) & flag_block_pool_used) == flag_block_pool_used;
		}

		// Возвращает истину, если блок выделен просто, его нет в пулах.
		inline static bool is_used_default_block(const block_base* block)
		{
			return (block_lifecycle::load(block) & flag_block_default_used) == flag_block_default_used;
		}

		inline static bool is_used_aligned_block(const block_base* block)
		{
			return (block_lifecycle::load(block) & flag_block_aligned) != 0;
		}

		inline static aligned_block* get_aligned_block(block_base* block)
		{
			return reinterpret_cast<aligned_block*>(
				reinterpret_cast<char*>(block) - offsetof(aligned_block, header));
		}

#if VOLTEK_MM_BLOCK_VERSION == 1
		// Возвращает размер памяти указанный в блоке или 0, если он неправильный.
		inline static size_t get_size_from_block(const block_base* block)
		{
			return is_valid_block(block) && block_lifecycle::live(block) ?
				(is_used_default_block(block) ? block->default_block.size : block->size) : 0;
		}

		// Изменяет размер памяти указанном в блоке.
		inline static bool set_size_from_block(block_base* block, size_t new_size)
		{
			bool ret = is_valid_block(block) && block_lifecycle::live(block);
			if (ret)
			{
				if (is_used_default_block(block))
					block->default_block.size = new_size;
				else
					block->size = (uint32_t)new_size;
			}
			return ret;
		}
#elif VOLTEK_MM_BLOCK_VERSION == 2
		// Возвращает размер памяти указанный в блоке или 0, если он неправильный.
		inline static size_t get_size_from_block(const block_base* block)
		{
			return is_valid_block(block) ? block->size : 0;
		}

		// Изменяет размер памяти указанном в блоке.
		inline static bool set_size_from_block(block_base* block, size_t new_size)
		{
			bool ret = is_valid_block(block);
			if (ret)
				block->size = (uint64_t)new_size;
			return ret;
		}
#endif

#if VOLTEK_MM_BLOCK_VERSION == 1
		// Возвращает номер пула, если указанный в блоке он пуловский и правильный,
		// иначе вернёт (uint8_t)-1.
		inline static uint8_t get_pool_id_from_block(const block_base* block)
		{
			return (is_valid_block(block) && is_used_pool_block(block)) ? block->pool_id : (uint8_t)-1;
		}
#elif VOLTEK_MM_BLOCK_VERSION == 2
		// Возвращает номер пула, если указанный в блоке он пуловский и правильный,
		// иначе вернёт (uint16_t)-1.
		inline static uint16_t get_pool_id_from_block(const block_base* block)
		{
			return (is_valid_block(block) && is_used_pool_block(block)) ? block->pool_id : (uint16_t)-1;
		}
#endif
	
		// Возвращает номер блока в странице, если указанный в блоке он пуловский и правильный,
		// иначе вернёт (uint32_t)-1.
		inline static uint32_t get_block_id_from_block(const block_base* block)
		{
			return (is_valid_block(block) && is_used_pool_block(block)) ? block->block_id : (uint32_t)-1;
		}
		
#if VOLTEK_MM_BLOCK_VERSION == 1
		// Возвращает номер страницы, если указанный в блоке он пуловский и правильный,
		// иначе вернёт (uint16_t)-1.
		inline static uint16_t get_page_id_from_block(const block_base* block)
		{
			return (is_valid_block(block) && is_used_pool_block(block)) ? block->page_id : (uint16_t)-1;
		}
#elif VOLTEK_MM_BLOCK_VERSION == 2
		// Возвращает номер страницы, если указанный в блоке он пуловский и правильный,
		// иначе вернёт (uint32_t)-1.
		inline static uint32_t get_page_id_from_block(const block_base* block)
		{
			return (is_valid_block(block) && is_used_pool_block(block)) ? block->page_id : (uint32_t)-1;
		}
#endif
		
		// Возвращает заголовок блока из указателя.
		inline static block_base* get_block_handle_from_ptr(const void* ptr)
		{
			return (block_base*)((char*)ptr - sizeof(block_base));
		}

		// Возвращает указатель на полезные данные блока.
		inline static void* get_ptr_from_block_handle(block_base* block)
		{
			return (void*)((char*)block + sizeof(block_base));
		}

		// Возвращает истину, если указатель правильный и пренадлежит менеджеру.
		inline static bool is_valid_ptr(const void* ptr)
		{
			return is_valid_block(get_block_handle_from_ptr(ptr));
		}

		// Возвращает истину, если указатель используется каким-то пулом.
		inline static bool is_used_pool_ptr(const void* ptr)
		{
			return is_used_pool_block(get_block_handle_from_ptr(ptr));
		}

		// Возвращает истину, если указатель выделен просто, его нет в пулах.
		inline static bool is_used_default_ptr(const void* ptr)
		{
			return is_used_default_block(get_block_handle_from_ptr(ptr));
		}

		// Возвращает размер памяти указанный в указателе или 0, если он неправильный.
		inline static size_t get_size_from_ptr(const void* ptr)
		{
			return get_size_from_block(get_block_handle_from_ptr(ptr));
		}

		// Изменяет размер памяти указанном в указателе.
		inline static bool set_size_from_ptr(void* ptr, size_t new_size)
		{
			return set_size_from_block(get_block_handle_from_ptr(ptr), new_size);
		}

#if VOLTEK_MM_BLOCK_VERSION == 1
		// Возвращает номер пула, если указанный в указателе он пуловский и правильный,
		// иначе вернёт (uint8_t)-1.
		inline static uint8_t get_pool_id_from_ptr(const void* ptr)
		{
			return get_pool_id_from_block(get_block_handle_from_ptr(ptr));
		}

		// Возвращает номер блока в странице, если указанный в указателе он пуловский и правильный,
		// иначе вернёт (uint32_t)-1.
		inline static uint32_t get_block_id_from_ptr(const void* ptr)
		{
			return get_block_id_from_block(get_block_handle_from_ptr(ptr));
		}

		// Возвращает номер страницы, если указанный в указателе он пуловский и правильный,
		// иначе вернёт (uint16_t)-1.
		inline static uint16_t get_page_id_from_ptr(const void* ptr)
		{
			return get_page_id_from_block(get_block_handle_from_ptr(ptr));
		}
#elif VOLTEK_MM_BLOCK_VERSION == 2
		// Возвращает номер пула, если указанный в указателе он пуловский и правильный,
		// иначе вернёт (uint16_t)-1.
		inline static uint16_t get_pool_id_from_ptr(const void* ptr)
		{
			return get_pool_id_from_block(get_block_handle_from_ptr(ptr));
		}

		// Возвращает номер блока в странице, если указанный в указателе он пуловский и правильный,
		// иначе вернёт (uint32_t)-1.
		inline static uint32_t get_block_id_from_ptr(const void* ptr)
		{
			return get_block_id_from_block(get_block_handle_from_ptr(ptr));
		}

		// Возвращает номер страницы, если указанный в указателе он пуловский и правильный,
		// иначе вернёт (uint32_t)-1.
		inline static uint32_t get_page_id_from_ptr(const void* ptr)
		{
			return get_page_id_from_block(get_block_handle_from_ptr(ptr));
		}
#endif

		// Функция инициализации блока для пула
		inline static block_base* create_pool_block(block_base* dst, uint32_t size, uint16_t page_id, uint32_t block_id, uint8_t pool_id)
		{
			dst->prologue = prologue_block;
			dst->pool_id = pool_id;
			dst->page_id = page_id;
			dst->block_id = block_id;
			dst->size = size;
			return dst;
		}

		// Функция инициализации блока в качестве обычного не пуловского
		inline static block_base* create_default_block(block_base* dst, size_t size)
		{
			dst->prologue = prologue_block;
			dst->default_block.size = size;
			block_lifecycle::store(dst, flag_block_default_used);
			return dst;
		}
	}
}
