// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <Windows.h>
#include <CKPE.Common.h>
#include <EditorAPI/IDs_VTABLE.h>
#include <EditorAPI/B/BSSpinLock.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Dialog
			{
				class BaseDialog
				{
				public:
					/* 0x000 */ virtual ~BaseDialog();

					constexpr static Common::ID VTABLE = IDs::VTABLE::Dialog__BaseDialog[0];

					/* 0x008 */ virtual bool PreprocessMessage(UINT Message, WPARAM wParam,
						LPARAM lParam, std::int32_t* Result) noexcept(true);
					/* 0x010 */ virtual std::int32_t DlgProc(UINT Message, WPARAM wParam,
						LPARAM lParam) noexcept(true) = 0;

					[[nodiscard]] inline HWND GetWindowHandle() const noexcept(true)
					{
						static_assert(offsetof(BaseDialog, _WindowLock) == 0x8);
						static_assert(offsetof(BaseDialog, _WindowHandle) == 0x10);
						_WindowLock.Acquire();
						const auto Handle = _WindowHandle;
						_WindowLock.Release();
						return Handle;
					}

					CKPE_READ_PROPERTY(GetWindowHandle) HWND WindowHandle;

				private:
					/* 0x008 */ mutable BSSpinLock _WindowLock;
					/* 0x010 */ HWND _WindowHandle;
				};
				static_assert(sizeof(BaseDialog) == 0x18);
			}
		}
	}
}
