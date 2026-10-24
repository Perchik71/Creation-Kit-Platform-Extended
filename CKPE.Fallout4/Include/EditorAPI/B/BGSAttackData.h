// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/B/BSTHashMap.h>
#include <EditorAPI/N/NiRefObject.h>

namespace CKPE::Fallout4::EditorAPI
{
	namespace Forms
	{
		class SpellItem;
		class BGSKeyword;
		class BGSEquipSlot;
		class TESRace;
	}

	struct AttackData
	{
		/* 0x000 */ float damageMult;
		/* 0x004 */ float attackChance;
		/* 0x008 */ Forms::SpellItem* attackSpell;
		/* 0x010 */ std::uint32_t flags;
		/* 0x014 */ float attackAngle;
		/* 0x018 */ float strikeAngle;
		/* 0x01C */ std::int32_t staggerOffset;
		/* 0x020 */ Forms::BGSKeyword* attackType;
		/* 0x028 */ float knockdown;
		/* 0x02C */ float recoveryTime;
		/* 0x030 */ float actionPointsMult;
		/* 0x034 */ std::uint8_t pad34[0x4];
	};
	static_assert(sizeof(AttackData) == 0x38);
	static_assert(offsetof(AttackData, attackType) == 0x20);

	class BGSAttackData : public NiAPI::NiRefObject
	{
	public:
		/* 0x010 */ BSFixedString event;
		/* 0x018 */ AttackData data;
		/* 0x050 */ Forms::BGSEquipSlot* weaponEquipSlot;
		/* 0x058 */ Forms::BGSEquipSlot* requiredEquipSlot;
		/* 0x060 */ BSFixedString unk60;
	};
	static_assert(sizeof(BGSAttackData) == 0x68);
	static_assert(offsetof(BGSAttackData, data) == 0x18);
	static_assert(offsetof(BGSAttackData, weaponEquipSlot) == 0x50);
	static_assert(offsetof(BGSAttackData, unk60) == 0x60);

	class BGSAttackDataMap : public NiAPI::NiRefObject
	{
	public:
		/* 0x010 */ BSTHashMap<BSFixedString, BGSAttackData*> attackDataMap;
		/* 0x048 */ Forms::TESRace* defaultDataRace;
	};
	static_assert(sizeof(BGSAttackDataMap) == 0x50);
	static_assert(offsetof(BGSAttackDataMap, attackDataMap) == 0x10);
	static_assert(offsetof(BGSAttackDataMap, defaultDataRace) == 0x48);
}
