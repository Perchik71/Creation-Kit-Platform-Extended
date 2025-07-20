#include <windows.h>
#include <EditorAPI/BSSimpleLock.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			void SimpleLock::Lock() noexcept(true)
			{
				int32_t myThreadID = GetCurrentThreadId();
				if (threadID == myThreadID) {
					lockCount++;
					return;
				}

				uint32_t spinCount = 0;
				while (InterlockedCompareExchange((PLONG)&threadID, myThreadID, 0))
					Sleep(++spinCount > kFastSpinThreshold);

				lockCount = 1;
			}

			void SimpleLock::Release() noexcept(true)
			{
				if (--lockCount == 0)
					InterlockedCompareExchange((PLONG)&threadID, 0, threadID);
			}
		}
	}
}