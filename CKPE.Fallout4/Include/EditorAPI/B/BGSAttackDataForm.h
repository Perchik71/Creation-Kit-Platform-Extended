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
			class BGSAttackDataMap;

			// 1.11.240.0: size 0x10, vtable RVA 0x3018670, 21 virtual functions
			class BGSAttackDataForm :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~BGSAttackDataForm() = default;

			public:
				BGSAttackDataMap* attackDataMap; // 08 - NiPointer<BGSAttackDataMap>
			};
			static_assert(sizeof(BGSAttackDataForm) == 0x10);
		}
	}
}
