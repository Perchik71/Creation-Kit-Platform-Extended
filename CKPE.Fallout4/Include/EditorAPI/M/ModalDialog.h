// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BaseDialog.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Dialog
			{
				class ModalDialog : public BaseDialog
				{
				public:
					/* 0x000 */ ~ModalDialog() override;
					/* 0x008 */ bool PreprocessMessage(UINT Message, WPARAM wParam,
						LPARAM lParam, std::int32_t* Result) noexcept(true) override;
					/* 0x010 */ std::int32_t DlgProc(UINT Message, WPARAM wParam,
						LPARAM lParam) noexcept(true) override;
				};
				static_assert(sizeof(ModalDialog) == 0x18);
			}
		}
	}
}
