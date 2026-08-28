// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include "..\iw\iw.h"
#include "vbase.h"
#include "valloc.h"
#include "vassert.h"
#include <intrin.h>
#include <string>

namespace voltek
{
	namespace core
	{
		bool initialize_success = false;
		// Поддержка процессором AVX2
		bool avx2_supported = false;
		// Поддержка процессором SSE4.1
		bool sse41_supported = false;
		// Поддержка Hyper
		bool hyper_threads = false;
		// Кол-во логический ядер или просто ядер, если нет Hyper
		unsigned char logical_cores = 0;

		void initialize()
		{
			if (initialize_success) return;

			initialize_success = true;

			auto info = iw::cpu::cpu_info();
			sse41_supported = iw::cpu::is_support_SSE41(&info);
			avx2_supported = iw::cpu::is_support_AVX2(&info);
			hyper_threads = iw::cpu::is_support_hyper(&info);
			logical_cores = iw::cpu::number_of_procs(&info);

			if (avx2_supported)
			{
				if (!hyper_threads) 
					// Отключение использования AVX2 на процессорах не поддерживающих Hyper 
					avx2_supported = false;
				else
				{
					// Отключение использования AVX2 на процессорах Intel
					if (iw::cpu::is_intel(&info))
						avx2_supported = false;
				}
			}
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