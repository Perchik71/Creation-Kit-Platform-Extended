// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESModel.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// 1.11.240.0: size 0x38, vtable RVA 0x304C7B8, 27 virtual functions
			class BGSBehaviorGraphModel :
				public TESModel   // 000
			{
			public:
				virtual ~BGSBehaviorGraphModel() = default;
				/* 70 */ bool Perform(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) override;
			};
			static_assert(sizeof(BGSBehaviorGraphModel) == 0x38);
		}
	}
}
