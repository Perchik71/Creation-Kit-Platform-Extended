// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/BSSpinLock.cpp

#include <windows.h>
#include <CKPE.Asserts.h>
#include <EditorAPI/BSSpinLock.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			BSSpinLock::~BSSpinLock() noexcept(true)
			{
				CKPE_ASSERT(m_LockCount == 0);
			}

			void BSSpinLock::Acquire(int InitialAttempts) noexcept(true)
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
					std::uint32_t counter = 0;
					bool locked = false;

					// Slow path #1 (PAUSE instruction)
					do
					{
						counter++;
						_mm_pause();

						locked = InterlockedCompareExchange(&m_LockCount, 1, 0) == 0;
					} while (!locked && counter < (std::uint32_t)InitialAttempts);

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

			void BSSpinLock::Release() noexcept(true)
			{
				CKPE_ASSERT_MSG(IsLocked(), "Invalid lock count");
				CKPE_ASSERT_MSG(ThreadOwnsLock(), "Thread does not own spinlock");

				// In the public build they ignore threading problems
				if (!ThreadOwnsLock())
					return;

				if (m_LockCount == 1)
				{
					m_OwningThread = 0;
					_mm_mfence();

					std::uint32_t oldCount = InterlockedCompareExchange(&m_LockCount, 0, 1);
					CKPE_ASSERT_MSG(oldCount == 1, "The spinlock wasn't correctly released");
				}
				else
				{
					std::uint32_t oldCount = InterlockedDecrement(&m_LockCount);
					CKPE_ASSERT_MSG(oldCount < 0xFFFFFFFF && oldCount, "Invalid lock count");
				}
			}

			bool BSSpinLock::IsLocked() const noexcept(true)
			{
				return m_LockCount != 0;
			}

			bool BSSpinLock::ThreadOwnsLock() const noexcept(true)
			{
				_mm_lfence();
				return m_OwningThread == GetCurrentThreadId();
			}
		}
	}
}