// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0xA8, vtable RVA 0x305AAD8, 124 virtual functions
				class BGSFootstepSet :
					public TESForm   // 000
				{
				public:
					virtual ~BGSFootstepSet() = default;

				public:
					std::uint8_t unk28[0x80]; // 28
				};
				static_assert(sizeof(BGSFootstepSet) == 0xA8);
			}
		}
	}
}
