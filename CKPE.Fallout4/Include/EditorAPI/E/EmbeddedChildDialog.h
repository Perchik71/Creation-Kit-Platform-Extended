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
				class EmbeddedChildDialog : public BaseModelessDialog
				{
				public:
					/* 0x000 */ ~EmbeddedChildDialog() override;
					/* 0x010 */ std::int32_t DlgProc(UINT Message, WPARAM wParam,
						LPARAM lParam) noexcept(true) override;

					[[nodiscard]] inline HWND GetHostWindow() const noexcept(true)
					{
						static_assert(offsetof(EmbeddedChildDialog, _HostWindowLock) == 0x18);
						static_assert(offsetof(EmbeddedChildDialog, _HostWindow) == 0x20);
						_HostWindowLock.Acquire();
						const auto Handle = _HostWindow;
						_HostWindowLock.Release();
						return Handle;
					}

					CKPE_READ_PROPERTY(GetHostWindow) HWND HostWindow;

				private:
					/* 0x018 */ mutable BSSpinLock _HostWindowLock;
					/* 0x020 */ HWND _HostWindow;
					/* 0x028 */ char pad28[0x8];
				};
				static_assert(sizeof(EmbeddedChildDialog) == 0x30);
			}
		}
	}
}
