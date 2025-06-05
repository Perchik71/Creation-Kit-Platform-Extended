// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <stddef.h>

namespace voltek
{
	namespace core
	{
		namespace _internal
		{
			//class St

			void* aligned_malloc(size_t size, size_t alignment);
			void* aligned_calloc(size_t count, size_t size, size_t alignment);
			void* aligned_realloc(void* ptr, size_t size, size_t alignment);
			void* aligned_recalloc(void* ptr, size_t count, size_t size, size_t alignment);
			void aligned_free(void* ptr);
			size_t aligned_msize(const void* ptr);

			template<typename _type> inline _type* aligned_talloc(size_t count, size_t alignment)
			{
				return (_type*)aligned_calloc(count, sizeof(_type), alignment);
			}

			template<typename _type> inline _type* aligned_retalloc(_type* ptr, size_t count, size_t alignment)
			{
				return (_type*)aligned_recalloc(ptr, count, sizeof(_type), alignment);
			}

			template<typename _type> inline size_t aligned_tcount(_type* ptr)
			{
				return aligned_msize(ptr) / sizeof(_type);
			}
		}
	}
}