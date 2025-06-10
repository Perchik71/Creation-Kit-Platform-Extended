// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.UIBaseWindow.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.UITreeView.h>
#include <vssym32.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace TreeView
			{
				static CRECT rc, rc2;
				static CKPE::Canvas Canvas(nullptr);

				CKPE_COMMON_API HTHEME Initialize(HWND hWindow) noexcept(true)
				{
					SetWindowSubclass(hWindow, TreeViewSubclass, 0, 0);

					TreeView_SetTextColor(hWindow, GetThemeSysColor(ThemeColor::ThemeColor_Text_4));
					TreeView_SetBkColor(hWindow, GetThemeSysColor(ThemeColor::ThemeColor_TreeView_Color));

					auto StyleEx = TreeView_GetExtendedStyle(hWindow);
					if ((StyleEx & TVS_EX_DOUBLEBUFFER) != TVS_EX_DOUBLEBUFFER)
					{
						// Eliminate the flicker when changing size trees
						StyleEx |= TVS_EX_DOUBLEBUFFER;
						TreeView_SetExtendedStyle(hWindow, StyleEx, StyleEx);
					}

					return OpenThemeData(hWindow, VSCLASS_SCROLLBAR);
				}

				CKPE_COMMON_API LRESULT CALLBACK TreeViewSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
					UINT_PTR uIdSubclass, DWORD_PTR dwRefData) noexcept(true)
				{
					if ((uMsg == WM_SETFOCUS) || (uMsg == WM_KILLFOCUS))
					{
						InvalidateRect(hWnd, NULL, TRUE);
						UpdateWindow(hWnd);
					}
					else if (uMsg == WM_PAINT)
					{
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

					return DefSubclassProc(hWnd, uMsg, wParam, lParam);
				}

				CKPE_COMMON_API LRESULT OnCustomDraw(HWND hWindow, LPNMLVCUSTOMDRAW lpTreeView) noexcept(true)
				{
					switch (lpTreeView->nmcd.dwDrawStage)
					{
						//Before the paint cycle begins
					case CDDS_PREPAINT:
						//request notifications for individual treeview items
						return CDRF_NOTIFYITEMDRAW;
						//Before an item is drawn
					case CDDS_ITEMPREPAINT:
						return CDRF_NOTIFYSUBITEMDRAW;
						//Before a subitem is drawn
					case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
					{
						lpTreeView->clrText = GetThemeSysColor(ThemeColor_Text_4);
						return CDRF_NEWFONT;
					}
					default:
						return CDRF_DODEFAULT;
					}
				}
			}
		}
	}
}