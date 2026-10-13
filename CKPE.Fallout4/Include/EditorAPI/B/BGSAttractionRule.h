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
				struct ATTRACTION_RULE_DATA
				{
					float radius; // 00
					float minDelay; // 04
					float maxDelay; // 08
					bool requiresLOS; // 0C
					bool combatTarget; // 0D
				};
				static_assert(sizeof(ATTRACTION_RULE_DATA) == 0x10);

				// 1.11.240.0: size 0x38, vtable RVA 0x2FB50F0, 124 virtual functions
				class BGSAttractionRule :
					public TESForm   // 000
				{
				public:
					virtual ~BGSAttractionRule() = default;

				public:
					ATTRACTION_RULE_DATA data; // 28
				};
				static_assert(sizeof(BGSAttractionRule) == 0x38);
			}
		}
	}
}
