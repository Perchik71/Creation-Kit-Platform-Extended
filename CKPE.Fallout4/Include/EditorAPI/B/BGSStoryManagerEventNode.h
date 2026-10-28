// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BGSStoryManagerBranchNode.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSRegisteredStoryEvent;
				// 1.11.240.0: size 0x90, vtable RVA 0x310BCF0, 136 virtual functions
				class BGSStoryManagerEventNode :
					public BGSStoryManagerBranchNode   // 000
				{
				public:
					virtual ~BGSStoryManagerEventNode() = default;

					constexpr static std::uint8_t TYPE_ID = ftStoryManagerEventNode;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSStoryManagerEventNode[0];
				public:
					/* 0x070 */ std::uint8_t unk70[0x18];
					/* 0x088 */ const BGSRegisteredStoryEvent* event;
				};
				static_assert(sizeof(BGSStoryManagerEventNode) == 0x90);
				static_assert(offsetof(BGSStoryManagerEventNode, event) == 0x88);
			}
		}
	}
}
