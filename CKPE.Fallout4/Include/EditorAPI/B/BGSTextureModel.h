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
			// 1.11.240.0: size 0x38, vtable RVA 0x304C6A8, 27 virtual functions
			class BGSTextureModel :
				public TESModel   // 000
			{
			public:
				virtual ~BGSTextureModel() = default;
				/* 70 */ bool Perform(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) override;
				/* 80 */ void UpdateDataInDialogBox(HWND hDialogBox) const noexcept(true) override;

				constexpr static Common::ID VTABLE = IDs::VTABLE::BGSTextureModel[0];
			};
			static_assert(sizeof(BGSTextureModel) == 0x38);
		}
	}
}
