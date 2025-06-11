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
		class CKPE_COMMON_API SafeExit
		{
			SafeExit(const SafeExit&) = delete;
			SafeExit& operator=(const SafeExit&) = delete;
		public:
			constexpr SafeExit() noexcept(true) = default;

			static void Quit() noexcept(true);
			static void QuitWithResult(std::int32_t err = 0) noexcept(true);
		public:
			struct CKPE_COMMON_API Hook
			{
				static void HKPostQuitMessage(int nExitCode) noexcept(true);
	
				static void Initialize() noexcept(true);
			};
		};
	}
}