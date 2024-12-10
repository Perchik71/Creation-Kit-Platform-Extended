// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "ResultCoreErrNo.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		const char* GetMessageStringByErrorNo(IResult nErrorNo)
		{
			static const char* szMessage[] = {
				"",
				"Unsupported version of Creation Kit.\nYou may need to update this mod."
				"\nCheck out the list of supported versions on the mod page.",
				"Deprecated version of Creation Kit.\nYou need to update the Creation Kit."
				"\nCheck out the list of supported versions on the mod page.",
				"Unknown application",
				"Reinitializing the engine"
				"Your OS is not supported"
			};

			return szMessage[nErrorNo];
		}
	}
}