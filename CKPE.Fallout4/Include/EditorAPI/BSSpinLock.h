// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/BSSpinLock.h

#pragma once

#include <cstdint>

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			class BSSpinLock
			{
			private:
				const static std::uint32_t SLOW_PATH_BACKOFF_COUNT = 10000;

				std::uint32_t m_OwningThread = 0;
				volatile std::uint32_t m_LockCount = 0;
			public:
				BSSpinLock() = default;
				~BSSpinLock() noexcept(true);

				void Acquire(int InitialAttempts = 0) noexcept(true);
				void Release() noexcept(true);

				bool IsLocked() const noexcept(true);
				bool ThreadOwnsLock() const noexcept(true);
			};
		}
	}
}