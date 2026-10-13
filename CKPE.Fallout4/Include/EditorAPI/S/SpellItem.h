// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/M/MagicItem.h>
#include <EditorAPI/B/BGSEquipType.h>
#include <EditorAPI/B/BGSMenuDisplayObject.h>
#include <EditorAPI/T/TESDescription.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x340, vtable RVA 0x30C3CE8, 188 virtual functions
				class SpellItem :
					public MagicItem,             // 000
					public BGSEquipType,          // 2D0
					public BGSMenuDisplayObject,  // 2E0
					public TESDescription         // 2F0
				{
				public:
					virtual ~SpellItem() = default;

				public:
					std::uint32_t unk318; // 318
					std::uint8_t unk31C[0x8]; // 31C
					std::uint32_t unk324; // 324
					std::uint32_t unk328; // 328
					std::uint8_t unk32C[0x8]; // 32C
					std::uint32_t unk334; // 334
					std::uint64_t unk338; // 338
				};
				static_assert(sizeof(SpellItem) == 0x340);
			}
		}
	}
}
