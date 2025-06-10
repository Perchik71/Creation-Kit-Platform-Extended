// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.UIGraphics.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace ListView
			{
				CKPE_COMMON_API HTHEME Initialize(HWND hWindow) noexcept(true);
				CKPE_COMMON_API LRESULT CALLBACK ListViewSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
					UINT_PTR uIdSubclass, DWORD_PTR dwRefData) noexcept(true);

				CKPE_COMMON_API void OnCustomDrawItemPlugins(HWND hWindow, LPDRAWITEMSTRUCT lpDrawItem) noexcept(true);
				CKPE_COMMON_API LRESULT OnCustomDraw(HWND hWindow, LPNMLVCUSTOMDRAW lpListView) noexcept(true);
			}
		}
	}
}