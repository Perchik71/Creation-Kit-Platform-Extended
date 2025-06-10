// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.UIBaseWindow.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.UIPageControl.h>
#include <windows.h>
#include <uxtheme.h>
#include <vssym32.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace PageControl
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawBorder(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						canvas.Frame(*pRect, GetThemeSysColor(ThemeColor_Divider_Highlighter_Ver2));
						CRECT rc = *pRect;
						rc.Height = 2;
						canvas.Fill(rc, GetThemeSysColor(ThemeColor_Border_Window));
					}

					CKPE_COMMON_API void DrawButton_Stylesheet(Canvas& canvas, LPCRECT pRect, COLORREF cColor) noexcept(true)
					{
						canvas.Fill(*pRect, cColor);
					}

					CKPE_COMMON_API void DrawButton_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawButton_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor_Default));
					}

					CKPE_COMMON_API void DrawButton_SelectedAndFocused(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawButton_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor_Border_Window));
					}

					CKPE_COMMON_API void DrawButton_Hot(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawButton_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor_Divider_Highlighter_Ver2));
					}

					CKPE_COMMON_API void DrawButton_Disabled(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						DrawButton_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor_Divider_Color_Disabled_Ver2));
					}
				}

				namespace Event
				{
					CKPE_COMMON_API void OnBeforeDrawText(Canvas& canvas, std::uint32_t& flags) noexcept(true)
					{
						flags |= DT_END_ELLIPSIS;
						canvas.ColorText = GetThemeSysColor(ThemeColor_Caption_Text);
					}
				}

				CKPE_COMMON_API void Initialize(HWND hWnd) noexcept(true)
				{
					LONG_PTR style = GetWindowLongPtrA(hWnd, GWL_STYLE);
					if ((style & TCS_BUTTONS) == TCS_BUTTONS)
						SetWindowLongPtrA(hWnd, GWL_STYLE, (style & ~TCS_BUTTONS) | TCS_TABS);

					OpenThemeData(hWnd, VSCLASS_TAB);
				}
			}
		}
	}
}