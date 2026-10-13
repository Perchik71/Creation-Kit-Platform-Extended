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
				// 1.11.240.0: size 0x90, vtable RVA 0x310BCF0, 136 virtual functions
				class BGSStoryManagerEventNode :
					public BGSStoryManagerBranchNode   // 000
				{
				public:
					virtual ~BGSStoryManagerEventNode() = default;

				public:
					std::uint8_t unk70[0x20]; // 70
				};
				static_assert(sizeof(BGSStoryManagerEventNode) == 0x90);
			}
		}
	}
}
