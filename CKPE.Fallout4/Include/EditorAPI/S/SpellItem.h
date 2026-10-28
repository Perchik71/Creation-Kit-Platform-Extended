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
				class BGSPerk;

				// 1.11.246.0: size 0x28 (CommonLibF4 SpellItem::Data; the defaults are set by the constructor 0x140E851B0, SpellItem::GetSpellType
				// reads 320 and GetCastingType 328)
				struct SpellItemData
				{
					std::int32_t costOverride; // 00 - -1 on construction
					std::uint32_t flags; // 04
					std::int32_t spellType; // 08 - REX::TEnumSet<MagicSystem::SpellType, std::int32_t>
					float chargeTime; // 0C
					std::int32_t castingType; // 10 - 3 on construction
					std::int32_t delivery; // 14 - 5 on construction
					float castDuration; // 18
					float range; // 1C
					BGSPerk* castingPerk; // 20
				};
				static_assert(sizeof(SpellItemData) == 0x28);

				// 1.11.240.0: size 0x340, vtable RVA 0x30C3CE8, 188 virtual functions
				class SpellItem :
					public MagicItem,             // 000
					public BGSEquipType,          // 2D0
					public BGSMenuDisplayObject,  // 2E0
					public TESDescription         // 2F0
				{
				public:
					virtual ~SpellItem() = default;

					constexpr static std::uint8_t TYPE_ID = ftSpell;
					constexpr static Common::ID VTABLE = IDs::VTABLE::SpellItem[0];
				public:
					SpellItemData data; // 318
				};
				static_assert(offsetof(SpellItem, data) == 0x318);
				static_assert(sizeof(SpellItem) == 0x340);
			}
		}
	}
}
