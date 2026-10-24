// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BaseModelessDialog.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Dialog
			{
				class ModelessDialog : public BaseModelessDialog
				{
				public:
					/* 0x000 */ ~ModelessDialog() override;
					/* 0x010 */ std::int32_t DlgProc(UINT Message, WPARAM wParam,
						LPARAM lParam) noexcept(true) override;
				};
				static_assert(sizeof(ModelessDialog) == 0x18);
			}
		}
	}
}
