// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.ErrorHandler.h>
#include <CKPE.Common.MemoryManager.h>
#include <Voltek.MemoryManager.h>
#include <memory.h>
#include <format>

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

		void* MemoryManager::Alloc(std::size_t size, bool zeroed) noexcept(true)
		{
			if (!size)
				// Creation Kit много раз требует памяти в 0 байт, данная функция вернёт ему память с
				// постоянным адресом, не выделяя его в пулах.
				return voltek::scalable_alloc(0);

			void* ptr = voltek::scalable_alloc(size);
			if (ptr && zeroed) memset(ptr, 0, size);

			if (!ptr && size <= (128llu * 1024 * 1024))
				ErrorHandler::Trigger(
					std::format("A memory allocation failed. This is due to memory leaks in the Creation Kit or not"
						" having enough free RAM.\n\nRequested chunk size: {} bytes.", size));

			return ptr;
		}

		void* MemoryManager::Alloc(std::size_t size_elem, std::size_t count, bool zeroed) noexcept(true)
		{
			return Alloc(size_elem * count, zeroed);
		}

		void* MemoryManager::ReAlloc(void* mem, std::size_t size, bool zeroed) noexcept(true)
		{
			if (!mem)
				return Alloc(size, zeroed);

			auto ptr = Alloc(size, zeroed);
			if (ptr)
			{
				auto ss = Size(mem);
				memcpy(ptr, mem, ss < size ? ss : size);
			}

			Free(mem);
			return ptr;
		}

		void* MemoryManager::ReAlloc(void* mem, std::size_t size_elem, std::size_t count, bool zeroed) noexcept(true)
		{
			return ReAlloc(mem, size_elem * count, zeroed);
		}

		std::size_t MemoryManager::Size(void* mem) noexcept(true)
		{
			return voltek::scalable_msize(mem);
		}

		void MemoryManager::Free(void* mem) noexcept(true)
		{
			voltek::scalable_free(mem);
		}
	}

	CKPE_COMMON_API void* malloc(std::size_t s) noexcept(true)
	{
		return Common::smemmgr.Alloc(s);
	}

	CKPE_COMMON_API void* realloc(void* m, std::size_t s) noexcept(true)
	{
		return Common::smemmgr.ReAlloc(m, s);
	}

	CKPE_COMMON_API void* calloc(std::size_t s, std::size_t c) noexcept(true)
	{
		return Common::smemmgr.Alloc(s, c);
	}

	CKPE_COMMON_API void* recalloc(void* m, std::size_t s, std::size_t c) noexcept(true)
	{
		return Common::smemmgr.ReAlloc(m, s, c);
	}

	CKPE_COMMON_API std::size_t msize(void* m) noexcept(true)
	{
		return Common::smemmgr.Size(m);
	}

	CKPE_COMMON_API void free(void* m) noexcept(true)
	{
		return Common::smemmgr.Free(m);
	}

	CKPE_COMMON_API void* aligned_malloc(std::size_t s, std::size_t a) noexcept(true)
	{
		if (a > 16) return _aligned_malloc(s, a);
		return Common::smemmgr.Alloc(s);
	}

	CKPE_COMMON_API void* aligned_realloc(void* m, std::size_t s, std::size_t a) noexcept(true)
	{
		if (a > 16) return aligned_realloc(m, s, a);
		return Common::smemmgr.ReAlloc(m, s);
	}

	CKPE_COMMON_API void* aligned_recalloc(void* m, std::size_t s, std::size_t c, std::size_t a) noexcept(true)
	{
		if (a > 16) return aligned_recalloc(m, s, c, a);
		return Common::smemmgr.ReAlloc(m, s, c);
	}

	CKPE_COMMON_API std::size_t aligned_msize(void* m, std::size_t a, std::size_t o) noexcept(true)
	{
		if ((a > 16) || (o != 0)) return aligned_msize(m, a, o);
		return Common::smemmgr.Size(m);
	}

	CKPE_COMMON_API void aligned_free(void* m) noexcept(true)
	{
		if (!Common::smemmgr.Size(m))
			aligned_free(m);
		else
			Common::smemmgr.Free(m);
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