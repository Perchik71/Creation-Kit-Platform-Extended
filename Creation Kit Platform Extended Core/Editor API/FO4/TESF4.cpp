// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "TESF4.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			using namespace CreationKitPlatformExtended::Core;

			void TES::SkyM::Disable()
			{
				flag = 0;

			//	if (!TESUnknown::Instance->Main->HasInterior())
			//		lpClouds->Enabled = false;
			}

			void TES::SkyM::Enable()
			{
				if (TESUnknown::Instance->Main->HasInterior())
					flag = ftSkyInInterior;
				else
				{
					flag = ftSkyInExterior;
					//lpClouds->Enabled = true;
				}
			}
		}
	}
}