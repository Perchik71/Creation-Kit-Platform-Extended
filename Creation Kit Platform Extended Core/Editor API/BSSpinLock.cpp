// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/BSSpinLock.cpp

#include "BSSpinLock.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		BSSpinLock::~BSSpinLock()
		{
			Assert(m_LockCount == 0);
		}

		void BSSpinLock::Acquire(int InitialAttempts)
		{
			// Check for recursive locking
			if (ThreadOwnsLock())
			{
				InterlockedIncrement(&m_LockCount);
				return;
			}

			// First test (no waits/pauses, fast path)
			if (InterlockedCompareExchange(&m_LockCount, 1, 0) != 0)
			{
				uint32_t counter = 0;
				bool locked = false;

				// Slow path #1 (PAUSE instruction)
				do
				{
					counter++;
					_mm_pause();

					locked = InterlockedCompareExchange(&m_LockCount, 1, 0) == 0;
				} while (!locked && counter < (uint32_t)InitialAttempts);

				// Slower path #2 (Sleep(X))
				for (counter = 0; !locked;)
				{
					if (counter < SLOW_PATH_BACKOFF_COUNT)
					{
						Sleep(0);
						counter++;
					}
					else
					{
						Sleep(1);
					}

					locked = InterlockedCompareExchange(&m_LockCount, 1, 0) == 0;
				}

				_mm_lfence();
			}

			m_OwningThread = GetCurrentThreadId();
			_mm_sfence();
		}

		void BSSpinLock::Release()
		{
			AssertMsg(IsLocked(), "Invalid lock count");
			AssertMsg(ThreadOwnsLock(), "Thread does not own spinlock");

			// In the public build they ignore threading problems
			if (!ThreadOwnsLock())
				return;

			if (m_LockCount == 1)
			{
				m_OwningThread = 0;
				_mm_mfence();

				uint32_t oldCount = InterlockedCompareExchange(&m_LockCount, 0, 1);
				AssertMsg(oldCount == 1, "The spinlock wasn't correctly released");
			}
			else
			{
				uint32_t oldCount = InterlockedDecrement(&m_LockCount);
				AssertMsg(oldCount < 0xFFFFFFFF && oldCount, "Invalid lock count");
			}
		}

		bool BSSpinLock::IsLocked() const
		{
			return m_LockCount != 0;
		}

		bool BSSpinLock::ThreadOwnsLock() const
		{
			_mm_lfence();
			return m_OwningThread == GetCurrentThreadId();
		}
	}
}