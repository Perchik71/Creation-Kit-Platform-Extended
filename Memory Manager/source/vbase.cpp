// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include "vbase.h"
#include "valloc.h"
#include "vassert.h"
#include <intrin.h>

namespace voltek
{
	namespace core
	{
		bool initialize_success = false;
		// Поддержка процессором AVX2
		bool avx2_supported = false;
		// Поддержка процессором SSE4.1
		bool sse41_supported = false;

		void initialize()
		{
			if (initialize_success) return;

			int info[4];
			__cpuid(info, 7);

			initialize_success = true;
			avx2_supported = (info[1] & (1 << 5)) != 0;

			__cpuid(info, 1);
			sse41_supported = (info[2] & (1 << 19)) != 0;
		}

		void* base::operator new (size_t size)
		{
			_vassert(size > 0);
			void* ptr = _internal::aligned_malloc(size, 0x10);
			_vassert(ptr != nullptr);
			return ptr;
		}

		void base::operator delete (void* ptr)
		{
			_vassert(ptr != nullptr);
			_internal::aligned_free(ptr);
		}
	}
}