// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma warning(disable : 6333)
#pragma warning(disable : 26819)
#pragma warning(disable : 28160)

#include "vmapper.h"
#include "vmmmain.h"
#include "vmmpool.h"
#include <limits.h>
#include <string.h>
#include <windows.h>

//#pragma warning(disable : 4996)
//#include <stdio.h>

namespace voltek
{
	namespace core
	{
		// Инициализация систем.
		void initialize();
	}

	namespace memory_manager
	{
		memory_manager* global_memory_manager = nullptr;

		constexpr size_t MAX_BLOCK_SIZE = 1ull * 1024 * 1024 * 1024;

		typedef page_t<block8_t> page8_t;
		typedef page_t<block16_t> page16_t;
		typedef page_t<block32_t> page32_t;
		typedef page_t<block64_t> page64_t;
		typedef page_t<block128_t> page128_t;
		typedef page_t<block256_t> page256_t;
		typedef page_t<block512_t> page512_t;
		typedef page_t<block1024_t> page1024_t;
		typedef page_t<block4096_t> page4096_t;
		typedef page_t<block8192_t> page8192_t;
		typedef page_t<block16384_t, __VMM_PAGE_CONFIG_SMALL_SIZE> page16384_t;
		typedef page_t<block32768_t, __VMM_PAGE_CONFIG_SMALL_SIZE> page32768_t;
		typedef page_t<block65536_t, __VMM_PAGE_CONFIG_LOW_SIZE> page65536_t;
		typedef page_t<block131072_t, __VMM_PAGE_CONFIG_LOW_SIZE> page131072_t;
		typedef pool_t<block8_t, page8_t, __VMM_POOL_CONFIG_BIG_SIZE> pool8_t;
		typedef pool_t<block16_t, page16_t, __VMM_POOL_CONFIG_BIG_SIZE> pool16_t;
		typedef pool_t<block32_t, page32_t, __VMM_POOL_CONFIG_BIG_SIZE> pool32_t;
		typedef pool_t<block64_t, page64_t, __VMM_POOL_CONFIG_LARGE_SIZE> pool64_t;
		typedef pool_t<block128_t, page128_t, __VMM_POOL_CONFIG_LARGE_SIZE> pool128_t;
		typedef pool_t<block256_t, page256_t> pool256_t;
		typedef pool_t<block512_t, page512_t> pool512_t;
		typedef pool_t<block1024_t, page1024_t> pool1024_t;
		typedef pool_t<block4096_t, page4096_t> pool4096_t;
		typedef pool_t<block8192_t, page8192_t> pool8192_t;
		typedef pool_t<block16384_t, page16384_t, __VMM_POOL_CONFIG_SMALL_SIZE> pool16384_t;
		typedef pool_t<block32768_t, page32768_t, __VMM_POOL_CONFIG_SMALL_SIZE> pool32768_t;
		typedef pool_t<block65536_t, page65536_t, __VMM_POOL_CONFIG_LOW_SIZE> pool65536_t;
		typedef pool_t<block131072_t, page131072_t, __VMM_POOL_CONFIG_LOW_SIZE> pool131072_t;


