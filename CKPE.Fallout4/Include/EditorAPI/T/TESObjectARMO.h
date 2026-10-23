// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BGSTypedFormValuePair.h>
#include <EditorAPI/B/TBO_InstanceData.h>
#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESRaceForm.h>
#include <EditorAPI/T/TESEnchantableForm.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSPickupPutdownSounds.h>
#include <EditorAPI/T/TESBipedModelForm.h>
#include <EditorAPI/B/BGSEquipType.h>
#include <EditorAPI/B/BGSBipedObjectForm.h>
#include <EditorAPI/B/BGSBlockBashData.h>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/T/TESDescription.h>
#include <EditorAPI/B/BGSInstanceNamingRulesForm.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BGSAttachParentArray.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESObjectARMA;
				class EnchantmentItem;
				class BGSMaterialSwap;

				// 1.11.246.0: size 0x58, created by TESObjectARMO::CreateInstanceData (0x140C6CCF0); the constructor defaults match CommonLibF4's
				// TESObjectARMO::InstanceData (colorRemappingIndex is FLT_MAX until set).
				class ArmorInstanceData :
					public TBO_InstanceData   // 00
				{
				public:
					BSTArray<EnchantmentItem*>* enchantments; // 10
					BSTArray<BGSMaterialSwap*>* materialSwaps; // 18
					BGSBlockBashData* blockBashData; // 20
					BGSKeywordForm* keywords; // 28
					BGSTypedFormValuePair::Array* damageTypes; // 30
					BGSTypedFormValuePair::Array* actorValues; // 38
					float weight; // 40
					float colorRemappingIndex; // 44
					std::uint32_t value; // 48
					std::uint32_t health; // 4C
					std::int32_t staggerRating; // 50 - REX::TEnumSet<STAGGER_MAGNITUDE, std::int32_t>
					std::uint16_t rating; // 54
					std::uint16_t index; // 56
				};
				static_assert(sizeof(ArmorInstanceData) == 0x58);

				class ArmorAddon
				{
				public:
					std::uint16_t index; // 00
					std::uint8_t pad02[6]; // 02
					TESObjectARMA* armorAddon; // 08
				};
				static_assert(sizeof(ArmorAddon) == 0x10);

				// 1.11.240.0: size 0x3C0, vtable RVA 0x3094C30, 154 virtual functions
				class TESObjectARMO :
					public TESBoundObject,              // 000
					public TESFullName,                 // 090
					public TESRaceForm,                 // 0A8
					public TESEnchantableForm,          // 0B8
					public BGSDestructibleObjectForm,   // 0D0
					public BGSPickupPutdownSounds,      // 0E0
					public TESBipedModelForm,           // 0F8
					public BGSEquipType,                // 288
					public BGSBipedObjectForm,          // 298
					public BGSBlockBashData,            // 2A8
					public BGSKeywordForm,              // 2C0
					public TESDescription,              // 2E0
					public BGSInstanceNamingRulesForm   // 308
				{
				public:
					virtual ~TESObjectARMO() = default;

				public:
					ArmorInstanceData armorData; // 318
					BSTArray<ArmorAddon> modelArray; // 370
					TESObjectARMO* armorTemplate; // 388
					BGSAttachParentArray attachParents; // 390
					std::uint8_t unk3B0[0x10]; // 3B0
				};
				static_assert(sizeof(TESObjectARMO) == 0x3C0);
			}
		}
	}
}
