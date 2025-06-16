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
		class CKPE_COMMON_API ClassicTheme
		{
			ClassicTheme(const ClassicTheme&) = delete;
			ClassicTheme& operator=(const ClassicTheme&) = delete;
		public:
			constexpr ClassicTheme() noexcept(true) = default;

			static void InitializeCurrentThread() noexcept(true);
			static void InitializeThread(uint32_t u32ThreadId) noexcept(true);

		public:
			struct CKPE_COMMON_API Hook
			{
				static void Initialize() noexcept(true);
			};
		};
	}
}