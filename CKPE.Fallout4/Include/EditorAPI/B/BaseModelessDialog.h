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
				class BaseModelessDialog : public BaseDialog
				{
				public:
					/* 0x000 */ ~BaseModelessDialog() override;
				};
				static_assert(sizeof(BaseModelessDialog) == 0x18);
			}
		}
	}
}
