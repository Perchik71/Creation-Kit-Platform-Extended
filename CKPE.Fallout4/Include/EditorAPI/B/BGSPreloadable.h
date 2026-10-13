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
			// 1.11.240.0: size 0x8, vtable RVA 0x3030B88, 21 virtual functions
			class BGSPreloadable :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~BGSPreloadable() = default;
			};
			static_assert(sizeof(BGSPreloadable) == 0x8);
		}
	}
}
