//////////////////////////////////////////
/*
* Fallout 4 Script Extender (F4SE)
* by Ian Patterson, Stephen Abel, and Brendan Borthwick (ianpatt, behippo, and purplelunchbox)
*
* Contact the F4SE Team
*
* Entire Team
* Send email to team [at] f4se [dot] silverlock [dot] org
*
* Ian (ianpatt)
* Send email to ianpatt+f4se [at] gmail [dot] com
*
* Stephen (behippo)
* Send email to gamer [at] silverlock [dot] org
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

//////////////////////////////////////////
/*
* Copyright (c) 2022 Perchik71 <email:perchik71@outlook.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this
* software and associated documentation files (the "Software"), to deal in the Software
* without restriction, including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

#include "NiClassesF4.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			void SimpleLock::Lock(uint32_t pauseAttempts) {
				uint32_t myThreadID = GetCurrentThreadId();

				_mm_lfence();
				if (threadID == myThreadID)
					InterlockedIncrement(&lockCount);
				else {
					uint32_t attempts = 0;
					if (InterlockedCompareExchange(&lockCount, 1, 0)) {
						do {
							++attempts;
							_mm_pause();
							if (attempts >= pauseAttempts) {
								uint32_t spinCount = 0;
								while (InterlockedCompareExchange(&lockCount, 1, 0))
									Sleep(++spinCount < kFastSpinThreshold ? 1 : 0);
								break;
							}
						} while (InterlockedCompareExchange(&lockCount, 1, 0));
						_mm_lfence();
					}

					threadID = myThreadID;
					_mm_sfence();
				}
			}

			void SimpleLock::Release() {
				uint32_t myThreadID = GetCurrentThreadId();

				_mm_lfence();
				if (threadID == myThreadID) {
					if (lockCount == 1) {
						threadID = 0;
						_mm_mfence();
						InterlockedCompareExchange(&lockCount, 0, 1);
					}
					else
						InterlockedDecrement(&lockCount);
				}
			}

			void BSReadWriteLock::LockForReadAndWrite() {
				uint32_t myThreadID = GetCurrentThreadId();

				if (threadID == myThreadID)
					InterlockedIncrement(&lockValue);
				else {
					uint32_t spinCount = 0;
					while (InterlockedCompareExchange(&lockValue, 1, 0) != 1)
						Sleep(++spinCount >= kFastSpinThreshold ? 1 : 0);

					threadID = myThreadID;
					_mm_mfence();
				}
			}

			bool BSReadWriteLock::TryLockForWrite()
			{
				uint32_t myThreadID = GetCurrentThreadId();

				bool result = FALSE;
				if (threadID == myThreadID) {
					InterlockedIncrement(&lockValue);
					result = TRUE;
				}
				else {
					result = InterlockedCompareExchange(&lockValue,
						uint32_t(1 | kLockWrite), 0) == uint32_t(1 | kLockWrite);
					if (result) {
						threadID = myThreadID;
						_mm_mfence();
					}
				}
				return result;
			}

			bool BSReadWriteLock::TryLockForRead() {
				uint32_t myThreadID = GetCurrentThreadId();

				bool result = FALSE;
				if (threadID == myThreadID) {
					InterlockedIncrement(&lockValue);
					result = TRUE;
				}
				else {
					uint32_t lockCount = lockValue & kLockCountMask;
					uint32_t lockResult = InterlockedCompareExchange(&lockValue, lockCount + 1, lockCount);
					while ((lockResult & kLockWrite) == 0) {
						if (lockResult == lockCount)
							break;

						lockCount = lockResult & kLockCountMask;
						lockResult = InterlockedCompareExchange(&lockValue, lockCount + 1, lockCount);
					}

					result = ~(lockResult >> 31) & 1;
				}

				return result;
			}

			void BSReadWriteLock::Unlock() {
				uint32_t myThreadID = GetCurrentThreadId();
				if (threadID == myThreadID) {
					uint32_t lockCount = lockValue - 1;
					if (lockValue == kLockWrite) {
						threadID = 0;
						_mm_mfence();
						InterlockedExchange(&lockValue, 0);
					}
					else
						InterlockedDecrement(&lockValue);
				}
				else
					InterlockedDecrement(&lockValue);
			}
		}
	}
}