// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

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
					std::uint64_t unk318; // 318
					std::uint64_t unk320; // 320
					std::uint64_t unk328; // 328
					std::uint64_t unk330; // 330
					std::uint64_t unk338; // 338
					std::uint64_t unk340; // 340
					std::uint64_t unk348; // 348
					std::uint64_t unk350; // 350
					std::uint32_t unk358; // 358
					std::uint8_t unk35C[0x8]; // 35C
					std::uint8_t unk364[0x8]; // 364
					std::uint32_t unk36C; // 36C
					BSTArray<void*> modelArray; // 370 - BSTArray<ArmorAddon>
					TESObjectARMO* armorTemplate; // 388
					BGSAttachParentArray attachParents; // 390
					std::uint8_t unk3B0[0x10]; // 3B0
				};
				static_assert(sizeof(TESObjectARMO) == 0x3C0);
			}
		}
	}
}
