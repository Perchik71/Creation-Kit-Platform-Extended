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
				class TESImageSpaceModifier;
			}

			// 1.11.240.0: size 0x10, vtable RVA 0x30204E0, 21 virtual functions
			class TESImageSpaceModifiableForm :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~TESImageSpaceModifiableForm() = default;

			public:
				Forms::TESImageSpaceModifier* formImageSpaceModifying; // 08
			};
			static_assert(sizeof(TESImageSpaceModifiableForm) == 0x10);
		}
	}
}
