// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <uxtheme.h>
#include <CKPE.Common.UIBaseWindow.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.UIStatusBar.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace StatusBar
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawBorder(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						// 12px by height can safely paint over
						RECT rc = *pRect;
						rc.bottom = rc.top + 12;

						canvas.Fill(rc, GetThemeSysColor(ThemeColor::ThemeColor_StatusBar_Back));

						// 1px by height from bottom can safely paint over
						rc = *pRect;
						rc.top = rc.bottom - 1;

						canvas.Fill(rc, GetThemeSysColor(ThemeColor::ThemeColor_StatusBar_Back));

						// 17px needs to be painted over in the rightmost corner
						rc = *pRect;
						rc.left = rc.right - 17;

						canvas.Fill(rc, GetThemeSysColor(ThemeColor::ThemeColor_StatusBar_Back));
					}

					CKPE_COMMON_API void DrawBackground(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						canvas.Fill(*pRect, GetThemeSysColor(ThemeColor::ThemeColor_StatusBar_Back));
					}
				}

				namespace Event
				{
					CKPE_COMMON_API void OnBeforeDrawText(Canvas& canvas, std::uint32_t& flags) noexcept(true)
					{
						flags |= DT_CENTER | DT_END_ELLIPSIS;
						canvas.ColorText = GetThemeSysColor(ThemeColor_StatusBar_Text);
					}
				}

				namespace Func
				{
					CKPE_COMMON_API void AdjustHeightByTextHeight(HWND hWnd, HFONT hFont) noexcept(true)
					{
						SendMessageA(hWnd, WM_SETFONT, (WPARAM)hFont, FALSE);
						if (HDC hDC = GetDC(hWnd); hDC)
						{
							Canvas* Canvas = new CKPE::Canvas(hDC);
							if (Canvas)
							{
								SendMessageA(hWnd, SB_SETMINHEIGHT, Canvas->TextHeight("A") + 8, 0);
								SendMessageA(hWnd, WM_SIZE, 0, 0);
							}

							delete Canvas;
							ReleaseDC(hWnd, hDC);
						}
					}
				}
			}
		}
	}
}