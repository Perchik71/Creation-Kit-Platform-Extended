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
				// 1.11.246.0: size 0x70, vtable RVA 0x3014568, 136 virtual functions
				class BGSStoryManagerBranchNode :
					public BGSStoryManagerNodeBase   // 000
				{
				public:
					virtual ~BGSStoryManagerBranchNode() = default;

					constexpr static std::uint8_t TYPE_ID = ftStoryManagerBranchNode;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSStoryManagerBranchNode[0];

					virtual std::uint32_t AcceptVisitor(void* Visitor) noexcept(true) { return 0; } // 135
				public:
					BSTArray<BGSStoryManagerNodeBase*> children; // 58
				};
				static_assert(sizeof(BGSStoryManagerBranchNode) == 0x70);
				static_assert(offsetof(BGSStoryManagerBranchNode, children) == 0x58);
			}
		}
	}
}
