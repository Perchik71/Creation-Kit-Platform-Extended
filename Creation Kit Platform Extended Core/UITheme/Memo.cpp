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

#include "Editor API/UI/UIBaseWindow.h"
#include "VarCommon.h"
#include "Memo.h"

namespace CreationKitPlatformExtended
{
	namespace UITheme
	{
		namespace Memo
		{
			static std::recursive_mutex locker;
			static Graphics::CRECT rc, rc2;
			static Graphics::CUICanvas Canvas(nullptr);

			HTHEME Initialize(HWND hWindow)
			{
				SetWindowSubclass(hWindow, MemoSubclass, 0, 0);
				return OpenThemeData(hWindow, VSCLASS_SCROLLBAR);
			}

			LRESULT CALLBACK MemoSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
			{
				if ((uMsg == WM_SETFOCUS) || (uMsg == WM_KILLFOCUS))
				{
					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);
				}
				else if (uMsg == WM_SETFONT) {
					// I noticed that the font does not change, so we will change it forcibly, without letting anyone change it.

					wParam = (WPARAM)ThemeFont->Handle;
				}
				else if (uMsg == WM_NCPAINT)
				{
					//std::lock_guard lock(locker);

					// Paint border
					LRESULT result = DefSubclassProc(hWnd, uMsg, wParam, lParam);

					HDC hdc = GetWindowDC(hWnd);
					*(HDC*)(((uintptr_t)&Canvas + 0x8)) = hdc;

					if (GetWindowRect(hWnd, (LPRECT)&rc))
					{
						rc.Offset(-rc.Left, -rc.Top);

						if (GetFocus() == hWnd)
							Canvas.Frame(rc, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Pressed));
						else
							Canvas.Frame(rc, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_End));

						rc.Inflate(-1, -1);
						Canvas.Frame(rc, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
					}

					// scrollbox detected grip
					if (GetClientRect(hWnd, (LPRECT)&rc2))
					{
						if ((abs(rc2.Width - rc.Width) > 5) && (abs(rc2.Height - rc.Height) > 5))
						{
							rc.Left = rc.Width - GetSystemMetrics(SM_CXVSCROLL);
							rc.Top = rc.Height - GetSystemMetrics(SM_CYHSCROLL);
							rc.Width = GetSystemMetrics(SM_CXVSCROLL);
							rc.Height = GetSystemMetrics(SM_CYHSCROLL);

							Canvas.Fill(rc, GetThemeSysColor(ThemeColor::ThemeColor_Default));
						}
					}

					ReleaseDC(hWnd, hdc);
					return result;
				}
				else if ((uMsg == WM_PAINT) && !IsWindowEnabled(hWnd))
				{
					PAINTSTRUCT ps;
					HDC hDC = BeginPaint(hWnd, &ps);

					Graphics::CUICanvas Canvas(hDC);
					Graphics::CRECT rc;
					GetWindowRect(hWnd, (LPRECT)&rc);
					rc.Offset(rc.Left, rc.Top);
					// skip border
					rc.Inflate(-2, -2);
					Canvas.Fill(rc, GetThemeSysColor(ThemeColor::ThemeColor_Edit_Color_Disabled));

					EndPaint(hWnd, &ps);
				}

				return DefSubclassProc(hWnd, uMsg, wParam, lParam);
			}
		}
	}
}