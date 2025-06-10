// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <windows.h>
#include <CKPE.Graphics.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace PopupMenu
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawBackground_NonClientArray(Canvas& canvas) noexcept(true);
					CKPE_COMMON_API void DrawItem_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawItem_Focused(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawItem_Divider(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawItem_Checkbox(Canvas& canvas, LPCRECT pRect, bool bSelected, 
						bool bDisabled) noexcept(true);
					CKPE_COMMON_API void DrawItem_Arrow(Canvas& canvas, LPCRECT pRect, 
						bool bSelected) noexcept(true);
				}

				[[nodiscard]] CKPE_COMMON_API bool IsSystemPopupMenu(HWND hWindow, HMENU hMenu) noexcept(true);
				[[nodiscard]] CKPE_COMMON_API bool IsSystemPopupMenuBlindly(HWND hWindow) noexcept(true);

				CKPE_COMMON_API void Initialize(HWND hWindow) noexcept(true);
				CKPE_COMMON_API LRESULT CALLBACK PopupMenuSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
					UINT_PTR uIdSubclass, DWORD_PTR dwRefData) noexcept(true);

				namespace Event
				{
					CKPE_COMMON_API void OnInitPopupMenu(HWND hWindow, HMENU hMenu) noexcept(true);
					CKPE_COMMON_API void OnDrawNoClientPopupMenu(HWND hWindow, HDC hDC) noexcept(true);
					CKPE_COMMON_API void OnMeasureItem(HWND hWindow, LPMEASUREITEMSTRUCT lpMeasureItem) noexcept(true);
					CKPE_COMMON_API void OnDrawItem(HWND hWindow, LPDRAWITEMSTRUCT lpDrawItem) noexcept(true);
				}
			}
		}
	}
}