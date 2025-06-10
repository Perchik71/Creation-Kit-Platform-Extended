// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.UIBaseWindow.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.UICustomBox.h>
#include <commctrl.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace CustomBox
			{
				namespace Render
				{
					CKPE_COMMON_API void DrawBorder(Canvas& canvas, CRECT& rc)
					{
						CRECT rc_temp = rc;
						canvas.GradientFill(rc_temp, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_Start),
							GetThemeSysColor(ThemeColor::ThemeColor_Divider_Highlighter_Gradient_End), gdVert);
						rc_temp.Inflate(-1, -1);
						canvas.Frame(rc_temp, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
					}
				}

				CKPE_COMMON_API void Initialize(HWND hWindow, AllowBox eAllowBox)
				{
					switch (eAllowBox)
					{
					case abColor:
						SetWindowSubclass(hWindow, ColorBoxSubclass, 0, 0);
						break;
					default:
						SetWindowSubclass(hWindow, NormalBoxSubclass, 0, 0);
						break;
					}
				}

				CKPE_COMMON_API LRESULT CALLBACK NormalBoxSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, 
					UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
				{
					if (uMsg == WM_PAINT)
					{
						LRESULT result = DefSubclassProc(hWnd, uMsg, wParam, lParam);

						HDC hdc = GetWindowDC(hWnd);
						Canvas Canvas(hdc);
						CRECT rc;
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

				CKPE_COMMON_API LRESULT CALLBACK ColorBoxSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, 
					UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
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
						Canvas Canvas(hdc);
						CRECT rc;
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