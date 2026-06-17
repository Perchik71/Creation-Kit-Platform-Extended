// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.ErrorHandler.h>
#include <CKPE.Asserts.h>
#include <CKPE.Common.MemoryManager.h>
#include <Voltek.MemoryManager.h>
#include <Windows.h>
#include <memory>
#include <format>
#include <algorithm>

namespace CKPE
{
	namespace Common
	{
		static MemoryManager smemmgr;

		MemoryManager::MemoryManager() noexcept(true)
		{
			// Инициализация библиотеки vmm
			voltek::scalable_memory_manager_initialize();
		}
	
		void* MemoryManager::MemAlloc(std::size_t size, std::size_t alignment, bool aligned, bool zeroed) noexcept(true)
		{
			// Если не задано, то будет 4
			if (!aligned)
				alignment = 4;

			if (!size)
				// Creation Kit много раз требует памяти в 0 байт, данная функция вернёт ему память с
				// постоянным адресом, не выделяя его в пулах.
				return voltek::scalable_alloc(0);

			CKPE_ASSERT_MSG_FMT(alignment != 0 && alignment % 2 == 0, "Alignment is fucked: %llu", alignment);

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
			{
				// Use to convert bytes to GB
				constexpr static auto AD_DIV = 1024 * 1024 * 1024;

				static MEMORYSTATUSEX statex = { 0 };
				statex.dwLength = sizeof(MEMORYSTATUSEX);
				if (!GlobalMemoryStatusEx(&statex))
					return ptr;

				//  https://learn.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-globalmemorystatusex
				//  Sample output:
				//  OUT OF MEMORY. A memory allocation failed.
				//  
				//  Requested chunk size: 123456 bytes.
				//  There is     51 percent of memory in use.
				//  There are 20.29 total GB of physical memory.
				//  There are  9.87 free  GB of physical memory.
				//  There are 38.84 total GB of paging file.
				//  There are 27.99 free  GB of paging file.
				CKPE_ASSERT_MSG_FMT(ptr,
					"OUT OF MEMORY. A memory allocation failed.\n\n"
					"Requested chunk size: %llu bytes.\n"
					"There is %ld percent of memory in use.\n"
					"There are %.2f total GB of physical memory.\n"
					"There are %.2f free GB of physical memory.\n"
					"There are %.2f total GB of paging file.\n"
					"There are %.2f free GB of paging file.\n",
					size, statex.dwMemoryLoad,
					(long double)statex.ullTotalPhys / AD_DIV, (long double)statex.ullAvailPhys / AD_DIV,
					(long double)statex.ullTotalPageFile / AD_DIV, (long double)statex.ullAvailPageFile / AD_DIV);
			}

			return ptr;
		}

		std::size_t MemoryManager::MemSize(void* mem) noexcept(true)
		{
			return voltek::scalable_msize(mem);
		}

		void MemoryManager::MemFree(void* mem) noexcept(true)
		{
			voltek::scalable_free(mem);
		}

		MemoryManager* MemoryManager::GetSingleton() noexcept(true)
		{
			return &smemmgr;
		}
	}

	CKPE_COMMON_API void* malloc(std::size_t s) noexcept(true)
	{
		return Common::smemmgr.MemAlloc(s, 0, false);
	}

	CKPE_COMMON_API void* realloc(void* m, std::size_t s) noexcept(true)
	{
		void* newMemory = nullptr;

		if (s > 0)
		{
			// Recalloc behaves like calloc if there's no existing allocation. Realloc doesn't. Zero it either way.
			newMemory = Common::smemmgr.MemAlloc(s, 0, false);
			if (m) memcpy(newMemory, m, std::min(s, voltek::scalable_msize(m)));
		}

		Common::smemmgr.MemFree(m);
		return newMemory;
	}

	CKPE_COMMON_API void* calloc(std::size_t s, std::size_t c) noexcept(true)
	{
		return Common::smemmgr.MemAlloc(c * s, 0, false);
	}

	CKPE_COMMON_API void* recalloc(void* m, std::size_t s, std::size_t c) noexcept(true)
	{
		return realloc(m, c * s);
	}

	CKPE_COMMON_API std::size_t msize(void* m) noexcept(true)
	{
		return Common::smemmgr.MemSize(m);
	}

	CKPE_COMMON_API void free(void* m) noexcept(true)
	{
		Common::smemmgr.MemFree(m);
	}

	CKPE_COMMON_API void* aligned_malloc(std::size_t s, std::size_t a) noexcept(true)
	{
		return Common::smemmgr.MemAlloc(s, a, true);
	}

	CKPE_COMMON_API void aligned_free(void* m) noexcept(true)
	{
		Common::smemmgr.MemFree(m);
	}

	CKPE_COMMON_API char* strdup(const char* s) noexcept(true)
	{
		if (!s) return nullptr;
		auto _size = strlen(s);
		if (_size)
		{
			auto _nstr = reinterpret_cast<char*>(malloc(_size + 1));
			if (_nstr)
			{
				memcpy(_nstr, s, _size);
				_nstr[_size] = '\0';
			}
			return _nstr;
		}
		return nullptr;
	}

	CKPE_COMMON_API wchar_t* wcsdup(const wchar_t* s) noexcept(true)
	{
		if (!s) return nullptr;
		auto _size = wcslen(s);
		if (_size)
		{
			auto _nstr = reinterpret_cast<wchar_t*>(malloc((_size + 1) * sizeof(wchar_t)));
			if (_nstr)
			{
				wmemcpy(_nstr, s, _size);
				_nstr[_size] = L'\0';
			}
			return _nstr;
		}
		return nullptr;
	}
}