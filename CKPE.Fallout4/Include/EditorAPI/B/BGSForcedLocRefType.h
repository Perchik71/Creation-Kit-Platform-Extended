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
				class BGSLocationRefType;
			}

			// 1.11.240.0: size 0x10, vtable RVA 0x2FA6E80, 21 virtual functions
			class BGSForcedLocRefType :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~BGSForcedLocRefType() = default;

			public:
				Forms::BGSLocationRefType* forcedType; // 08
			};
			static_assert(sizeof(BGSForcedLocRefType) == 0x10);
		}
	}
}
