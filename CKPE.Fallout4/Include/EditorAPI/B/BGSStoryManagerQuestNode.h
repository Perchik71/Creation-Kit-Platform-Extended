// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BGSStoryManagerNodeBase.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.246.0: size 0xE8, vtable RVA 0x310D630, 136 virtual functions
				class BGSStoryManagerQuestNode :
					public BGSStoryManagerNodeBase   // 000
				{
				public:
					virtual ~BGSStoryManagerQuestNode() = default;

					virtual std::uint32_t AcceptVisitor(void* Visitor) noexcept(true) { return 0; } // 135

				public:
					std::uint8_t unk58[0x90]; // 58
				};
				static_assert(sizeof(BGSStoryManagerQuestNode) == 0xE8);
			}
		}
	}
}
