// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/BGSKeywordForm.h>
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
				class TESFaction;
				class BGSMusicType;
				class BGSEncounterZone;

				// 1.11.240.0: size 0x1E0, vtable RVA 0x30A3F38, 124 virtual functions
				class BGSLocation :
					public TESForm,         // 000
					public TESFullName,     // 028
					public BGSKeywordForm   // 040
				{
				public:
					virtual ~BGSLocation() = default;

				public:
					BGSLocation* parentLoc; // 60
					TESFaction* unreportedCrimeFaction; // 68
					BGSMusicType* musicType; // 70
					BGSEncounterZone* zone; // 78
					std::uint64_t unk80; // 80
					std::uint32_t worldLocMarker; // 88 - MNAM - ObjectRefHandle
					float worldLocRadius; // 8C - RNAM
					float actorFadeMult; // 90 - ANAM
					std::uint32_t horseLocMarker; // 94 - NAM0 - ObjectRefHandle
					BSTArray<void*> specialRefs; // 98 - BSTArray<SpecialRefData>
					BSTArray<void*> uniqueNPCs; // B0 - BSTArray<UniqueNPCData>
					BSTArray<void*> unkC8; // C8 - element type not identified yet
					// Three 0x30 byte BSTHashMap-like tables (BSTScatterTable: size dword at +0x14, free entry tag pointer at +0x18); element types not identified.
					std::uint8_t hashMapE0[0x30]; // E0
					std::uint64_t unk110; // 110
					BSTArray<void*> unk118; // 118 - element type not identified yet
					BSTArray<void*> unk130; // 130 - element type not identified yet
					std::uint8_t hashMap148[0x30]; // 148
					std::uint64_t unk178; // 178
					std::uint8_t hashMap180[0x30]; // 180
					std::uint64_t unk1B0; // 1B0
					std::uint64_t unk1B8; // 1B8
					BSTArray<void*> unk1C0; // 1C0 - element type not identified yet
					std::uint32_t unk1D8; // 1D8
					std::uint32_t unk1DC; // 1DC
				};
				static_assert(offsetof(BGSLocation, hashMapE0) == 0xE0);
				static_assert(offsetof(BGSLocation, hashMap148) == 0x148);
				static_assert(offsetof(BGSLocation, hashMap180) == 0x180);
				static_assert(sizeof(BGSLocation) == 0x1E0);
			}
		}
	}
}
