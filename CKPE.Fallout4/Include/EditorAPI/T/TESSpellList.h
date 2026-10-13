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
			class SpellData;

			// 1.11.240.0: size 0x10, vtable RVA 0x3022250, 21 virtual functions
			class TESSpellList :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~TESSpellList() = default;

			public:
				SpellData* spellData; // 08 - TESSpellList::SpellData
			};
			static_assert(sizeof(TESSpellList) == 0x10);
		}
	}
}
