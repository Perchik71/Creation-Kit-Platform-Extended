// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESLeveledList.h>
#include <EditorAPI/B/BGSModelMaterialSwap.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x108, vtable RVA 0x308D820, 154 virtual functions
				class TESLevCharacter :
					public TESBoundAnimObject,    // 000
					public TESLeveledList,        // 090
					public BGSModelMaterialSwap   // 0B8
				{
				public:
					virtual ~TESLevCharacter() = default;
				};
				static_assert(sizeof(TESLevCharacter) == 0x108);
			}
		}
	}
}
