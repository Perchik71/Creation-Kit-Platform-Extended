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
				// 1.11.240.0: size 0x28, vtable RVA 0x2FF2B30, 126 virtual functions
				class TESFormIDListView :
					public TESForm   // 000
				{
				public:
					virtual ~TESFormIDListView() = default;

					virtual void sub_3E0() { return; } // 124 - RVA 0x725DE0, return sub_14003195D(a2, a3);
					virtual void sub_3E8() { return; } // 125 - RVA 0x725E10
				};
				static_assert(sizeof(TESFormIDListView) == 0x28);
			}
		}
	}
}
