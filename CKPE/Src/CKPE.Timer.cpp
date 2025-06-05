// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Timer.h>

namespace CKPE
{
	Timer::Timer() noexcept(true)
	{
		LARGE_INTEGER Freq;
		QueryPerformanceFrequency(&Freq);
		perfFreq = (double)Freq.QuadPart;
	}

	void Timer::Start() noexcept(true)
	{
		QueryPerformanceCounter((PLARGE_INTEGER)&perfCounter);
	}

	void Timer::Reset() noexcept(true)
	{
		perfCounter = 0;
	}

	double Timer::Get() const noexcept(true)
	{
		LARGE_INTEGER Counter;
		QueryPerformanceCounter(&Counter);
		return (Counter.QuadPart - perfCounter) / perfFreq;
	}
}