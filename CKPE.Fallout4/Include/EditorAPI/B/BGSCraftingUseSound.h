// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BaseFormComponent.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSSoundDescriptorForm;
			}

			// 1.11.240.0: size 0x10, vtable RVA 0x2FA6BA8, 21 virtual functions
			class BGSCraftingUseSound :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~BGSCraftingUseSound() = default;

			public:
				Forms::BGSSoundDescriptorForm* craftingUseSound; // 08
			};
			static_assert(sizeof(BGSCraftingUseSound) == 0x10);
		}
	}
}
