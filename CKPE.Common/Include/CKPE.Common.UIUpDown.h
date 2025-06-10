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
			namespace UpDown
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawUp_Stylesheet(Canvas& canvas, LPCRECT pRect, COLORREF cColorBorder,
						COLORREF cColorStart, COLORREF cColorEnd, COLORREF cColor, COLORREF cShadowColor) noexcept(true);
					CKPE_COMMON_API void DrawUp_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawUp_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawUp_Pressed(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawUp_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawDown_Stylesheet(Canvas& canvas, LPCRECT pRect, COLORREF cColorBorder,
						COLORREF cColorStart, COLORREF cColorEnd, COLORREF cColor, COLORREF cShadowColor) noexcept(true);
					CKPE_COMMON_API void DrawDown_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawDown_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawDown_Pressed(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawDown_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true);

					CKPE_COMMON_API void DrawLeft_Stylesheet(Canvas& canvas, LPCRECT pRect, COLORREF cColorBorder,
						COLORREF cColorStart, COLORREF cColorEnd, COLORREF cColor, COLORREF cShadowColor,
						COLORREF cHihglighter, bool drawHihglighter) noexcept(true);
					CKPE_COMMON_API void DrawLeft_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawLeft_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawLeft_Pressed(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawLeft_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawRight_Stylesheet(Canvas& canvas, LPCRECT pRect, COLORREF cColorBorder,
						COLORREF cColorStart, COLORREF cColorEnd, COLORREF cColor, COLORREF cShadowColor,
						COLORREF cHihglighter, bool drawHihglighter) noexcept(true);
					CKPE_COMMON_API void DrawRight_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawRight_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawRight_Pressed(Canvas& canvas, LPCRECT pRect) noexcept(true);
					CKPE_COMMON_API void DrawRight_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true);
				}

				CKPE_COMMON_API void Initialize(HWND hWindow) noexcept(true);
				CKPE_COMMON_API LRESULT CALLBACK UpDownSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
					UINT_PTR uIdSubclass, DWORD_PTR dwRefData) noexcept(true);
			}
		}
	}
}