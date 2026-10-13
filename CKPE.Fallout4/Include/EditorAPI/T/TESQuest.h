// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BGSStoryManagerTreeForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSLocation;

				struct QUEST_DATA
				{
					float questDelayTime; // 00
					std::uint16_t flags; // 04
					std::int8_t priority; // 06
					std::int8_t questType; // 07
				};
				static_assert(sizeof(QUEST_DATA) == 0x8);

				// 1.11.240.0: size 0x270, vtable RVA 0x304A590, 129 virtual functions
				class TESQuest :
					public BGSStoryManagerTreeForm,  // 000
					public TESFullName               // 028
				{
				public:
					virtual ~TESQuest() = default;

					constexpr static std::uint8_t TYPE_ID = ftQuest;

					virtual void sub_400() { return; } // 128 - RVA 0xACB470, return a1 + 224;

				public:
					BSTArray<void*> aliases; // 40 - BSTArray<BGSBaseAlias*>
					std::uint32_t unk58; // 58
					std::uint32_t unk5C; // 5C
					BGSLocation* nonDormantLocation; // 60 - LNAM
					std::uint64_t unk68; // 68
					BSFixedString swfFile; // 70 - SNAM
					std::uint64_t filter; // 78 - FLTR chunk (CK only), string
					std::uint32_t unk80; // 80
					std::uint32_t unk84; // 84
					std::uint64_t unk88; // 88
					std::uint64_t unk90; // 90
					std::uint64_t unk98; // 98
					std::uint8_t unkA0; // A0
					std::uint8_t unkA1[0x3]; // A1
					QUEST_DATA data; // A4
					std::uint32_t eventID; // AC - ENAM
					BSTArray<void*> stages; // B0 - BSTArray<TESQuestStage*>
					BSTArray<void*> unkC8; // C8 - element type not identified yet
					std::uint8_t objConditions[0x18]; // E0 - TESCondition (0x18 bytes in the CK, vptr + list head + owner; both owners are set after construction)
					std::uint8_t storyManagerConditions[0x18]; // F8 - TESCondition
					std::uint8_t unk110[0xC]; // 110
					std::uint8_t unk11C[0x8]; // 11C
					std::uint32_t unk124; // 124
					std::uint64_t unk128; // 128
					std::uint32_t unk130; // 130
					std::uint8_t unk134[0xC]; // 134
					std::uint64_t unk140; // 140
					std::uint8_t unk148[0xC]; // 148
					std::uint8_t unk154[0x8]; // 154
					std::uint32_t unk15C; // 15C
					std::uint64_t unk160; // 160
					std::uint32_t unk168; // 168
					std::uint8_t unk16C[0xC]; // 16C
					std::uint64_t unk178; // 178
					BSTArray<void*> topics[6]; // 180 - BSTArray<TESTopic*>[6] (destructor loops over 6 arrays)
					BSTArray<void*> unk210; // 210 - element type not identified yet
					void* textGlobal; // 228 - BSTArray<TESGlobal*>*
					std::uint64_t unk230; // 230
					std::uint8_t unk238; // 238
					std::uint8_t unk239[0x7]; // 239
					std::uint64_t unk240; // 240
					BSTArray<void*> unk248; // 248 - element type not identified yet
					std::uint64_t unk260; // 260
					std::uint64_t unk268; // 268
				};
				static_assert(sizeof(TESQuest) == 0x270);
			}
		}
	}
}
