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
				// 1.11.240.0: size 0x70, vtable RVA 0x3013568, 136 virtual functions
				class BGSStoryManagerBranchNode :
					public BGSStoryManagerNodeBase   // 000
				{
				public:
					virtual ~BGSStoryManagerBranchNode() = default;

					virtual void sub_438() { return; } // 135 - RVA 0xFF1880, return (*(__int64 (__fastcall **)(__int64, __int64))(*(_QWORD *)a2 + 8LL))(a2, a1);

				public:
					BSTArray<void*> children; // 58 - BSTArray<BGSStoryManagerNodeBase*>
				};
				static_assert(sizeof(BGSStoryManagerBranchNode) == 0x70);
			}
		}
	}
}
