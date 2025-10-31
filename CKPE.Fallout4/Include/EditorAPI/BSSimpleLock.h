// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class SimpleLock
			{
				enum
				{
					kFastSpinThreshold = 10000
				};

				volatile std::int32_t threadID;	// 00
				std::uint32_t lockCount;		// 04

			public:
				SimpleLock() : threadID(0), lockCount(0) {}

				void Lock() noexcept(true);
				void Release() noexcept(true);
			};
			static_assert(sizeof(SimpleLock) == 0x8);
		}
	}
}