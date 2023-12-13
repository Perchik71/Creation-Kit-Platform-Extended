// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/BSSpinLock.h

#pragma once

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		class BSSpinLock
		{
		private:
			const static uint32_t SLOW_PATH_BACKOFF_COUNT = 10000;

			uint32_t m_OwningThread = 0;
			volatile uint32_t m_LockCount = 0;

		public:
			BSSpinLock() = default;
			~BSSpinLock();

			void Acquire(int InitialAttempts = 0);
			void Release();

			bool IsLocked() const;
			bool ThreadOwnsLock() const;
		};
	}
}