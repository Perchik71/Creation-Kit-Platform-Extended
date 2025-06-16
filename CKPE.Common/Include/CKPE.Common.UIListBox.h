// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <windows.h>
#include <uxtheme.h>
#include <CKPE.Graphics.h>
#include <CKPE.Common.Common.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace ListBox
			{
				CKPE_COMMON_API HTHEME Initialize(HWND hWindow) noexcept(true);
				CKPE_COMMON_API LRESULT CALLBACK ListBoxSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
					UINT_PTR uIdSubclass, DWORD_PTR dwRefData) noexcept(true);
				CKPE_COMMON_API std::ptrdiff_t OnCtlColorListBox(HWND hWindow, HDC hDC) noexcept(true);
			}
		}
	}
}