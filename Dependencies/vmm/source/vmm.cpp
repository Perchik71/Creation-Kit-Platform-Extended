// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include "Voltek.MemoryManager.h"

#include "vmmmain.h"
#include "vassert.h"
#include <string.h>

namespace voltek
{
	VOLTEK_MM_API void scalable_memory_manager_initialize()
	{
		if (!memory_manager::global_memory_manager)
			memory_manager::global_memory_manager = new memory_manager::memory_manager();
	}

	VOLTEK_MM_API void scalable_memory_manager_shutdown()
	{
		if (memory_manager::global_memory_manager)
		{
			delete memory_manager::global_memory_manager;
			memory_manager::global_memory_manager = nullptr;
		}
	}

	VOLTEK_MM_API void* scalable_alloc(size_t size)
	{
		if (!memory_manager::global_memory_manager) return nullptr;
		return memory_manager::global_memory_manager->alloc(size);
	}

	VOLTEK_MM_API void* scalable_calloc(size_t count, size_t size)
	{
		size_t need_size = count * size;
		void* ptr = scalable_alloc(need_size);
		if (ptr) memset(ptr, 0, need_size);
		return ptr;
	}

	VOLTEK_MM_API void* scalable_realloc(const void* ptr, size_t size)
	{
		if (!memory_manager::global_memory_manager) return nullptr;
		return memory_manager::global_memory_manager->realloc(ptr, size);
	}

	VOLTEK_MM_API void* scalable_recalloc(const void* ptr, size_t count, size_t size)
	{
		if (!ptr || !memory_manager::global_memory_manager) return nullptr;
		// Получение размера памяти, что было ранее.
		size_t old_size = memory_manager::global_memory_manager->msize(ptr);
		// Новый требуемый размер памяти.
		size_t need_size = count * size;
		void* new_ptr = memory_manager::global_memory_manager->realloc(ptr, need_size);
		if (new_ptr && (old_size < need_size)) 
			memset((void*)((char*)new_ptr + old_size), 0, need_size - old_size);
		return new_ptr;
	}

	VOLTEK_MM_API bool scalable_free(const void* ptr)
	{
		if (!memory_manager::global_memory_manager) return false;
		return memory_manager::global_memory_manager->free(ptr);
	}

	VOLTEK_MM_API size_t scalable_msize(const void* ptr)
	{
		if (!memory_manager::global_memory_manager) return 0;
		return memory_manager::global_memory_manager->msize(ptr);
	}
}