// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		class Timer
		{
			LARGE_INTEGER perfCounter;
			double perfFreq;
		public:
			Timer();
			virtual ~Timer() = default;

			virtual void Start() noexcept(true);
			virtual void Reset() noexcept(true);
			virtual double Get() const noexcept(true);
		private:
			Timer(const Timer&) = delete;
			Timer& operator=(const Timer&) = delete;
		};
	}
}