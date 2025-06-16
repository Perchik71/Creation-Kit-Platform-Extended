// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.UIBaseWindow.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.UIMemo.h>
#include <vssym32.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace Memo
			{
				static CRECT rc, rc2;
				static Canvas canvas(nullptr);

				CKPE_COMMON_API HTHEME Initialize(HWND hWindow) noexcept(true)
				{
					SetWindowSubclass(hWindow, MemoSubclass, 0, 0);
					return OpenThemeData(hWindow, VSCLASS_SCROLLBAR);
				}

				CKPE_COMMON_API LRESULT CALLBACK MemoSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, 
					UINT_PTR uIdSubclass, DWORD_PTR dwRefData) noexcept(true)
				{
					if ((uMsg == WM_SETFOCUS) || (uMsg == WM_KILLFOCUS))
					{
						InvalidateRect(hWnd, NULL, TRUE);
						UpdateWindow(hWnd);
					}
					else if (uMsg == WM_SETFONT) 
					{
						// I noticed that the font does not change, so we will change it forcibly, without letting anyone change it.
						wParam = (WPARAM)ThemeData::GetSingleton()->ThemeFont->Handle;
					}
					else if (uMsg == WM_NCPAINT)
					{
						// Paint border
						LRESULT result = DefSubclassProc(hWnd, uMsg, wParam, lParam);

						HDC hdc = GetWindowDC(hWnd);
						*(HDC*)(((uintptr_t)&canvas + 0x8)) = hdc;

						if (GetWindowRect(hWnd, (LPRECT)&rc))
						{
							rc.Offset(-rc.Left, -rc.Top);

							if (GetFocus() == hWnd)
								canvas.Frame(rc, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Pressed));
							else
								canvas.Frame(rc, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_End));

							rc.Inflate(-1, -1);
							canvas.Frame(rc, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
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

								canvas.Fill(rc, GetThemeSysColor(ThemeColor::ThemeColor_Default));
							}
						}

						ReleaseDC(hWnd, hdc);
						return result;
					}
					else if ((uMsg == WM_PAINT) && !IsWindowEnabled(hWnd))
					{
						PAINTSTRUCT ps;
						HDC hDC = BeginPaint(hWnd, &ps);

						Canvas canvas(hDC);
						CRECT rc;
						GetWindowRect(hWnd, (LPRECT)&rc);
						rc.Offset(rc.Left, rc.Top);
						// skip border
						rc.Inflate(-2, -2);
						canvas.Fill(rc, GetThemeSysColor(ThemeColor::ThemeColor_Edit_Color_Disabled));

						EndPaint(hWnd, &ps);
					}

					return DefSubclassProc(hWnd, uMsg, wParam, lParam);
				}
			}
		}
	}
}