// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Engine.h"
#include "MemoryManager.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		MemoryManager* GlobalMemoryManagerPtr = nullptr;

		MemoryManager::MemoryManager()
		{
			// Инициализация библиотеки vmm
			voltek::scalable_memory_manager_initialize();
		}

		void* MemoryManager::MemAlloc(size_t size, size_t alignment, bool aligned, bool zeroed)
		{
			// Если не задано, то будет 4
			if (!aligned)
				alignment = 4;

			if (!size)
				// Creation Kit много раз требует памяти в 0 байт, данная функция вернёт ему память с
				// постоянным адресом, не выделяя его в пулах.
				return voltek::scalable_alloc(0);

			AssertMsgVa(alignment != 0 && alignment % 2 == 0, "Alignment is fucked: %llu", alignment);

			// Должно быть в степени 2, округлить его, если необходимо
			if ((alignment & (alignment - 1)) != 0)
			{
				alignment--;
				alignment |= alignment >> 1;
				alignment |= alignment >> 2;
				alignment |= alignment >> 4;
				alignment |= alignment >> 8;
				alignment |= alignment >> 16;
				alignment++;
			}

			// Размер должен быть кратен выравниванию с округлением до ближайшего
			if ((size % alignment) != 0)
				size = ((size + alignment - 1) / alignment) * alignment;

			void* ptr = voltek::scalable_alloc(size);
			if (ptr && zeroed) memset(ptr, 0, size);

			if (!ptr && size <= (128llu * 1024 * 1024))
				AssertMsgVa(false, "A memory allocation failed. This is due to memory leaks in the Creation Kit or not"
					" having enough free RAM.\n\nRequested chunk size: %llu bytes.", size);

			return ptr;
		}

		void MemoryManager::MemFree(void* block)
		{
			voltek::scalable_free(block);
		}

		size_t MemoryManager::MemSize(void* block)
		{
			return voltek::scalable_msize(block);
		}
	}
}