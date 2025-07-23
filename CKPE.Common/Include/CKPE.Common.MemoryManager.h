// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <memory>
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

			[[nodiscard]] virtual void* MemAlloc(size_t size, size_t alignment = 0, bool aligned = false, 
				bool zeroed = true) noexcept(true);
			virtual void MemFree(void* block) noexcept(true);
			[[nodiscard]] virtual size_t MemSize(void* block) noexcept(true);

			[[nodiscard]] static MemoryManager* GetSingleton() noexcept(true);
		};
	}

	CKPE_COMMON_API void* malloc(std::size_t s) noexcept(true);
	CKPE_COMMON_API void* realloc(void* m, std::size_t s) noexcept(true);
	CKPE_COMMON_API void* calloc(std::size_t s, std::size_t c) noexcept(true);
	CKPE_COMMON_API void* recalloc(void* m, std::size_t s, std::size_t c) noexcept(true);
	CKPE_COMMON_API std::size_t msize(void* m) noexcept(true);
	CKPE_COMMON_API void free(void* m) noexcept(true);

	CKPE_COMMON_API void* aligned_malloc(std::size_t s, std::size_t a) noexcept(true);
	CKPE_COMMON_API void aligned_free(void* m) noexcept(true);

	CKPE_COMMON_API char* strdup(const char* s) noexcept(true);
	CKPE_COMMON_API wchar_t* wcsdup(const wchar_t* s) noexcept(true);
}