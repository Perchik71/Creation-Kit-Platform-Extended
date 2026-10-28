// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BSTHashMap.h>
#include <EditorAPI/T/TESActorBase.h>
#include <EditorAPI/T/TESRaceForm.h>
#include <EditorAPI/B/BGSOverridePackCollection.h>
#include <EditorAPI/B/BGSForcedLocRefType.h>
#include <EditorAPI/B/BGSNativeTerminalForm.h>
#include <EditorAPI/B/BGSAttachParentArray.h>
#include <EditorAPI/B/BGSLocalizedString.h>
#include <EditorAPI/B/BGSSkinForm.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BGSCharacterMorph.h>
#include <EditorAPI/B/BSTHashMap.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class HeadRelatedData;
				class TESClass;
				class BGSRelationship;
				class BGSListForm;
				class TESCombatStyle;
				class TESFurniture;
				class BGSOutfit;
				class TESFaction;
				class BGSHeadPart;


				// 1.11.240.0: size 0x3B8, vtable RVA 0x30465B0, 158 virtual functions
				class TESNPC :
					public TESActorBase,               // 000
					public TESRaceForm,                // 248
					public BGSOverridePackCollection,  // 258
					public BGSForcedLocRefType,        // 290
					public BGSNativeTerminalForm       // 2A0
				{
				public:
					virtual ~TESNPC() = default;

					constexpr static std::uint8_t TYPE_ID = ftActor;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESNPC[0];
				public:
					BGSAttachParentArray attachParents; // 2B0
					std::uint8_t data[0x8]; // 2D0 - NPC_DATA (saved as an 8 byte record)
					TESClass* cl; // 2D8
					HeadRelatedData* headRelatedData; // 2E0 - read by GetHairColor
					BGSListForm* giftFilter; // 2E8
					TESCombatStyle* combatStyle; // 2F0
					NiAPI::NiPoint3 morphWeight; // 2F8 - thin/muscular/fat, FLT_MAX means unset
					float height; // 304
					float heightMax; // 308
					std::uint32_t unk30C; // 30C
					TESNPC* faceNPC; // 310
					BGSLocalizedString shortName; // 318
					BGSSkinForm farSkin; // 328
					BGSLocalizedString unk340; // 340
					TESFurniture* powerArmorFurn; // 350
					BGSOutfit* defOutfit; // 358
					BGSOutfit* sleepOutfit; // 360
					BGSListForm* defaultPackList; // 368
					TESFaction* crimeFaction; // 370
					BGSHeadPart** headParts; // 378
					BSTArray<float>* morphRegionSliderValues; // 380 - 24 byte heap array (InitializeData/ClearData), CommonLibF4 position + A8
					BSTHashMap<std::uint32_t, BGSCharacterMorph::Transform>* facialBoneRegionSliderValues; // 388 - 56 byte table freed by ClearData
					std::int8_t numHeadParts; // 390
					std::int8_t soundLevel; // 391 - 1 on construction
					std::int8_t bodyTintColorR; // 392
					std::int8_t bodyTintColorG; // 393
					std::int8_t bodyTintColorB; // 394
					std::int8_t bodyTintColorA; // 395
					std::uint8_t pad396[0x2]; // 396
					BSTArray<BGSRelationship*>* relationships; // 398 - by position like CommonLibF4
					std::uint64_t unk3A0; // 3A0
					BSTHashMap<std::uint32_t, float>* morphSliderValues; // 3A8 - 0x38 byte table allocated by Load
					BGSCharacterTint::Entries* tintingData; // 3B0 - 24 byte heap object
				};
				static_assert(offsetof(TESNPC, tintingData) == 0x3B0);
				static_assert(sizeof(TESNPC) == 0x3B8);
			}
		}
	}
}
