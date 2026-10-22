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
				// 1.11.246.0: size 0x28, vtable RVA 0x2FF3B30, 126 virtual functions
				class TESFormIDListView :
					public TESForm   // 000
				{
				public:
					virtual ~TESFormIDListView() = default;

					virtual void PopulateListView(HWND hListView, const char* Filter) const noexcept(true) { return; } // 124
					virtual void PopulateAndSortListView(HWND hListView) const noexcept(true) { return; } // 125
				};
				static_assert(sizeof(TESFormIDListView) == 0x28);
			}
		}
	}
}
