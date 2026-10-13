// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BaseFormComponent.h>
#include <EditorAPI/T/TESIcon.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// 1.11.240.0: size 0x30, vtable RVA 0x3019F98, 21 virtual functions
			class BGSMessageIcon :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~BGSMessageIcon() = default;

			public:
				TESIcon icon; // 08
			};
			static_assert(sizeof(BGSMessageIcon) == 0x30);
		}
	}
}
