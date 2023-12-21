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
#include "CustomBox.h"
#include <CommCtrl.h>

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace CustomBox
			{
				namespace Render
				{
					VOID FIXAPI DrawBorder(Graphics::CUICanvas& canvas, Graphics::CRECT& rc)
					{
						Graphics::CRECT rc_temp = rc;
						canvas.GradientFill(rc_temp, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_End), Core::Classes::UI::gdVert);
						rc_temp.Inflate(-1, -1);
						canvas.Frame(rc_temp, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
					}
				}

				VOID FIXAPI Initialize(HWND hWindow, AllowBox eAllowBox)
				{
					switch (eAllowBox)
					{
					case Core::UI::Theme::CustomBox::abColor:
						SetWindowSubclass(hWindow, ColorBoxSubclass, 0, 0);
						break;
					default:
						SetWindowSubclass(hWindow, NormalBoxSubclass, 0, 0);
						break;
					}				
				}

				LRESULT CALLBACK NormalBoxSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
				{
					if (uMsg == WM_PAINT)
					{
						LRESULT result = DefSubclassProc(hWnd, uMsg, wParam, lParam);

						HDC hdc = GetWindowDC(hWnd);
						Core::Classes::UI::CUICanvas Canvas(hdc);
						Core::Classes::UI::CRECT rc;
						GetWindowRect(hWnd, (LPRECT)&rc);
						rc.Offset(-rc.Left, -rc.Top);

						rc.Inflate(-2, -2);
						Canvas.ExcludeRect(rc);
						rc.Inflate(2, 2);

						Render::DrawBorder(Canvas, rc);
						
						ReleaseDC(hWnd, hdc);
						return result;
					}

					return DefSubclassProc(hWnd, uMsg, wParam, lParam);
				}

				LRESULT CALLBACK ColorBoxSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
				{
					// Generally strange behavior, this window does't receive WM_PAINT, however, WM_NCPAINT is current in the path

					// WM_SIZE
					// WM_MOVE
					// WM_SHOWWINDOW
					// WM_SETFONT ??? why ???
					// WM_GETDLGCODE
					// WM_NCPAINT (wParam 1)
					// ...
					// WM_NCHITTEST (very-very many)
					// ...
					// WM_GETDLGCODE
					// WM_DESTROY
					// WM_NCDESTROY

					// there is't a single message that would tell me where it gets the color from
					// PS: the application itself draws on its client area without send message the control.
					// Let's draw it once taking into account the border of 3 pixels (Windows 10)

					if (uMsg == WM_NCPAINT)
					{
						LRESULT result = DefSubclassProc(hWnd, uMsg, wParam, lParam);

						HDC hdc = GetWindowDC(hWnd);
						Core::Classes::UI::CUICanvas Canvas(hdc);
						Core::Classes::UI::CRECT rc;
						GetWindowRect(hWnd, (LPRECT)&rc);
						rc.Offset(-rc.Left, -rc.Top);
						
						rc.Inflate(-3, -3);
						Canvas.ExcludeRect(rc);
						rc.Inflate(3, 3);

						Canvas.Frame(rc, GetThemeSysColor(ThemeColor::ThemeColor_Default));
						rc.Inflate(-1, -1);
						Render::DrawBorder(Canvas, rc);
						
						ReleaseDC(hWnd, hdc);
						return result;
					}

					return DefSubclassProc(hWnd, uMsg, wParam, lParam);
				}
			}
		}
	}
}