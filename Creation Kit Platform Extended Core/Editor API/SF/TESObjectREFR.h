// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "TESForm.h"

#pragma once
#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Starfield
		{
			class TESObjectREFR : public TESForm
			{
			public:
				// The function returns BSFadeNode, it has not been studied, I use parent class
				inline NiAPI::Starfield::NiNode* GetFadeNode() const
				{
					NiAPI::Starfield::NiNode* pReturn = nullptr;
					thisVirtualCall<NiAPI::Starfield::NiNode**>(0x708, this, &pReturn);
					return pReturn;
				}
			};
		}
	}
}

#pragma pack(pop)