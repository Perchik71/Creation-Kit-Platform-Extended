// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

namespace voltek
{
	namespace core
	{
		// Поддержка процессором AVX2
		extern bool avx2_supported;
		// Поддержка процессором SSE4.1 
		extern bool sse41_supported;
		// Инициализация внутренних систем
		void initialize();

		// Общий класс для всех внутренних классов,
		// просто для перекрытия стандартного new и delete.
		class base
		{
		public:
			void* operator new (size_t size);
			void operator delete (void* pointer);
		};
	}
}