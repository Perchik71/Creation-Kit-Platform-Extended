// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESActorBase.h>
#include <EditorAPI/T/TESRaceForm.h>
#include <EditorAPI/B/BGSOverridePackCollection.h>
#include <EditorAPI/B/BGSForcedLocRefType.h>
#include <EditorAPI/B/BGSNativeTerminalForm.h>
#include <EditorAPI/B/BGSAttachParentArray.h>
#include <EditorAPI/B/BGSLocalizedString.h>
#include <EditorAPI/B/BGSSkinForm.h>
#include <EditorAPI/N/NiPoint.h>

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
				class BGSListForm;
				class TESCombatStyle;
				class TESFurniture;
				class BGSOutfit;
				class TESFaction;
				class BGSHeadPart;


				class BGSCharacterTint;
				class Entries;

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
					std::uint8_t unk380[0x10]; // 380
					std::int8_t numHeadParts; // 390
					std::uint8_t unk391[0x7]; // 391
					std::uint8_t unk398[0x10]; // 398
					void* morphSliderValues; // 3A8 - BSTHashMap<std::uint32_t, float>*
					void* tintingData; // 3B0 - BGSCharacterTint::Entries*
				};
				static_assert(sizeof(TESNPC) == 0x3B8);
			}
		}
	}
}
