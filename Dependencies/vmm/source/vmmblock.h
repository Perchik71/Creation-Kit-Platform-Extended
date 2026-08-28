// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <stddef.h>
#include <stdint.h>

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

		// Шаблонный фиксируемый блок
		template<int _size> 
		struct block_base_t : public block_base
		{
			// Полезные данные
			char data[_size];
		};

		// Фиксируемый блок на 8 байт.
		// На самом деле его размер равен 16 байт,
		// Это нужно для выравненной памяти.
		typedef block_base_t<16> block8_t;
		// Фиксируемый блок на 16 байт.
		typedef block_base_t<16> block16_t;
		// Фиксируемый блок на 32 байт.
		typedef block_base_t<32> block32_t;
		// Фиксируемый блок на 64 байт.
		typedef block_base_t<64> block64_t;
		// Фиксируемый блок на 128 байт.
		typedef block_base_t<128> block128_t;
		// Фиксируемый блок на 256 байт.
		typedef block_base_t<256> block256_t;
		// Фиксируемый блок на 512 байт.
		typedef block_base_t<512> block512_t;
		// Фиксируемый блок на 1024 байт.
		typedef block_base_t<1024> block1024_t;
		// Фиксируемый блок на 4096 байт.
		typedef block_base_t<4096> block4096_t;
		// Фиксируемый блок на 8192 байт.
		typedef block_base_t<8192> block8192_t;
		// Фиксируемый блок на 16384 байт.
		typedef block_base_t<16384> block16384_t;
		// Фиксируемый блок на 32768 байт.
		typedef block_base_t<32768> block32768_t;
		// Фиксируемый блок на 65536 байт.
		typedef block_base_t<65536> block65536_t;
		// Фиксируемый блок на 131072 байт.
		typedef block_base_t<131072> block131072_t;

#if VOLTEK_MM_BLOCK_VERSION == 1
		static_assert(sizeof(block8_t) == 0x20, "sizeof(block8_t) == 0x20");
		static_assert(sizeof(block16_t) == 0x20, "sizeof(block16_t) == 0x20");
		static_assert(sizeof(block32_t) == 0x30, "sizeof(block32_t) == 0x30");
		static_assert(sizeof(block64_t) == 0x50, "sizeof(block64_t) == 0x50");
		static_assert(sizeof(block128_t) == 0x90, "sizeof(block128_t) == 0x90");
		static_assert(sizeof(block256_t) == 0x110, "sizeof(block256_t) == 0x110");
		static_assert(sizeof(block512_t) == 0x210, "sizeof(block512_t) == 0x210");
		static_assert(sizeof(block1024_t) == 0x410, "sizeof(block1024_t) == 0x410");
		static_assert(sizeof(block4096_t) == 0x1010, "sizeof(block4096_t) == 0x1010");
		static_assert(sizeof(block8192_t) == 0x2010, "sizeof(block8192_t) == 0x2010");
		static_assert(sizeof(block16384_t) == 0x4010, "sizeof(block16384_t) == 0x4010");
		static_assert(sizeof(block32768_t) == 0x8010, "sizeof(block32768_t) == 0x8010");
		static_assert(sizeof(block65536_t) == 0x10010, "sizeof(block65536_t) == 0x10010");
		static_assert(sizeof(block131072_t) == 0x20010, "sizeof(block131072_t) == 0x20010");
#elif VOLTEK_MM_BLOCK_VERSION == 2
		static_assert(sizeof(block8_t) == 0x30, "sizeof(block8_t) == 0x30");
		static_assert(sizeof(block16_t) == 0x30, "sizeof(block16_t) == 0x30");
		static_assert(sizeof(block32_t) == 0x40, "sizeof(block32_t) == 0x40");
		static_assert(sizeof(block64_t) == 0x60, "sizeof(block64_t) == 0x60");
		static_assert(sizeof(block128_t) == 0xA0, "sizeof(block128_t) == 0xA0");
		static_assert(sizeof(block256_t) == 0x120, "sizeof(block256_t) == 0x120");
		static_assert(sizeof(block512_t) == 0x220, "sizeof(block512_t) == 0x220");
		static_assert(sizeof(block1024_t) == 0x420, "sizeof(block1024_t) == 0x420");
		static_assert(sizeof(block4096_t) == 0x1020, "sizeof(block4096_t) == 0x1020");
		static_assert(sizeof(block8192_t) == 0x2020, "sizeof(block8192_t) == 0x2020");
		static_assert(sizeof(block16384_t) == 0x4020, "sizeof(block16384_t) == 0x4020");
		static_assert(sizeof(block32768_t) == 0x8020, "sizeof(block32768_t) == 0x8020");
		static_assert(sizeof(block65536_t) == 0x10020, "sizeof(block65536_t) == 0x10020");
		static_assert(sizeof(block131072_t) == 0x20020, "sizeof(block131072_t) == 0x20020");
#endif

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

		// Возвращает истину, если блок правильный и пренадлежит менеджеру.
		inline static bool is_valid_block(const block_base* block)
		{
			return block->prologue == prologue_block;
		}

		// Возвращает истину, если блок используется каким-то пулом.
		inline static bool is_used_pool_block(const block_base* block)
		{
			return (block->flags & flag_block_pool_used) == flag_block_pool_used;
		}

		// Возвращает истину, если блок выделен просто, его нет в пулах.
		inline static bool is_used_default_block(const block_base* block)
		{
			return (block->flags & flag_block_default_used) == flag_block_default_used;
		}

#if VOLTEK_MM_BLOCK_VERSION == 1
		// Возвращает размер памяти указанный в блоке или 0, если он неправильный.
		inline static size_t get_size_from_block(const block_base* block)
		{
			return is_valid_block(block) ? (is_used_default_block(block) ? block->default_block.size : block->size) : 0;
		}

		// Изменяет размер памяти указанном в блоке.
		inline static bool set_size_from_block(block_base* block, size_t new_size)
		{
			bool ret = is_valid_block(block);
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
			dst->flags = flag_block_pool_used;
			return dst;
		}

		// Функция инициализации блока в качестве обычного не пуловского
		inline static block_base* create_default_block(block_base* dst, size_t size)
		{
			dst->prologue = prologue_block;
			dst->default_block.size = size;
			dst->flags = flag_block_default_used;
			return dst;
		}
	}
}
