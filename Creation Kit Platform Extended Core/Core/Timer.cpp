// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Engine.h"
#include "Timer.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		Timer::Timer()
		{
			LARGE_INTEGER Freq;
			QueryPerformanceFrequency(&Freq);
			perfFreq = (double)Freq.QuadPart;
			perfCounter.QuadPart = 0;
		}

		void Timer::Start() noexcept(true)
		{
			QueryPerformanceCounter(&perfCounter);
		}

		void Timer::Reset() noexcept(true)
		{
			perfCounter.QuadPart = 0;
		}

		double Timer::Get() const noexcept(true)
		{
			LARGE_INTEGER Counter;
			QueryPerformanceCounter(&Counter);
			return (Counter.QuadPart - perfCounter.QuadPart) / perfFreq;
		}
	}
}