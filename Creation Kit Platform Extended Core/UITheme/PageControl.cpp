//////////////////////////////////////////
/*
* Copyright (c) 2022 Perchik71 <email:perchik71@outlook.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this
* software and associated documentation files (the "Software"), to deal in the Software
* without restriction, including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

#include "..\UIBaseWindow.h"
#include "VarCommon.h"
#include "PageControl.h"
#include <Uxtheme.h>
#include <vssym32.h>

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace PageControl
			{
				namespace Render
				{
					VOID FIXAPI DrawBorder(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						canvas.Frame(*pRect, GetThemeSysColor(ThemeColor_Divider_Highlighter_Ver2));
						Graphics::CRECT rc = *pRect;
						rc.Height = 2;
						canvas.Fill(rc, GetThemeSysColor(ThemeColor_Border_Window));
					}

					VOID FIXAPI DrawButton_Stylesheet(Graphics::CUICanvas& canvas, LPCRECT pRect, COLORREF cColor)
					{
						canvas.Fill(*pRect, cColor);
					}

					VOID FIXAPI DrawButton_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawButton_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor_Default));
					}

					VOID FIXAPI DrawButton_SelectedAndFocused(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawButton_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor_Border_Window));
					}

					VOID FIXAPI DrawButton_Hot(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawButton_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor_Divider_Highlighter_Ver2));
					}

					VOID FIXAPI DrawButton_Disabled(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						DrawButton_Stylesheet(canvas, pRect, GetThemeSysColor(ThemeColor_Divider_Color_Disabled_Ver2));
					}
				}

				namespace Event
				{
					VOID FIXAPI OnBeforeDrawText(Graphics::CUICanvas& canvas, DWORD& flags)
					{
						flags |= DT_END_ELLIPSIS;
						canvas.ColorText = GetThemeSysColor(ThemeColor_Caption_Text);
					}
				}

				VOID FIXAPI Initialize(HWND hWnd)
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