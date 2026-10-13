// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/BGSModelMaterialSwap.h>
#include <EditorAPI/T/TESIcon.h>
#include <EditorAPI/T/TESEnchantableForm.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSEquipType.h>
#include <EditorAPI/B/BGSPreloadable.h>
#include <EditorAPI/B/BGSMessageIcon.h>
#include <EditorAPI/B/BGSPickupPutdownSounds.h>
#include <EditorAPI/B/BGSBlockBashData.h>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/T/TESDescription.h>
#include <EditorAPI/B/BGSInstanceNamingRulesForm.h>
#include <EditorAPI/B/BGSAttachParentArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class ActorValueInfo;
				class BGSAimModel;
				class BGSEquipSlot;
				class BGSImpactDataSet;
				class BGSSoundDescriptorForm;
				class BGSZoomData;
				class SpellItem;
				class TESAmmo;
				class TESLevItem;

				struct TESObjectWEAPData
				{
					std::uint8_t tboInstanceData[0x10]; // 00 - TBO_InstanceData (vptr, refcount)
					BGSSoundDescriptorForm* attackSound; // 10
					BGSSoundDescriptorForm* attackSound2D; // 18
					BGSSoundDescriptorForm* attackLoop; // 20
					BGSSoundDescriptorForm* attackFailSound; // 28
					BGSSoundDescriptorForm* idleSound; // 30
					BGSSoundDescriptorForm* equipSound; // 38
					BGSSoundDescriptorForm* unEquipSound; // 40
					BGSSoundDescriptorForm* fastEquipSound; // 48
					void* blockBashData; // 50 - BGSBlockBashData*
					BGSImpactDataSet* impactDataSet; // 58
					TESLevItem* npcAddAmmoList; // 60
					TESAmmo* ammo; // 68
					BGSEquipSlot* equipSlot; // 70
					SpellItem* effect; // 78
					void* keywords; // 80 - BGSKeywordForm*
					BGSAimModel* aimModel; // 88
					BGSZoomData* zoomData; // 90
					void* rangedData; // 98 - RangedData*
					void* enchantments; // A0 - BSTArray<EnchantmentItem*>*
					void* materialSwaps; // A8 - BSTArray<BGSMaterialSwap*>*
					void* damageTypes; // B0 - BSTArray<BSTTuple<TESForm*, BGSTypedFormValuePair::SharedVal>>*
					void* actorValues; // B8 - BSTArray<BSTTuple<TESForm*, BGSTypedFormValuePair::SharedVal>>*
					float attackSeconds; // C0
					float reloadSpeed; // C4
					float speed; // C8
					float reach; // CC
					float minRange; // D0
					float maxRange; // D4
					float attackDelaySec; // D8
					float damageToWeaponMult; // DC
					float outOfRangeDamageMult; // E0
					float secondaryDamage; // E4
					float criticalChargeBonus; // E8
					float weight; // EC
					float soundLevelMult; // F0
					float attackActionPointCost; // F4
					float fullPowerSeconds; // F8
					float minPowerPerShot; // FC
					float colorRemappingIndex; // 100
					float criticalDamageMult; // 104
					std::int32_t staggerValue; // 108 - REX::TEnumSet<STAGGER_MAGNITUDE, std::int32_t>
					std::uint32_t value; // 10C
					std::uint32_t flags; // 110 - REX::TEnumSet<WEAPON_FLAGS, std::uint32_t>
					std::int32_t soundLevel; // 114 - REX::TEnumSet<SOUND_LEVEL, std::int32_t>
					std::int32_t hitBehavior; // 118 - REX::TEnumSet<WEAPONHITBEHAVIOR, std::int32_t>
					ActorValueInfo* skill; // 120
					ActorValueInfo* resistance; // 128
					std::uint16_t ammoCapacity; // 130
					std::uint16_t attackDamage; // 132
					std::uint16_t rank; // 134
					std::int8_t accuracyBonus; // 136
					std::uint8_t type; // 137 - REX::TEnum<WEAPON_TYPE, std::uint8_t>
				};
				static_assert(sizeof(TESObjectWEAPData) == 0x138);

				// 1.11.240.0: size 0x3B0, vtable RVA 0x309E6E0, 154 virtual functions
				class TESObjectWEAP :
					public TESBoundObject,              // 000
					public TESFullName,                 // 090
					public BGSModelMaterialSwap,        // 0A8
					public TESIcon,                     // 0F8
					public TESEnchantableForm,          // 120
					public BGSDestructibleObjectForm,   // 138
					public BGSEquipType,                // 148
					public BGSPreloadable,              // 158
					public BGSMessageIcon,              // 160
					public BGSPickupPutdownSounds,      // 190
					public BGSBlockBashData,            // 1A8
					public BGSKeywordForm,              // 1C0
					public TESDescription,              // 1E0
					public BGSInstanceNamingRulesForm   // 208
				{
				public:
					virtual ~TESObjectWEAP() = default;

				public:
					TESObjectWEAPData weaponData; // 218 - TESObjectWEAP::Data (0x138, starts with a vptr)
					void* firstPersonModel; // 350 - BGSModelMaterialSwap*
					TESObjectWEAP* weaponTemplate; // 358
					void* embeddedWeaponMod; // 360 - BGSMod::Attachment::Mod*
					BGSAttachParentArray attachParents; // 368
					std::uint32_t unk388; // 388
					std::uint32_t unk38C; // 38C
					std::uint64_t unk390; // 390
					std::uint32_t unk398; // 398
					std::uint32_t unk39C; // 39C
					std::uint8_t unk3A0[0x10]; // 3A0
				};
				static_assert(sizeof(TESObjectWEAP) == 0x3B0);
			}
		}
	}
}
