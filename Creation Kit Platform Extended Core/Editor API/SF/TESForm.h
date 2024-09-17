// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "NiAPI/NiNode.h"

#pragma once
#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Starfield
		{
			class TESForm
			{
			public:
				inline static TESForm* (*GetFormByNumericID)(const uint32_t SearchID);
				inline static TESForm* (*GetFormByEditorID)(const char* EditorID);
			};
		}
	}
}

#pragma pack(pop)