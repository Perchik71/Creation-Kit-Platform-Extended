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
#include <EditorAPI/B/BSTHashMap.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSLocation;
				class BGSBaseAlias;
				class TESQuestStage;
				class BGSQuestObjective;
				class BGSDialogueBranch;
				class BGSScene;
				class TESGlobal;
				class TESTopic;

				struct QUEST_DATA
				{
					float questDelayTime; // 00
					std::uint16_t flags; // 04
					std::int8_t priority; // 06
					std::int8_t questType; // 07
				};
				static_assert(sizeof(QUEST_DATA) == 0x8);

				// 1.11.246.0: size 0x270, vtable RVA 0x304B590, 129 virtual functions
				class TESQuest :
					public BGSStoryManagerTreeForm,  // 000
					public TESFullName               // 028
				{
				public:
					virtual ~TESQuest() = default;

					constexpr static std::uint8_t TYPE_ID = ftQuest;

					virtual void* GetConditions() noexcept(true) { return nullptr; } // 128

				public:
					BSTArray<BGSBaseAlias*> aliases; // 40 - polymorphic objects, destroyed through their virtual destructor by ClearData
					std::uint32_t unk58; // 58
					std::uint32_t unk5C; // 5C
					BGSLocation* nonDormantLocation; // 60 - LNAM
					TESGlobal* questCompleteXPGlobal; // 68 - 0 on construction, by position like CommonLibF4
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
					BSTArray<TESQuestStage*> stages; // B0 - 0x28 byte heap objects
					BSTArray<BGSQuestObjective*> objectives; // C8 - 0x30 byte heap objects (the game's order: stages, objectives)
					std::uint8_t objConditions[0x18]; // E0 - TESCondition (0x18 bytes in the CK, vptr + list head + owner; both owners are set after construction)
					std::uint8_t storyManagerConditions[0x18]; // F8 - TESCondition
					// Two BSTHashMap<BGSDialogueBranch*, BSTArray<TESTopic*>*> in the CK's 0x38 byte layout (CommonLibF4's branchedDialogues; the CK has no
					// stage/objective/alias index tables). The constructor sets the end of chain markers at 128 and 160.
					BSTHashMap38<BGSDialogueBranch*, BSTArray<TESTopic*>*> branchedDialogues[2]; // 110
					BSTArray<TESTopic*> topics[6]; // 180 - the destructor loops over 6 arrays
					BSTArray<BGSScene*> scenes; // 210 - by position like CommonLibF4's scenes
					BSTArray<TESGlobal*>* textGlobal; // 228
					std::uint64_t unk230; // 230 - cleared as one qword by the constructor and ClearData; by position CommonLibF4's totalRefsAliased / currentStage / alreadyRun
					std::uint8_t unk238; // 238
					std::uint8_t unk239[0x7]; // 239
					std::uint64_t unk240; // 240
					BSTArray<void*> unk248; // 248 - element type not identified yet
					std::uint64_t unk260; // 260
					std::uint64_t unk268; // 268
				};
				static_assert(offsetof(TESQuest, stages) == 0xB0);
				static_assert(offsetof(TESQuest, branchedDialogues) == 0x110);
				static_assert(offsetof(TESQuest, topics) == 0x180);
				static_assert(offsetof(TESQuest, textGlobal) == 0x228);
				static_assert(sizeof(TESQuest) == 0x270);
			}
		}
	}
}
