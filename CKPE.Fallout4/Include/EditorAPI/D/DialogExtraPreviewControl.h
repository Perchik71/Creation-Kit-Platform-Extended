// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BSExtraData.h>
#include <EditorAPI/T/TESPreviewControl.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class DialogExtraPreviewControl : public BSExtraData
			{
			public:
				enum : std::uint8_t { kDialogExtraType = 0xE };

				/* 0x000 */ virtual ~DialogExtraPreviewControl();

				[[nodiscard]] inline TESPreviewControl* GetPreviewControl() const noexcept(true)
				{
					static_assert(offsetof(DialogExtraPreviewControl, _PreviewControl) == 0x18);
					return _PreviewControl;
				}

				[[nodiscard]] inline std::uint32_t GetControlID() const noexcept(true)
				{
					static_assert(offsetof(DialogExtraPreviewControl, _ControlID) == 0x20);
					return _ControlID;
				}

				CKPE_READ_PROPERTY(GetPreviewControl) TESPreviewControl* PreviewControl;
				CKPE_READ_PROPERTY(GetControlID) std::uint32_t ControlID;

			private:
				/* 0x018 */ TESPreviewControl* _PreviewControl;
				/* 0x020 */ std::uint32_t _ControlID;
				/* 0x024 */ char pad24[0x4];
			};
			static_assert(sizeof(DialogExtraPreviewControl) == 0x28);
		}
	}
}
