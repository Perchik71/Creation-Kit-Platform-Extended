// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BGSEquipType.h>
#include <EditorAPI/B/BGSMenuDisplayObject.h>
#include <EditorAPI/B/TESDescription.h>
#include <EditorAPI/T/MagicItem.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x2D8, vtable RVA 0x312BE28
				class SpellItem :
					public MagicItem,	// 00
					public BGSEquipType,	// 268
					public BGSMenuDisplayObject,	// 278
					public TESDescription	// 288
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftSpell;

					virtual ~SpellItem() = default;
				public:
					std::uint8_t unk2B0[0x28];	// 2B0 - data, layout not resolved
				};
				static_assert(offsetof(SpellItem, unk2B0) == 0x2B0);
				static_assert(sizeof(SpellItem) == 0x2D8);
			}
		}
	}
}