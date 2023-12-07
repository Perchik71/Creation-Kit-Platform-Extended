// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include "valloc.h"
#include <malloc.h>
#include <string.h>

#ifdef VOLTEK_VIRTUAL_ALLOC_WIN
#	if (defined(_WIN32) || defined(_WIN64))	
#		pragma warning (disable : 6333)
#		pragma warning (disable : 28160)
#		define WIN32_LEAN_AND_MEAN
#		define VOLTEK_VIRTUAL_ALLOC_WIN_ENABLE
#		include <windows.h>
#	endif
#endif

namespace voltek
{
	namespace core
	{
		namespace _internal
		{
			// В разных ОС свой msize, более того, для линукса, подобная функция только для отладки.
			// И никаких гарантий, что она вернёт тот размер, что указали функции при выделении памяти.
			// Поэтому откажусь от этой функции и реализую собственную.

			struct pointer_handle
			{
				void* original;
				size_t alloc_size;
#if VOLTEK_VIRTUAL_ALLOC_WIN_ENABLE
				size_t real_size;
#endif
			};

#if VOLTEK_VIRTUAL_ALLOC_WIN_ENABLE
			void* _malloc(size_t size)
			{
				return (void*)VirtualAlloc(NULL, (SIZE_T)size, 
					MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			}

			void* _realloc(void* ptr, size_t new_size)
			{
				pointer_handle* handle = (pointer_handle*)ptr;
				if (new_size == handle->real_size) return ptr;
				void* new_ptr = (void*)VirtualAlloc(NULL, (SIZE_T)new_size,
					MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
				if (new_ptr)
					memcpy(new_ptr, handle->original,
						(handle->real_size > new_size) ? new_size : handle->real_size);
				VirtualFree(handle->original, handle->real_size, MEM_RELEASE);
				return new_ptr;
			}

			void _free(void* ptr)
			{
				pointer_handle* handle = (pointer_handle*)ptr;
				VirtualFree(handle->original, handle->real_size, MEM_RELEASE);
			}
#else
			inline void* _malloc(size_t size) { return malloc(size); }
			inline void* _realloc(void* ptr, size_t new_size) { return realloc(ptr, new_size); }
			inline void _free(void* ptr) { free(ptr); }
#endif

			void* aligned_malloc(size_t size, size_t alignment)
			{
				if (!size)
					return nullptr;

				if (!alignment)
					alignment = 4;

				// Должно быть в степени 2, округлить его, если необходимо
				if ((alignment % 2) != 0)
					alignment = ((alignment + 1) >> 1) << 1;

				// Выделим память больше положенного для размещения указателя и его размера
				size_t offset = alignment - 1 + sizeof(pointer_handle);
				void* original_ptr = _malloc(size + offset);
				if (!original_ptr) return nullptr;
				size_t real = (((size_t)original_ptr) + offset) & ~(alignment - 1);
				pointer_handle* handle = (pointer_handle*)(real - sizeof(pointer_handle));
					
				handle->original = original_ptr;
				handle->alloc_size = size;
#if VOLTEK_VIRTUAL_ALLOC_WIN_ENABLE
				handle->real_size = size + offset;
#endif

				return (void*)real;
			}

			void* aligned_calloc(size_t count, size_t size, size_t alignment)
			{
				size_t alloc_size = count * size;
				void* ptr = aligned_malloc(alloc_size, alignment);
				if (ptr) memset(ptr, 0, alloc_size);
				return ptr;
			}

			void* aligned_realloc(void* ptr, size_t size, size_t alignment)
			{
				if (!size || !ptr)
					return nullptr;

				pointer_handle* old_handle = (pointer_handle*)((size_t)ptr - sizeof(pointer_handle));
				if (old_handle->alloc_size == size)
					return ptr;

				if (!alignment)
					alignment = 4;

				// Должно быть в степени 2, округлить его, если необходимо
				if ((alignment % 2) != 0)
					alignment = ((alignment + 1) >> 1) << 1;

				// Выделим память больше положенного для размещения указателя и его размера
				size_t offset = alignment - 1 + sizeof(pointer_handle);
				void* original_ptr = _realloc(old_handle->original, size + offset);
				if (!original_ptr) return nullptr;
				size_t real = (((size_t)original_ptr) + offset) & ~(alignment - 1);
				pointer_handle* handle = (pointer_handle*)(real - sizeof(pointer_handle));

				handle->original = original_ptr;
				handle->alloc_size = size;
#if VOLTEK_VIRTUAL_ALLOC_WIN_ENABLE
				handle->real_size = size + offset;
#endif

				return (void*)real;
			}

			void* aligned_recalloc(void* ptr, size_t count, size_t size, size_t alignment)
			{
				if (!size || !count || !ptr)
					return nullptr;

				pointer_handle* old_handle = (pointer_handle*)((size_t)ptr - sizeof(pointer_handle));
				size_t old_alloc_size = old_handle->alloc_size;
				size_t new_alloc_size = count * size;
				void* new_ptr = aligned_realloc(ptr, new_alloc_size, alignment);
				if (new_ptr && (new_alloc_size > old_alloc_size))
					memset((void*)((uintptr_t)new_ptr + old_alloc_size), 0, new_alloc_size - old_alloc_size);
				return new_ptr;
			}

			void aligned_free(void* ptr)
			{
				if (ptr)
				{
					pointer_handle* handle = (pointer_handle*)((size_t)ptr - sizeof(pointer_handle));
					_free(handle->original);
				}
			}

			size_t aligned_msize(const void* ptr)
			{
				if (!ptr) return 0;

				pointer_handle* handle = (pointer_handle*)((size_t)ptr - sizeof(pointer_handle));
				return handle->alloc_size;
			}
		}
	}
}