		// Проверка на допустимость памяти
		// Только Windows: Если произошло исключение, то вернёт false, иначе true.
		bool is_valid_pointer(const void* ptr)
		{
			if (!ptr) return false;

#if (defined(_WIN32) || defined(_WIN64))	
			__try
			{
				// Попытка что-то разыменовать и записать...
				if (*((char*)(ptr)) != 0)
					*(char*)(ptr) = *(char*)(ptr);

				return true;
			}
			__except (1)
			{
				return false;
			}
#else
			return true;
#endif
		}

//		static FILE* file_dbg_sniffer;

#define _fsniff(fmt, ...) \
	{ fprintf(file_dbg_sniffer, fmt "\n", ##__VA_ARGS__); fflush(file_dbg_sniffer); }

		static size_t POOL_SIZE = 64 * 1024;

		memory_manager::memory_manager() : pools(nullptr), thread(nullptr)
		{
			core::initialize();
			create_default_block(&zero_size_request_block, 0);
			
			event_close = CreateEventA(nullptr, true, false, nullptr);
			event_close_w = CreateEventA(nullptr, true, false, nullptr);
			if (!event_close || !event_close_w)
			{
				_vassert(!new_block);
				return;
			}
			
			ResetEvent((HANDLE)event_close);
			ResetEvent((HANDLE)event_close_w);

			//file_dbg_sniffer = fopen("vmm.log", "w+");

			// Вся технология ускорения зависит от новых инструкций, если их нет, незачем
			// это создавать.
			pools = voltek::core::_internal::aligned_talloc<void*>(POOL_MAX, 0x10);
			if (pools)
			{
				pools[POOL_8] = new pool8_t(POOL_SIZE);
				pools[POOL_16] = new pool16_t(POOL_SIZE);
				pools[POOL_32] = new pool32_t(POOL_SIZE);
				pools[POOL_64] = new pool64_t(POOL_SIZE);
			}

			thread = new std::thread([](HANDLE* ev_close, HANDLE* ev_close_w, void** pools, 
				voltek::core::_internal::simple_lock* lock) {
				while (1)
				{
					{
						// Блокируем. Снятие блокировки будет заботить компилятор.
						voltek::core::_internal::simple_scope_lock scope_lock(*lock);

						if (pools[POOL_8]) { ((pool8_t*)pools[POOL_8])->push_free_block_to_cache(); }
						if (pools[POOL_16]) { ((pool16_t*)pools[POOL_16])->push_free_block_to_cache(); }
						if (pools[POOL_32]) { ((pool32_t*)pools[POOL_32])->push_free_block_to_cache(); }
						if (pools[POOL_64]) { ((pool64_t*)pools[POOL_64])->push_free_block_to_cache(); }
						if (pools[POOL_128]) { ((pool128_t*)pools[POOL_128])->push_free_block_to_cache(); }
						if (pools[POOL_256]) { ((pool256_t*)pools[POOL_256])->push_free_block_to_cache(); }
						if (pools[POOL_512]) { ((pool512_t*)pools[POOL_512])->push_free_block_to_cache(); }
						if (pools[POOL_1024]) { ((pool1024_t*)pools[POOL_1024])->push_free_block_to_cache(); }
						if (pools[POOL_4096]) { ((pool4096_t*)pools[POOL_4096])->push_free_block_to_cache(); }
						if (pools[POOL_8192]) { ((pool8192_t*)pools[POOL_8192])->push_free_block_to_cache(); }
						if (pools[POOL_16384]) { ((pool16384_t*)pools[POOL_16384])->push_free_block_to_cache(); }
						if (pools[POOL_32768]) { ((pool32768_t*)pools[POOL_32768])->push_free_block_to_cache(); }
						if (pools[POOL_65536]) { ((pool65536_t*)pools[POOL_65536])->push_free_block_to_cache(); }
						if (pools[POOL_131072]) { ((pool131072_t*)pools[POOL_131072])->push_free_block_to_cache(); }
					}

					if (WaitForSingleObject(*ev_close, 10) == WAIT_OBJECT_0)
					{
						SetEvent(*ev_close_w);
						break;
					}

					Sleep(1);
				}
			}, (HANDLE*)&event_close, (HANDLE*)&event_close_w, pools, &lock);
			_vassert(!thread);
			SetThreadPriority(thread->native_handle(), THREAD_PRIORITY_HIGHEST);
			_vassert(!SetThreadAffinityMask(thread->native_handle(), 1llu << (thread->hardware_concurrency() - 1)));
			thread->detach();
		}

		memory_manager::~memory_manager()
		{
			if (thread)
			{
				SetEvent((HANDLE)event_close);
				WaitForSingleObject((HANDLE)event_close_w, INFINITE);

				if (pools)
				{
					if (pools[POOL_8]) delete ((pool8_t*)pools[POOL_8]);
					if (pools[POOL_16]) delete ((pool16_t*)pools[POOL_16]);
					if (pools[POOL_32]) delete ((pool32_t*)pools[POOL_32]);
					if (pools[POOL_64]) delete ((pool64_t*)pools[POOL_64]);
					if (pools[POOL_128]) delete ((pool128_t*)pools[POOL_128]);
					if (pools[POOL_256]) delete ((pool256_t*)pools[POOL_256]);
					if (pools[POOL_512]) delete ((pool512_t*)pools[POOL_512]);
					if (pools[POOL_1024]) delete ((pool1024_t*)pools[POOL_1024]);
					if (pools[POOL_4096]) delete ((pool4096_t*)pools[POOL_4096]);
					if (pools[POOL_8192]) delete ((pool8192_t*)pools[POOL_8192]);
					if (pools[POOL_16384]) delete ((pool16384_t*)pools[POOL_16384]);
					if (pools[POOL_32768]) delete ((pool32768_t*)pools[POOL_32768]);
					if (pools[POOL_65536]) delete ((pool65536_t*)pools[POOL_65536]);
					if (pools[POOL_131072]) delete ((pool131072_t*)pools[POOL_131072]);

					voltek::core::_internal::aligned_free(pools);
					pools = nullptr;
				}

				delete thread;
				thread = nullptr;
			}
		}

		void* memory_manager::alloc(size_t size)
		{
			//if (ULONG_MAX < size)
			//	return nullptr;

			if (!size)
				return get_ptr_from_block_handle(&zero_size_request_block);

			// Блокируем. Снятие блокировки будет заботить компилятор.
			//voltek::core::_internal::simple_scope_lock scope_lock(lock);

			//_fsniff("The beginning of the allocation of a memory block of %llu sizes", size);

			// Проблемы с пулами? или размер больше фиксируемых блоков?
			// Тогда выделим память простым способом.
			if (!pools || (size > 131072))
			{
			alloc_default_ptr_label:
				block_base* new_block;
				
				if (size >= MAX_BLOCK_SIZE)
					new_block = (block_base*)VirtualAlloc(NULL, size + sizeof(block_base), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
				else
					new_block = (block_base*)voltek::core::_internal::aligned_malloc(size + sizeof(block_base), 0x10);
	
				if (new_block)
				{
					//_fsniff("Default block allocated: %p", new_block);

					create_default_block(new_block, size);
					return get_ptr_from_block_handle(new_block);
				}

				_vassert(!new_block);
				return nullptr;
			}

			void* new_ptr = nullptr;
			// Блокируем. Снятие блокировки будет заботить компилятор.
			voltek::core::_internal::simple_scope_lock scope_lock(lock);
	
			if (size > 65536)
			{
				if (!pools[POOL_131072])
					pools[POOL_131072] = (void*)(new pool131072_t(POOL_SIZE));

				pool131072_t* pool = (pool131072_t*)pools[POOL_131072];
				page131072_t* page = nullptr;
				block131072_t* block = nullptr;
				size_t index_block = 0;

				if (pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, (uint32_t)size, (uint16_t)page->get_user_data(),
						(uint32_t)index_block, (uint16_t)POOL_131072);
					new_ptr = get_ptr_from_block_handle(block);
					//_fsniff("Pool block allocated <131072>: %p %llu", new_ptr, size);
				}
			}
			else if (size > 32768)
			{
				if (!pools[POOL_65536])
					pools[POOL_65536] = (void*)(new pool65536_t(POOL_SIZE));

				pool65536_t* pool = (pool65536_t*)pools[POOL_65536];
				page65536_t* page = nullptr;
				block65536_t* block = nullptr;
				size_t index_block = 0;

				if (pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, (uint32_t)size, (uint16_t)page->get_user_data(),
						(uint32_t)index_block, (uint16_t)POOL_65536);
					new_ptr = get_ptr_from_block_handle(block);
					//_fsniff("Pool block allocated <65536>: %p %llu", new_ptr, size);
				}
			}
			else if (size > 16384)
			{
				if (!pools[POOL_32768])
					pools[POOL_32768] = (void*)(new pool32768_t(POOL_SIZE));

				pool32768_t* pool = (pool32768_t*)pools[POOL_32768];
				page32768_t* page = nullptr;
				block32768_t* block = nullptr;
				size_t index_block = 0;

				if (pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, (uint32_t)size, (uint16_t)page->get_user_data(),
						(uint32_t)index_block, (uint16_t)POOL_32768);
					new_ptr = get_ptr_from_block_handle(block);
					//_fsniff("Pool block allocated <32768>: %p %llu", new_ptr, size);
				}
			}
			else if (size > 8192)
			{
				if (!pools[POOL_16384])
					pools[POOL_16384] = (void*)(new pool16384_t(POOL_SIZE));

				pool16384_t* pool = (pool16384_t*)pools[POOL_16384];
				page16384_t* page = nullptr;
				block16384_t* block = nullptr;
				size_t index_block = 0;

				if (pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, (uint32_t)size, (uint16_t)page->get_user_data(),
						(uint32_t)index_block, (uint16_t)POOL_16384);
					new_ptr = get_ptr_from_block_handle(block);
					//_fsniff("Pool block allocated <16384>: %p %llu", new_ptr, size);
				}
			}
			else if (size > 4096)
			{
				if (!pools[POOL_8192])
					pools[POOL_8192] = (void*)(new pool8192_t(POOL_SIZE));

				pool8192_t* pool = (pool8192_t*)pools[POOL_8192];
				page8192_t* page = nullptr;
				block8192_t* block = nullptr;
				size_t index_block = 0;

				if (pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, (uint32_t)size, (uint16_t)page->get_user_data(),
						(uint32_t)index_block, (uint16_t)POOL_8192);
					new_ptr = get_ptr_from_block_handle(block);
					//_fsniff("Pool block allocated <8192>: %p %llu", new_ptr, size);
				}
			}
			else if (size > 1024)
			{
				if (!pools[POOL_4096])
					pools[POOL_4096] = (void*)(new pool4096_t(POOL_SIZE));

				pool4096_t* pool = (pool4096_t*)pools[POOL_4096];
				page4096_t* page = nullptr;
				block4096_t* block = nullptr;
				size_t index_block = 0;

				if (pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, (uint32_t)size, (uint16_t)page->get_user_data(),
						(uint32_t)index_block, (uint16_t)POOL_4096);
					new_ptr = get_ptr_from_block_handle(block);
					//_fsniff("Pool block allocated <4096>: %p %llu", new_ptr, size);
				}
			}
			else if (size > 512)
			{
				if (!pools[POOL_1024])
					pools[POOL_1024] = (void*)(new pool1024_t(POOL_SIZE));

				pool1024_t* pool = (pool1024_t*)pools[POOL_1024];
				page1024_t* page = nullptr;
				block1024_t* block = nullptr;
				size_t index_block = 0;

				if (pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, (uint32_t)size, (uint16_t)page->get_user_data(),
						(uint32_t)index_block, (uint16_t)POOL_1024);
					new_ptr = get_ptr_from_block_handle(block);
					//_fsniff("Pool block allocated <1024>: %p %llu", new_ptr, size);
				}
			}
			else if (size > 256)
			{
				if (!pools[POOL_512])
					pools[POOL_512] = (void*)(new pool512_t(POOL_SIZE));

				pool512_t* pool = (pool512_t*)pools[POOL_512];
				page512_t* page = nullptr;
				block512_t* block = nullptr;
				size_t index_block = 0;

				if (pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, (uint32_t)size, (uint16_t)page->get_user_data(),
						(uint32_t)index_block, (uint16_t)POOL_512);
					new_ptr = get_ptr_from_block_handle(block);
					//_fsniff("Pool block allocated <512>: %p %llu", new_ptr, size);
				}
			}
			else if (size > 128)
			{
				if (!pools[POOL_256])
					pools[POOL_256] = (void*)(new pool256_t(POOL_SIZE));

				pool256_t* pool = (pool256_t*)pools[POOL_256];
				page256_t* page = nullptr;
				block256_t* block = nullptr;
				size_t index_block = 0;

				if (pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, (uint32_t)size, (uint16_t)page->get_user_data(),
						(uint32_t)index_block, (uint16_t)POOL_256);
					new_ptr = get_ptr_from_block_handle(block);
					//_fsniff("Pool block allocated <256>: %p %llu", new_ptr, size);
				}
			}
			else if (size > 64)
			{
				if (!pools[POOL_128])
					pools[POOL_128] = (void*)(new pool128_t(POOL_SIZE));

				pool128_t* pool = (pool128_t*)pools[POOL_128];
				page128_t* page = nullptr;
				block128_t* block = nullptr;
				size_t index_block = 0;

				if (pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, (uint32_t)size, (uint16_t)page->get_user_data(),
						(uint32_t)index_block, (uint16_t)POOL_128);
					new_ptr = get_ptr_from_block_handle(block);
					//_fsniff("Pool block allocated <128>: %p %llu", new_ptr, size);
				}
			}
			else if (size > 32)
			{
				pool64_t* pool = (pool64_t*)pools[POOL_64];
				page64_t* page = nullptr;
				block64_t* block = nullptr;
				size_t index_block = 0;

				if (pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, (uint32_t)size, (uint16_t)page->get_user_data(),
						(uint32_t)index_block, (uint16_t)POOL_64);
					new_ptr = get_ptr_from_block_handle(block);
					//_fsniff("Pool block allocated <64>: %p %llu", new_ptr, size);
				}
			}
			else if (size > 16)
			{
				pool32_t* pool = (pool32_t*)pools[POOL_32];
				page32_t* page = nullptr;
				block32_t* block = nullptr;
				size_t index_block = 0;

				if (pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, (uint32_t)size, (uint16_t)page->get_user_data(),
						(uint32_t)index_block, (uint16_t)POOL_32);
					new_ptr = get_ptr_from_block_handle(block);
					//_fsniff("Pool block allocated <32>: %p %llu", new_ptr, size);
				}
			}
			else if (size > 8)
			{
				pool16_t* pool = (pool16_t*)pools[POOL_16];
				page16_t* page = nullptr;
				block16_t* block = nullptr;
				size_t index_block = 0;

				if (pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, (uint32_t)size, (uint16_t)page->get_user_data(),
						(uint32_t)index_block, (uint16_t)POOL_16);
					new_ptr = get_ptr_from_block_handle(block);
					//_fsniff("Pool block allocated <16>: %p %llu", new_ptr, size);
				}
			}
			else
			{
				pool8_t* pool = (pool8_t*)pools[POOL_8];
				page8_t* page = nullptr;
				block8_t* block = nullptr;
				size_t index_block = 0;

				if (pool->get_free_block(block, page, index_block))
				{
					create_pool_block(block, (uint32_t)size, (uint16_t)page->get_user_data(),
						(uint32_t)index_block, (uint16_t)POOL_8);
					new_ptr = get_ptr_from_block_handle(block);
					//_fsniff("Pool block allocated <8>: %p %llu", new_ptr, size);
				}
			}

			// Если каким-то чудом память не выделена, то выделим память простым способом.
			if (!new_ptr)
				goto alloc_default_ptr_label;
			
			return new_ptr;
		}

		void* memory_manager::realloc(const void* ptr, size_t size)
		{
			if (!ptr || !is_valid_ptr(ptr) || !is_valid_pointer(ptr) || !size /*|| (ULONG_MAX < size)*/)
				return nullptr;

			void* new_ptr = nullptr;	
			// Блокируем. Снятие блокировки будет заботить компилятор.
			voltek::core::_internal::simple_scope_lock scope_lock(lock);

			// Если память выделена ранее как обычный, то тут выделение новой памяти неизбежно.
			if (is_used_default_ptr(ptr))
			{
			realloc_def_label:
				size_t old_size = msize(ptr);
				new_ptr = alloc(size);
				if (new_ptr && (old_size > 0)) memcpy(new_ptr, ptr, old_size > size ? size : old_size);
				free(ptr);
			}
			else
			{
				new_ptr = const_cast<void*>(ptr);
				block_base* block = get_block_handle_from_ptr(new_ptr);
				uint8_t pool_id = block->pool_id;

				switch (pool_id)
				{
				case POOL_131072:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 131072)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = (uint32_t)size;
				}
				break;
				case POOL_65536:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 65536)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = (uint32_t)size;
				}
				break;
				case POOL_32768:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 32768)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = (uint32_t)size;
				}
				break;
				case POOL_16384:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 16384)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = (uint32_t)size;
				}
				break;
				case POOL_8192:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 8192)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = (uint32_t)size;
				}
				break;
				case POOL_4096:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 4096)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = (uint32_t)size;
				}
				break;
				case POOL_1024:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 1024)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = (uint32_t)size;
				}
				break;
				case POOL_512:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 512)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = (uint32_t)size;
				}
				break;
				case POOL_256:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 256)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = (uint32_t)size;
				}
				break;
				case POOL_128:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 128)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = (uint32_t)size;
				}
				break;
				case POOL_64:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 64)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = (uint32_t)size;
				}
				break;
				case POOL_32:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 32)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = (uint32_t)size;
				}
				break;
				case POOL_16:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 16)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = (uint32_t)size;
				}
				break;
				default:
				{
					// Если требуемая память больше, чем может позволить блок,
					// то выделение новой памяти неизбежно.
					if (size > 8)
						goto realloc_def_label;
					// Новый размер для памяти.
					block->size = (uint32_t)size;
				}
				break;
				}
			}

			return new_ptr;
		}

		bool memory_manager::free(const void* ptr)
		{
			if (!ptr || !is_valid_ptr(ptr) || !is_valid_pointer(ptr))
				return false;

			bool ret = true;
			// Блокируем. Снятие блокировки будет заботить компилятор.
			voltek::core::_internal::simple_scope_lock scope_lock(lock);
			//_fsniff("The beginning of memory release: %p", ptr);

			if (is_used_default_ptr(ptr))
			{
				auto block = get_block_handle_from_ptr(ptr);
				if (block->size > 0)
				{
					if (block->size >= MAX_BLOCK_SIZE)
						VirtualFree((LPVOID)block, (SIZE_T)block->size, MEM_RELEASE);
					else 
						voltek::core::_internal::aligned_free(block);
				}
				//_fsniff("Default memory block released");
			}
			else
			{
				block_base* block = get_block_handle_from_ptr(ptr);
				uint8_t pool_id = block->pool_id;
				uint16_t page_id = block->page_id;
				uint32_t block_id = block->block_id;

				switch (pool_id)
				{
				case POOL_131072:
				{
					pool131072_t* pool = (pool131072_t*)(pools[POOL_131072]);
					ret = pool->release_block((*pool)[page_id], block_id);
					//_fsniff("Pool memory block <131072> released [%s]", (ret ? "SUCCESS" : "FAILED"));
				}
				break;
				case POOL_65536:
				{
					pool65536_t* pool = (pool65536_t*)(pools[POOL_65536]);
					ret = pool->release_block((*pool)[page_id], block_id);
					//_fsniff("Pool memory block <65536> released [%s]", (ret ? "SUCCESS" : "FAILED"));
				}
				break;
				case POOL_32768:
				{
					pool32768_t* pool = (pool32768_t*)(pools[POOL_32768]);
					ret = pool->release_block((*pool)[page_id], block_id);
					//_fsniff("Pool memory block <32768> released [%s]", (ret ? "SUCCESS" : "FAILED"));
				}
				break;
				case POOL_16384:
				{
					pool16384_t* pool = (pool16384_t*)(pools[POOL_16384]);
					ret = pool->release_block((*pool)[page_id], block_id);
					//_fsniff("Pool memory block <16384> released [%s]", (ret ? "SUCCESS" : "FAILED"));
				}
				break;
				case POOL_8192:
				{
					pool8192_t* pool = (pool8192_t*)(pools[POOL_8192]);
					ret = pool->release_block((*pool)[page_id], block_id);
					//_fsniff("Pool memory block <8192> released [%s]", (ret ? "SUCCESS" : "FAILED"));
				}
				break;
				case POOL_4096:
				{
					pool4096_t* pool = (pool4096_t*)(pools[POOL_4096]);
					ret = pool->release_block((*pool)[page_id], block_id);
					//_fsniff("Pool memory block <4096> released [%s]", (ret ? "SUCCESS" : "FAILED"));
				}
				break;
				case POOL_1024:
				{
					pool1024_t* pool = (pool1024_t*)(pools[POOL_1024]);
					ret = pool->release_block((*pool)[page_id], block_id);
					//_fsniff("Pool memory block <1024> released [%s]", (ret ? "SUCCESS" : "FAILED"));
				}
				break;
				case POOL_512:
				{
					pool512_t* pool = (pool512_t*)(pools[POOL_512]);
					ret = pool->release_block((*pool)[page_id], block_id);
					//_fsniff("Pool memory block <512> released [%s]", (ret ? "SUCCESS" : "FAILED"));
				}
				break;
				case POOL_256:
				{
					pool256_t* pool = (pool256_t*)(pools[POOL_256]);
					ret = pool->release_block((*pool)[page_id], block_id);
					//_fsniff("Pool memory block <256> released [%s]", (ret ? "SUCCESS" : "FAILED"));
				}
				break;
				case POOL_128:
				{
					pool128_t* pool = (pool128_t*)(pools[POOL_128]);
					ret = pool->release_block((*pool)[page_id], block_id);
					//_fsniff("Pool memory block <128> released [%s]", (ret ? "SUCCESS" : "FAILED"));
				}
				break;
				case POOL_64:
				{
					pool64_t* pool = (pool64_t*)(pools[POOL_64]);
					ret = pool->release_block((*pool)[page_id], block_id);
					//_fsniff("Pool memory block <64> released [%s]", (ret ? "SUCCESS" : "FAILED"));
				}
				break;
				case POOL_32:
				{
					pool32_t* pool = (pool32_t*)(pools[POOL_32]);
					ret = pool->release_block((*pool)[page_id], block_id);
					//_fsniff("Pool memory block <32> released [%s]", (ret ? "SUCCESS" : "FAILED"));
				}
				break;
				case POOL_16:
				{
					pool16_t* pool = (pool16_t*)(pools[POOL_16]);
					ret = pool->release_block((*pool)[page_id], block_id);
					//_fsniff("Pool memory block <16> released [%s]", (ret ? "SUCCESS" : "FAILED"));
				}
				break;
				default:
				{
					pool8_t* pool = (pool8_t*)(pools[POOL_8]);
					ret = pool->release_block((*pool)[page_id], block_id);
					//_fsniff("Pool memory block <8> released [%s]", (ret ? "SUCCESS" : "FAILED"));
				}
				break;
				}
			}

			return ret;
		}

		size_t memory_manager::msize(const void* ptr) const
		{
			if (!ptr || !is_valid_pointer(ptr)) return 0;
			// Блокируем. Снятие блокировки будет заботить компилятор.
			voltek::core::_internal::simple_scope_lock scope_lock(lock);
			// Получение размера.
			return (size_t)get_size_from_ptr(ptr);
		}

		void memory_manager::dump_map(size_t pool_id, const char* filename) const
		{
#ifndef VMMDLL_EXPORTS
			if ((POOL_MAX >= pool_id) || !filename)
				return;

			// Блокируем. Снятие блокировки будет заботить компилятор.
			voltek::core::_internal::simple_scope_lock scope_lock(lock);

			switch (pool_id)
			{
			case POOL_131072:
			{
				pool131072_t* pool = (pool131072_t*)(pools[POOL_131072]);
				pool->dump_map(filename);
			}
			break;
			case POOL_65536:
			{
				pool65536_t* pool = (pool65536_t*)(pools[POOL_65536]);
				pool->dump_map(filename);
			}
			break;
			case POOL_32768:
			{
				pool32768_t* pool = (pool32768_t*)(pools[POOL_32768]);
				pool->dump_map(filename);
			}
			break;
			case POOL_16384:
			{
				pool16384_t* pool = (pool16384_t*)(pools[POOL_16384]);
				pool->dump_map(filename);
			}
			break;
			case POOL_8192:
			{
				pool8192_t* pool = (pool8192_t*)(pools[POOL_8192]);
				pool->dump_map(filename);
			}
			break;
			case POOL_4096:
			{
				pool4096_t* pool = (pool4096_t*)(pools[POOL_4096]);
				pool->dump_map(filename);
			}
			break;
			case POOL_1024:
			{
				pool1024_t* pool = (pool1024_t*)(pools[POOL_1024]);
				pool->dump_map(filename);
			}
			break;
			case POOL_512:
			{
				pool512_t* pool = (pool512_t*)(pools[POOL_512]);
				pool->dump_map(filename);
			}
			break;
			case POOL_256:
			{
				pool256_t* pool = (pool256_t*)(pools[POOL_256]);
				pool->dump_map(filename);
			}
			break;
			case POOL_128:
			{
				pool128_t* pool = (pool128_t*)(pools[POOL_128]);
				pool->dump_map(filename);
			}
			break;
			case POOL_64:
			{
				pool64_t* pool = (pool64_t*)(pools[POOL_64]);
				pool->dump_map(filename);
			}
			break;
			case POOL_32:
			{
				pool32_t* pool = (pool32_t*)(pools[POOL_32]);
				pool->dump_map(filename);
			}
			break;
			case POOL_16:
			{
				pool16_t* pool = (pool16_t*)(pools[POOL_16]);
				pool->dump_map(filename);
			}
			break;
			default:
			{
				pool8_t* pool = (pool8_t*)(pools[POOL_8]);
				pool->dump_map(filename);
			}
			break;
			}
#endif // !VMMDLL_EXPORTS
		}

		void memory_manager::dump(size_t pool_id, const char* filename) const
		{
#ifndef VMMDLL_EXPORTS
			if ((POOL_MAX >= pool_id) || !filename)
				return;

			// Блокируем. Снятие блокировки будет заботить компилятор.
			voltek::core::_internal::simple_scope_lock scope_lock(lock);

			switch (pool_id)
			{
			case POOL_131072:
			{
				pool131072_t* pool = (pool131072_t*)(pools[POOL_131072]);
				pool->dump(filename);
			}
			break;
			case POOL_65536:
			{
				pool65536_t* pool = (pool65536_t*)(pools[POOL_65536]);
				pool->dump(filename);
			}
			break;
			case POOL_32768:
			{
				pool32768_t* pool = (pool32768_t*)(pools[POOL_32768]);
				pool->dump(filename);
			}
			break;
			case POOL_16384:
			{
				pool16384_t* pool = (pool16384_t*)(pools[POOL_16384]);
				pool->dump(filename);
			}
			break;
			case POOL_8192:
			{
				pool8192_t* pool = (pool8192_t*)(pools[POOL_8192]);
				pool->dump(filename);
			}
			break;
			case POOL_4096:
			{
				pool4096_t* pool = (pool4096_t*)(pools[POOL_4096]);
				pool->dump(filename);
			}
			break;
			case POOL_1024:
			{
				pool1024_t* pool = (pool1024_t*)(pools[POOL_1024]);
				pool->dump(filename);
			}
			break;
			case POOL_512:
			{
				pool512_t* pool = (pool512_t*)(pools[POOL_512]);
				pool->dump(filename);
			}
			break;
			case POOL_256:
			{
				pool256_t* pool = (pool256_t*)(pools[POOL_256]);
				pool->dump(filename);
			}
			break;
			case POOL_128:
			{
				pool128_t* pool = (pool128_t*)(pools[POOL_128]);
				pool->dump(filename);
			}
			break;
			case POOL_64:
			{
				pool64_t* pool = (pool64_t*)(pools[POOL_64]);
				pool->dump(filename);
			}
			break;
			case POOL_32:
			{
				pool32_t* pool = (pool32_t*)(pools[POOL_32]);
				pool->dump(filename);
			}
			break;
			case POOL_16:
			{
				pool16_t* pool = (pool16_t*)(pools[POOL_16]);
				pool->dump(filename);
			}
			break;
			default:
			{
				pool8_t* pool = (pool8_t*)(pools[POOL_8]);
				pool->dump(filename);
			}
			break;
			}
#endif // !VMMDLL_EXPORTS
		}

		memory_manager::memory_manager(const memory_manager& ob) : pools(nullptr)
		{
			memset(&zero_size_request_block, 0, sizeof(zero_size_request_block));
		}

		memory_manager& memory_manager::operator=(const memory_manager& ob)
		{
			return *this;
		}
	}
}

#pragma warning(default : 26819)