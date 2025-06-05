// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include "valloc.h"
#include "vmapper.h"
#include "vsimplelock.h"
#include <malloc.h>
#include <string.h>

#include <Windows.h>
#include <comdef.h>

#define VOLTEK_DEFAULT_HEAP_SIZE ((uint64_t)8ull * 1024 * 1024 * 1024)

namespace voltek
{
	namespace core
	{
		namespace _internal
		{
#if 0
			class heap_allocator
			{
				uint8_t* _start_address;
				uint64_t _size;
				simple_lock _locker;

				struct block_header_t
				{
					uint64_t size;                 // physical size of block
					uint64_t allocated;            // allocated size; 0 means free
					block_header_t* next;
				};

				inline uint64_t alloc_size(uint64_t size) const noexcept(true) { return (size + 7) & ~7; }
			public:
				heap_allocator(uint64_t size = VOLTEK_DEFAULT_HEAP_SIZE);
				~heap_allocator();

				inline void* get_start_address() const noexcept(true) { return _start_address; }

				void* alloc(uint64_t size) noexcept(true);
				void* realloc(void* pointer, uint64_t size) noexcept(true);
				void free(void* pointer) noexcept(true);
			};
			
			heap_allocator::heap_allocator(uint64_t size) :
				_size(size), _start_address(nullptr)
			{
				if (_size < sizeof(block_header_t))
					_size = VOLTEK_DEFAULT_HEAP_SIZE;

				_start_address = (uint8_t*)VirtualAlloc(nullptr, _size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
				if (!_start_address)
				{
					MessageBoxA(0, _com_error(GetLastError()).ErrorMessage(), "Heap Allocator Error", 0);
					TerminateProcess(GetCurrentProcess(), 1);
					return;
				}

				auto block_hdr = (block_header_t*)_start_address;

				block_hdr->size = _size;
				block_hdr->next = nullptr;
				block_hdr->allocated = 0;
			}

			heap_allocator::~heap_allocator()
			{
				if (_start_address)
				{
					VirtualFree(_start_address, 0, MEM_RELEASE);
					_start_address = nullptr;
					_size = 0;
				}
			}
			
			void* heap_allocator::alloc(uint64_t size) noexcept(true)
			{
				if (!size)
					return nullptr;

				simple_scope_lock lock(_locker);

				auto block_hdr = (block_header_t*)_start_address;
				auto need_alloc_size = alloc_size(size);
				void* new_pointer = nullptr;

				while (true)
				{
					if (!block_hdr)
						return nullptr;

					if (block_hdr->allocated)
					{ 
						// skip it
						block_hdr = block_hdr->next;
						continue;
					} 

					if (block_hdr->size >= size)
					{
						// it's big enough, split or not?
						auto delta = block_hdr->size - need_alloc_size;
						if (delta == 0)
						{ 
							// perfect match
							new_pointer = (void*)&block_hdr[1];
							block_hdr->allocated = size;
							break;
						}

						// if delta is too small to hold a header + 32 (minimal block) 
						// bytes of useful space, do not split
						if (delta < (sizeof(block_header_t) + 32))
						{
							// no split
							new_pointer = (void*)&block_hdr[1];
							block_hdr->allocated = size;
							break;
						}

						auto new_hdr = (block_header_t*)(((uint8_t*)block_hdr) + need_alloc_size + sizeof(block_header_t));

						new_hdr->size = delta - sizeof(block_header_t);
						new_hdr->allocated = 0;							 // no space is allocated in this new block
						new_hdr->next = block_hdr->next;

						block_hdr->next = new_hdr;
						block_hdr->size = need_alloc_size;
						block_hdr->allocated = size;
						new_pointer = (void*)&block_hdr[1];

						// this is the structure we now have
						break;
					}
					else
					{
						// it's too small
						// Skip this block if it is too small
						block_hdr = block_hdr->next;
					} 
				}

				return new_pointer;
			}

			void* heap_allocator::realloc(void* pointer, uint64_t size) noexcept(true)
			{
				simple_scope_lock lock(_locker);

				if (!pointer)
					return alloc(size);

				if (!size)
				{
					free(pointer);
					return nullptr;
				}

				auto new_pointer = alloc(size);
				if (new_pointer)
				{
					memcpy(new_pointer, pointer, size);
					free(pointer);
				}

				return new_pointer;
			}

			void heap_allocator::free(void* pointer) noexcept(true)
			{
				if (!pointer) return;
				simple_scope_lock lock(_locker);
				auto block_hdr = (block_header_t*)((uint8_t*)pointer - sizeof(block_header_t));
				block_hdr->allocated = 0;
			}

			heap_allocator g_heap_allocator;
#endif

			// В разных ОС свой msize, более того, для линукса, подобная функция только для отладки.
			// И никаких гарантий, что она вернёт тот размер, что указали функции при выделении памяти.
			// Поэтому откажусь от этой функции и реализую собственную.

			struct pointer_handle
			{
				void* original;
				size_t alloc_size;
			};

			inline static void* _malloc(size_t size)
			{
#if 0
				auto new_pointer = g_heap_allocator.alloc(size);
				if (new_pointer) return new_pointer;
#endif
				return malloc(size);
			}

			inline static void* _realloc(void* ptr, size_t new_size)
			{
#if 0
				auto new_pointer = g_heap_allocator.realloc(ptr, new_size);
				if (new_pointer) return new_pointer;
#endif
				return realloc(ptr, new_size);
			}

			inline static void _free(void* ptr)
			{
#if 0
				g_heap_allocator.free(ptr);
#else
				free(ptr);
#endif
			}

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