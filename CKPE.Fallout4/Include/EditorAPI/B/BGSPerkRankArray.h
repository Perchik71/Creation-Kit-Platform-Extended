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
			class PerkRankData;

			// 1.11.240.0: size 0x10, vtable RVA 0x301AFE0, 21 virtual functions
			class BGSPerkRankArray :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~BGSPerkRankArray() = default;

				constexpr static Common::ID VTABLE = IDs::VTABLE::BGSPerkRankArray[0];
			public:
				PerkRankData* perks; // 08 - the CK has no perkCount member (size 0x10)
			};
			static_assert(sizeof(BGSPerkRankArray) == 0x10);
		}
	}
}
