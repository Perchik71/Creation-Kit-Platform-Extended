// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <uxtheme.h>
#include <vssym32.h>
#include <CKPE.Common.UIBaseWindow.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.UIProgressBar.h>

#define INTERNAL 5
#define MARQUEE_TIMER_ID (WM_USER + 0x1000)

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace ProgressBar
			{
				typedef struct ProgressBarInfo
				{
					HWND hWnd;
					CRECT rc_fill;
				} *lpProgressBarInfo;

				namespace Render
				{
					CKPE_COMMON_API void DrawBar(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						CRECT rc = *pRect;

						if (GetTheme() != Theme::Theme_NightBlue)
						{
							canvas.GradientFill(rc, GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_Start),
								GetThemeSysColor(ThemeColor::ThemeColor_Button_Pressed_Gradient_End),
								gdVert);

							canvas.Frame(rc, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color));
						}
						else
						{
							canvas.Fill(rc, GetThemeSysColor(ThemeColor::ThemeColor_Edit_Color));
							canvas.Frame(rc, GetThemeSysColor(ThemeColor::ThemeColor_Divider_Color_Ver2));
						}
					}

					CKPE_COMMON_API void DrawFill(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						CRECT rc = *pRect;
						rc.Inflate(-1, -1);

						if (rc.Right > 0)
						{
							canvas.GradientFill(rc, GetThemeSysColor(ThemeColor::ThemeColor_Progress_Fill_Gradient_Start),
								GetThemeSysColor(ThemeColor::ThemeColor_Progress_Fill_Gradient_End),
								gdVert);

							canvas.Frame(rc, GetThemeSysColor(ThemeColor::ThemeColor_Progress_Fill_Highlighter));
							canvas.Pen.Color = GetThemeSysColor(ThemeColor::ThemeColor_Progress_Fill_Highlighter_Up);
							canvas.MoveTo(rc.Left, rc.Top);
							canvas.LineTo(rc.Right, rc.Top);
						}
					}
				}

				CKPE_COMMON_API void Initialize(HWND hWindow) noexcept(true)
				{
					// for marquee style
					SetWindowSubclass(hWindow, ProgressBarSubclass, 0, 0);
					// for standart progressbar (marquee does't draw)
					OpenThemeData(hWindow, VSCLASS_PROGRESS);
				}

				CKPE_COMMON_API LRESULT CALLBACK ProgressBarSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
					UINT_PTR uIdSubclass, DWORD_PTR dwRefData) noexcept(true)
				{
					if (uMsg == PBM_SETMARQUEE)
					{
						if (wParam)
						{
							lpProgressBarInfo pbi = new ProgressBarInfo;
							GetClientRect(hWnd, (LPRECT)&pbi->rc_fill);
							pbi->rc_fill.Width = pbi->rc_fill.Width >> 2;
							SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)pbi);
							SetTimer(hWnd, MARQUEE_TIMER_ID, std::max((UINT)30, (UINT)lParam), NULL);
						}
						else
						{
							KillTimer(hWnd, MARQUEE_TIMER_ID);
							delete (lpProgressBarInfo)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
							SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)NULL);
						}

						return S_OK;
					}
					else if (uMsg == WM_DESTROY)
					{
						KillTimer(hWnd, MARQUEE_TIMER_ID);
						delete (lpProgressBarInfo)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
					}
					else if (uMsg == WM_TIMER)
					{
						if (MARQUEE_TIMER_ID == wParam)
						{
							lpProgressBarInfo pbi = (lpProgressBarInfo)GetWindowLongPtrA(hWnd, GWLP_USERDATA);

							CUIBaseWindow window = hWnd;
							CRECT rc = window.ClientRect(), need_redraw;
							rc.Inflate(-1, -1);

							need_redraw = pbi->rc_fill;
							need_redraw.Inflate(INTERNAL, 0);

							pbi->rc_fill.Left += INTERNAL;
							if ((pbi->rc_fill.Left) >= (rc.Right + pbi->rc_fill.Width))
								pbi->rc_fill.Left = -pbi->rc_fill.Width;

							// to make it flicker less, redraw what you need
							InvalidateRect(hWnd, (LPRECT)&need_redraw, TRUE);
							UpdateWindow(hWnd);

							return S_OK;
						}
					}
					else if (uMsg == WM_PAINT)
					{
						lpProgressBarInfo pbi = (lpProgressBarInfo)GetWindowLongPtrA(hWnd, GWLP_USERDATA);

						if (pbi)
						{
							PAINTSTRUCT ps;
							HDC hDC = BeginPaint(hWnd, &ps);
							Canvas canvas = hDC;
							CUIBaseWindow window = hWnd;
							CRECT rc = window.ClientRect();

							Render::DrawBar(canvas, (LPRECT)&rc);
							Render::DrawFill(canvas, (LPRECT)&pbi->rc_fill);

							EndPaint(hWnd, &ps);

							return S_OK;
						}
					}

					return DefSubclassProc(hWnd, uMsg, wParam, lParam);
				}
			}
		}
	}
}