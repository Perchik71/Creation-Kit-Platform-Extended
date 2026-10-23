// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/F/FormInterfaces.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/B/BGSKeywordForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESQuest;
				class TESTopic;
				class BGSKeyword;
				class BGSSoundOutput;

				// 1.11.240.0: size 0x230, vtable RVA 0x303DEA8, 124 virtual functions
				class BGSScene :
					public TESForm,           // 000
					public IKeywordFormBase   // 028
				{
				public:
					virtual ~BGSScene() = default;

				public:
					std::uint32_t unk30; // 30
					std::uint32_t unk34; // 34
					BSTArray<void*> phases; // 38 - BSTArray<BGSScenePhase*>
					BSTArray<std::uint32_t> actors; // 50
					BSTArray<std::uint32_t> actorFlags; // 68
					BSTArray<std::uint32_t> actorProgressionFlags; // 80
					BSTArray<void*> actions; // 98 - BSTArray<BGSSceneAction*>
					TESQuest* parentQuest; // B0
					BGSScene* templateScene; // B8
					std::uint32_t flags; // C0
					std::uint32_t unkC4; // C4
					std::uint8_t repeatConditions[0x18]; // C8 - TESCondition (0x18 in the CK)
					std::uint8_t unkE0[0x2C]; // E0
					std::uint32_t unk10C; // 10C
					std::uint64_t unk110; // 110
					std::uint8_t unk118[0x48]; // 118
					// Embedded BGSSceneActionDialogue (0x160-0x1E7, vtable BGSSceneActionDialogue installed by the constructor). The CK scene action base
					// is 0x30 bytes (the game's is 0x20); the dialogue members then follow CommonLibF4's BGSSceneActionDialogue (topic, loop times,
					// head track targets, loop timer, keywords, output model) shifted by 0x170.
					void* dialogueActionVTable; // 160
					std::uint64_t unk168; // 168
					std::uint32_t unk170; // 170
					std::uint32_t unk174; // 174
					std::uint64_t unk178; // 178
					std::int32_t unk180; // 180 - -1 on construction
					std::uint8_t unk184[0xC]; // 184
					TESTopic* dialogueTopic; // 190
					float maxLoopTime; // 198 - 10.0 on construction
					float minLoopTime; // 19C - 1.0 on construction
					BSTArray<std::uint32_t> headTrackTargets; // 1A0 - ObjectRefHandle
					float currentLoopTimer; // 1B8
					std::uint32_t unk1BC; // 1BC
					BGSKeyword* animFaceArchType; // 1C0
					BGSKeyword* subtypeKeyword; // 1C8
					BGSSoundOutput* outputModel; // 1D0
					BSFixedString unk1D8; // 1D8 - the game's pInputLayer slot is a string in the CK
					std::uint16_t unk1E0; // 1E0
					std::uint8_t unk1E2; // 1E2
					std::uint8_t unk1E3[0x5]; // 1E3
					BGSKeywordForm* keywordForm; // 1E8 - allocated in the constructor (KSIZ chunk)
					std::uint16_t unk1F0; // 1F0
					std::uint16_t unk1F2; // 1F2
					std::uint32_t unk1F4; // 1F4
					std::uint32_t unk1F8; // 1F8
					std::uint32_t unk1FC; // 1FC
					std::uint64_t unk200; // 200
					std::uint64_t unk208; // 208
					std::uint64_t unk210; // 210
					std::uint64_t unk218; // 218
					std::uint64_t unk220; // 220
					std::uint16_t unk228; // 228
					std::uint8_t unk22A[0x6]; // 22A
				};
				static_assert(offsetof(BGSScene, dialogueActionVTable) == 0x160);
				static_assert(offsetof(BGSScene, headTrackTargets) == 0x1A0);
				static_assert(offsetof(BGSScene, keywordForm) == 0x1E8);
				static_assert(sizeof(BGSScene) == 0x230);
			}
		}
	}
}
