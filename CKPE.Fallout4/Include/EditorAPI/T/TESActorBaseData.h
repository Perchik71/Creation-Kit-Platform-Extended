// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BaseFormComponent.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESForm;
				class TESGlobal;
				class TESLevItem;
				class TESFaction;
				class BGSVoiceType;
			}

			// 0x14 bytes, layout from CommonLibF4, defaults verified against the CK constructor
			// (level = 1, baseDisposition = global default, everything else 0)
			struct ACTOR_BASE_DATA
			{
				std::uint32_t actorBaseFlags;		// 00
				std::int16_t xpValueOffset;			// 04
				std::uint16_t level;				// 06
				std::uint16_t calcLevelMin;			// 08
				std::uint16_t calcLevelMax;			// 0A
				std::uint16_t baseDisposition;		// 0C
				std::uint16_t templateUseFlags;		// 0E
				std::int16_t bleedoutOverride;		// 10
				std::uint16_t pad12;				// 12
			};
			static_assert(sizeof(ACTOR_BASE_DATA) == 0x14);

			struct FACTION_RANK
			{
				Forms::TESFaction* faction;			// 00
				std::int8_t rank;					// 08
				std::uint8_t pad09[7];				// 09
			};
			static_assert(sizeof(FACTION_RANK) == 0x10);


			// 1.11.246.0: size 0x68, vtable RVA 0x301CB50, 25 virtual functions
			class TESActorBaseData :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~TESActorBaseData() = default;

				virtual void CopyFromTemplateForms() noexcept(true) { return; } // 21 - RVA 0x8ADD30
				virtual bool GetIsGhost() const noexcept(true) { return false; } // 22 - RVA 0x8ADE20
				virtual bool GetInvulnerable() const noexcept(true) { return false; } // 23 - RVA 0x8ADE10
				virtual void UpdateLevelControls(HWND hDialogBox) const noexcept(true) { return; } // 24

			public:
				ACTOR_BASE_DATA actorData; // 08
				std::int32_t changeFlags; // 1C
				Forms::TESLevItem* deathItem; // 20
				Forms::BGSVoiceType* voiceType; // 28
				Forms::TESForm* baseTemplateForm; // 30
				Forms::TESForm** templateForms; // 38
				Forms::TESGlobal* legendChance; // 40
				Forms::TESForm* legendTemplate; // 48
				BSTArray<FACTION_RANK> factions; // 50
			};
			static_assert(sizeof(TESActorBaseData) == 0x68);
		}
	}
}
