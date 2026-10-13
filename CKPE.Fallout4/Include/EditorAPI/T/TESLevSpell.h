// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESLeveledList.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0xB8, vtable RVA 0x308EB50, 154 virtual functions
				class TESLevSpell :
					public TESBoundObject,  // 000
					public TESLeveledList   // 090
				{
				public:
					virtual ~TESLevSpell() = default;
				};
				static_assert(sizeof(TESLevSpell) == 0xB8);
			}
		}
	}
}
