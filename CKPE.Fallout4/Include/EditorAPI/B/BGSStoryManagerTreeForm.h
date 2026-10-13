// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x28, vtable RVA 0x3012B70, 128 virtual functions
				class BGSStoryManagerTreeForm :
					public TESForm   // 000
				{
				public:
					virtual ~BGSStoryManagerTreeForm() = default;

					virtual void sub_3E0() { return; } // 124 - RVA 0x87BF30
					virtual void sub_3E8() { return; } // 125 - RVA 0x87E080
					virtual void sub_3F0() { return; } // 126 - RVA 0x87CF20, return 0;
					virtual void sub_3F8() { return; } // 127 - RVA 0x87BD10, return 0;
				};
				static_assert(sizeof(BGSStoryManagerTreeForm) == 0x28);
			}
		}
	}
}
