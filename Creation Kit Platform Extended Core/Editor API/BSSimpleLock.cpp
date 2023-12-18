#include "BSSimpleLock.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		void SimpleLock::Lock(void)
		{
			int32_t myThreadID = GetCurrentThreadId();
			if (threadID == myThreadID) {
				lockCount++;
				return;
			}

			uint32_t spinCount = 0;
			while (InterlockedCompareExchange(&threadID, myThreadID, 0))
				Sleep(++spinCount > kFastSpinThreshold);

			lockCount = 1;
		}

		void SimpleLock::Release(void)
		{
			if (--lockCount == 0)
				InterlockedCompareExchange(&threadID, 0, threadID);
		}
	}
}