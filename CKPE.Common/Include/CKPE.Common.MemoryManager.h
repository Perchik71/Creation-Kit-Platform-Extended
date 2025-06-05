// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <CKPE.Common.Common.h>

namespace CKPE
{
	namespace Common
	{
		class CKPE_COMMON_API MemoryManager
		{
			MemoryManager(const MemoryManager&) = delete;
			MemoryManager& operator=(const MemoryManager&) = delete;
		public:
			MemoryManager() noexcept(true);

			[[nodiscard]] static void* Alloc(std::size_t size, bool zeroed = false) noexcept(true);
			[[nodiscard]] static void* Alloc(std::size_t size_elem, std::size_t count, bool zeroed = true) noexcept(true);

			template <typename _Ty>
			[[nodiscard]] static _Ty* Alloc(std::size_t count, bool zeroed = true) noexcept(true)
			{ return (_Ty*)Alloc(sizeof(_Ty), count, zeroed); }

			[[nodiscard]] static void* ReAlloc(void* mem, std::size_t size, bool zeroed = false) noexcept(true);
			[[nodiscard]] static void* ReAlloc(void* mem, std::size_t size_elem, std::size_t count, bool zeroed = true) noexcept(true);

			template <typename _Ty>
			[[nodiscard]] static _Ty* ReAlloc(_Ty* mem, std::size_t count, bool zeroed = true) noexcept(true)
			{
				return (_Ty*)ReAlloc(mem, sizeof(_Ty), count, zeroed);
			}

			[[nodiscard]] static std::size_t Size(void* mem) noexcept(true);

			static void Free(void* mem) noexcept(true);
		};
	}

	CKPE_COMMON_API void* malloc(std::size_t s) noexcept(true);
	CKPE_COMMON_API void* realloc(void* m, std::size_t s) noexcept(true);
	CKPE_COMMON_API void* calloc(std::size_t s, std::size_t c) noexcept(true);
	CKPE_COMMON_API void* recalloc(void* m, std::size_t s, std::size_t c) noexcept(true);
	CKPE_COMMON_API std::size_t msize(void* m) noexcept(true);
	CKPE_COMMON_API void free(void* m) noexcept(true);

	CKPE_COMMON_API void* aligned_malloc(std::size_t s, std::size_t a) noexcept(true);
	CKPE_COMMON_API void* aligned_realloc(void* m, std::size_t s, std::size_t a) noexcept(true);
	CKPE_COMMON_API void* aligned_recalloc(void* m, std::size_t s, std::size_t c, std::size_t a) noexcept(true);
	CKPE_COMMON_API std::size_t aligned_msize(void* m, std::size_t a, std::size_t o) noexcept(true);
	CKPE_COMMON_API void aligned_free(void* m) noexcept(true);

	CKPE_COMMON_API char* strdup(const char* s) noexcept(true);
	CKPE_COMMON_API wchar_t* wcsdup(const wchar_t* s) noexcept(true);
}