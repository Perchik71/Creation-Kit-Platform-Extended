// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESDescription.h>
#include <EditorAPI/T/TESIcon.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/B/BSFixedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSSoundDescriptorForm;

				struct PerkData
				{
					bool trait; // 00
					std::int8_t level; // 01
					std::int8_t numRanks; // 02
					bool playable; // 03
					bool hidden; // 04
				};
				static_assert(sizeof(PerkData) == 0x5);

				// 1.11.240.0: size 0x138, vtable RVA 0x303B298, 124 virtual functions
				class BGSPerk :
					public TESForm,         // 000
					public TESFullName,     // 028
					public TESDescription,  // 040
					public TESIcon          // 068
				{
				public:
					virtual ~BGSPerk() = default;

				public:
					PerkData data; // 90
					std::uint8_t unk95[0x3]; // 95
					std::uint8_t perkConditions[0x18]; // 98 - TESCondition (0x18 in the CK) - CTDA
					BSTArray<void*> perkEntries; // B0 - BSTArray<BGSPerkEntry*>, by position after perkConditions
					BSFixedString swfFile; // C8 - BSFixedStringCS
					BGSPerk* nextPerk; // D0
					BGSSoundDescriptorForm* sound; // D8
					std::uint64_t unkE0; // E0
					std::uint64_t unkE8; // E8
					std::uint8_t unkF0; // F0
					std::uint8_t unkF1[0x13]; // F1
					std::uint8_t unk104[0x8]; // 104
					std::uint32_t unk10C; // 10C
					std::uint64_t unk110; // 110
					std::uint32_t unk118; // 118
					std::uint8_t unk11C[0xC]; // 11C
					std::uint64_t unk128; // 128
					std::uint16_t unk130; // 130
					std::uint8_t unk132[0x6]; // 132
				};
				static_assert(sizeof(BGSPerk) == 0x138);
			}
		}
	}
}
