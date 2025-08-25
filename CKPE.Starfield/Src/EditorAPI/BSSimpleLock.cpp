// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <EditorAPI/BSSimpleLock.h>

namespace CKPE
{
	namespace Starfield
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