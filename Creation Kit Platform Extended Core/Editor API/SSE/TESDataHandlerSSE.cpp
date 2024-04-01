// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "TESDataHandler.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			TESDataHandler* TESDataHandler_CKPEIntf::GetSingleton()
			{
				__try
				{
					return TESDataHandler::Singleton.Singleton;
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					return nullptr;
				}
			}

			TESForm* TESDataHandler_CKPEIntf::GetFormById(uint32_t FormId)
			{
				__try
				{
					return TESForm::GetFormByNumericID(FormId);
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					return nullptr;
				}
			}
		}
	}
}