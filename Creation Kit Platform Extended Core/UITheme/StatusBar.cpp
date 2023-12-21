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
#include "StatusBar.h"

#include <CommCtrl.h>

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace StatusBar
			{
				namespace Render
				{
					VOID FIXAPI DrawBorder(Graphics::CUICanvas& canvas, LPCRECT pRect)
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

					VOID FIXAPI DrawBackground(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						canvas.Fill(*pRect, GetThemeSysColor(ThemeColor::ThemeColor_StatusBar_Back));
					}
				}

				namespace Event
				{
					VOID FIXAPI OnBeforeDrawText(Graphics::CUICanvas& canvas, DWORD& flags)
					{
						flags |= DT_CENTER | DT_END_ELLIPSIS;
						canvas.ColorText = GetThemeSysColor(ThemeColor_StatusBar_Text);
					}
				}

				namespace Func
				{
					VOID FIXAPI AdjustHeightByTextHeight(HWND hWnd, HFONT hFont)
					{
						SendMessageA(hWnd, WM_SETFONT, (WPARAM)hFont, FALSE);
						if (HDC hDC = GetDC(hWnd); hDC)
						{
							Graphics::CUICanvas* Canvas = new Graphics::CUICanvas(hDC);
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