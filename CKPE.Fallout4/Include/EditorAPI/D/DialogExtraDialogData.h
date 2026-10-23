// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BSExtraData.h>
#include <EditorAPI/B/BaseDialog.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class DialogExtraDialogData : public BSExtraData
			{
			public:
				enum : std::uint8_t { kDialogExtraType = 0x10 };

				/* 0x000 */ virtual ~DialogExtraDialogData();

				[[nodiscard]] inline Dialog::BaseDialog* GetDialog() const noexcept(true)
				{
					static_assert(offsetof(DialogExtraDialogData, _Dialog) == 0x18);
					return _Dialog;
				}

				CKPE_READ_PROPERTY(GetDialog) Dialog::BaseDialog* DialogObject;

			private:
				/* 0x018 */ Dialog::BaseDialog* _Dialog;
			};
			static_assert(sizeof(DialogExtraDialogData) == 0x20);
		}
	}
}
