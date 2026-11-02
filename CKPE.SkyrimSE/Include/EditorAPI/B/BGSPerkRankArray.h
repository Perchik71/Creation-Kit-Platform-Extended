// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BaseFormComponent.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			// 1.7.99.0: size 0x10, vtable RVA 0x3078028. Pointer @08 is zeroed on construction (perk rank entries; layout not resolved).
			class BGSPerkRankArray : public BaseFormComponent
			{
			public:
				virtual ~BGSPerkRankArray() = default;

				void* unk08;	// 08
			};
			static_assert(sizeof(BGSPerkRankArray) == 0x10);
		}
	}
}