// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BGSStoryManagerNodeBase.h>
#include <EditorAPI/B/BSTHashMap.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESQuest;
				// 1.11.246.0: size 0xE8, vtable RVA 0x310D630, 136 virtual functions
				class BGSStoryManagerQuestNode :
					public BGSStoryManagerNodeBase   // 000
				{
				public:
					virtual ~BGSStoryManagerQuestNode() = default;

					virtual std::uint32_t AcceptVisitor(void* Visitor) noexcept(true) { return 0; } // 135

				public:
					/* 0x058 */ BSTArray<TESQuest*> children;
					/* 0x070 */ BSTHashMap<TESQuest*, std::uint32_t> perQuestFlags;
					/* 0x0A8 */ BSTHashMap<TESQuest*, float> perQuestHoursUntilReset;
					/* 0x0E0 */ std::uint32_t numQuestsToStart;
					/* 0x0E4 */ std::uint8_t padE4[0x4];
				};
				static_assert(sizeof(BGSStoryManagerQuestNode) == 0xE8);
				static_assert(offsetof(BGSStoryManagerQuestNode, children) == 0x58);
				static_assert(offsetof(BGSStoryManagerQuestNode, perQuestFlags) == 0x70);
				static_assert(offsetof(BGSStoryManagerQuestNode, perQuestHoursUntilReset) == 0xA8);
				static_assert(offsetof(BGSStoryManagerQuestNode, numQuestsToStart) == 0xE0);
			}
		}
	}
}
