// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <commctrl.h>
#include <CKPE.Common.UIBaseWindow.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.UIPopupMenu.h>
#include <unordered_map>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace PopupMenu
			{
				static std::unordered_map<HWND, HMENU> globalStoragePopupMenu;

				namespace Render
				{
					constexpr static std::uint32_t generalCheckSize = 16;

					CKPE_COMMON_API void DrawBackground_NonClientArray(Canvas& canvas) noexcept(true)
					{
						CRECT rc[4];

						// paint space 

						rc[0] = canvas.GetClipRect();
						rc[1] = rc[0];
						rc[2] = rc[1];
						rc[3] = rc[2];

						// left rect
						rc[0].Width = GetSystemMetrics(SM_CXEDGE) + 1;
						// top rect
						rc[1].Height = GetSystemMetrics(SM_CYEDGE) + 1;
						// right rect
						rc[2].Left = rc[2].Width - rc[0].Width;
						rc[2].Width = rc[0].Width;
						// bottom rect
						rc[3].Top = rc[3].Height - rc[1].Height;
						rc[3].Height = rc[1].Height;

						canvas.Fill(rc, 4, GetThemeSysColor(ThemeColor_Default));

						// border
						canvas.Frame(GetThemeSysColor(ThemeColor_Divider_Color));
					}

					CKPE_COMMON_API void DrawItem_Normal(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						canvas.Fill(*pRect, GetThemeSysColor(ThemeColor_Default));
					}

					CKPE_COMMON_API void DrawItem_Focused(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						canvas.Fill(*pRect, GetThemeSysColor(ThemeColor_Divider_Highlighter_Pressed));
					}

					CKPE_COMMON_API void DrawItem_Divider(Canvas& canvas, LPCRECT pRect) noexcept(true)
					{
						CRECT rc_temp = *pRect;
						canvas.Fill(rc_temp, GetThemeSysColor(ThemeColor_Default));
						rc_temp.Offset(0, 1);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor_Divider_Color_Ver2);
						canvas.MoveTo(rc_temp.Left, rc_temp.Top);
						canvas.LineTo(rc_temp.Right, rc_temp.Top);
						rc_temp.Offset(0, 1);
						canvas.Pen.Color = GetThemeSysColor(ThemeColor_Divider_Highlighter_Ver2);
						canvas.MoveTo(rc_temp.Left, rc_temp.Top);
						canvas.LineTo(rc_temp.Right, rc_temp.Top);
					}

					CKPE_COMMON_API void DrawItem_Checkbox(Canvas& canvas, LPCRECT pRect, bool bSelected,
						bool bDisabled) noexcept(true)
					{
						CRECT rc_temp = *pRect;

						rc_temp.Width = generalCheckSize;
						rc_temp.Height = generalCheckSize;
						rc_temp.Top = (((pRect->bottom - pRect->top) - generalCheckSize) >> 1) + pRect->top;

						COLORREF c_s, c_c;
						if (!bSelected && !bDisabled)
						{
							c_s = GetThemeSysColor(ThemeColor_Divider_Highlighter_Pressed);
							c_c = GetThemeSysColor(ThemeColor_Default);
						}
						else
						{
							c_s = GetThemeSysColor(ThemeColor_Shape);
							c_c = GetThemeSysColor(ThemeColor_Divider_Highlighter_Pressed);
						}


						canvas.Fill(rc_temp, c_s);

						POINT p[6] = {
							{ 3 + rc_temp.Left, 7 + rc_temp.Top },
							{ 6 + rc_temp.Left, 10 + rc_temp.Top },
							{ 12 + rc_temp.Left, 4 + rc_temp.Top },
							{ 13 + rc_temp.Left, 5 + rc_temp.Top },
							{ 6 + rc_temp.Left, 12 + rc_temp.Top },
							{ 2 + rc_temp.Left, 8 + rc_temp.Top }
						};

						canvas.Brush.Color = c_c;
						canvas.Pen.Color = c_c;
						canvas.Polygon(p, 6);
					}

					CKPE_COMMON_API void DrawItem_Arrow(Canvas& canvas, LPCRECT pRect, bool bSelected) noexcept(true)
					{
						CRECT rc_temp(*pRect);

						std::vector<POINT> p;

						p.resize(3);
						p[0] = { 11 + rc_temp.Left, 7 + rc_temp.Top };
						p[1] = { 14 + rc_temp.Left, 10 + rc_temp.Top };
						p[2] = { 11 + rc_temp.Left, 13 + rc_temp.Top };

						if (bSelected)
						{
							canvas.Pen.Color = GetThemeSysColor(ThemeColor_Shape_Pressed);
							canvas.Brush.Color = GetThemeSysColor(ThemeColor_Shape_Pressed);
						}
						else
						{
							canvas.Pen.Color = GetThemeSysColor(ThemeColor_Shape);
							canvas.Brush.Color = GetThemeSysColor(ThemeColor_Shape);
						}

						canvas.Polygon(p);
					}
				}

				CKPE_COMMON_API void Initialize(HWND hWindow) noexcept(true)
				{
					SetWindowSubclass(hWindow, PopupMenuSubclass, 0, 0);
				}

				CKPE_COMMON_API bool IsSystemPopupMenu(HWND hWindow, HMENU hMenu) noexcept(true)
				{
					return hMenu && (GetSystemMenu(hWindow, FALSE) == hMenu);
				}

				CKPE_COMMON_API bool IsSystemPopupMenuBlindly(HWND hWindow) noexcept(true)
				{
					HMENU hMenu = (HMENU)SendMessageA(hWindow, MN_GETHMENU, 0, 0);
					if (HWND hParent = GetParent(hWindow); hParent)
					{
						if (hMenu == GetSystemMenu(hParent, FALSE))
							return TRUE;
					}
					else
					{
						HWND hWindow = GetForegroundWindow();
						HWND hFocus = GetFocus();

						if ((hMenu == GetSystemMenu(hWindow, FALSE)) || (hMenu == GetSystemMenu(hFocus, FALSE)))
							return TRUE;
					}

					return FALSE;
				}

				CKPE_COMMON_API LRESULT CALLBACK PopupMenuSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
					UINT_PTR uIdSubclass, DWORD_PTR dwRefData) noexcept(true)
				{
					// Test: Windows 10 (2004)

					// The pop-up menu calls WM_PRINT message to draw a non-client area, however, 
					// it is triggered only the first time, then ignored. Therefore, 
					// will have to additionally process the WM_NCPAINT message.

					// Bug: If the menu is too large, the up and down arrows are displayed normally. 
					// Their rendering is handled by the OS. 
					// They are outside the client rendering area, in addition, periodically restored.
					// I haven't been able to solve this problem yet.

					HMENU hMenu = 0;

					// Let it work as it should
					LRESULT lResult = DefSubclassProc(hWnd, uMsg, wParam, lParam);

					switch (uMsg)
					{
					case WM_PRINT:
					{
						// If the border is drawn, redraw it
						if (((lParam & PRF_ERASEBKGND) == PRF_ERASEBKGND) &&
							((lParam & PRF_NONCLIENT) == PRF_NONCLIENT) &&
							((lParam & PRF_CHILDREN) != PRF_CHILDREN) &&
							((!IsSystemPopupMenuBlindly(hWnd))))
							Event::OnDrawNoClientPopupMenu(hWnd, (HDC)wParam);
					}
					break;

					case WM_NCPAINT:
					{
						if (!IsSystemPopupMenuBlindly(hWnd))
						{
							HDC hDC = GetWindowDC(hWnd);
							Event::OnDrawNoClientPopupMenu(hWnd, hDC);
							ReleaseDC(hWnd, hDC);
						}
					}
					break;

					}

					return lResult;
				}

				namespace Event
				{
					CKPE_COMMON_API void OnDrawNoClientPopupMenu(HWND hWindow, HDC hDC) noexcept(true)
					{
						Canvas Canvas(hDC);
						Render::DrawBackground_NonClientArray(Canvas);
					}

					CKPE_COMMON_API void OnInitPopupMenu(HWND hWindow, HMENU hMenu) noexcept(true)
					{
						// https://www.codeproject.com/Articles/8715/Owner-drawn-menus-in-two-lines-of-code

						// iterate any menu about to be displayed and make sure
						// all the items have the ownerdrawn style set
						// We receive a WM_INITMENUPOPUP as each menu is displayed, even if the user
						// switches menus or brings up a sub menu. This means we only need to
						// set the style at the current popup level.
						// we also set the user item data to the index into the menu to allow
						// us to measure/draw the item correctly later
						//
						if (hMenu && IsMenu(hMenu) && !IsSystemPopupMenu(hWindow, hMenu))
						{
							CUIMenu Menu = hMenu;
							UINT itemCount = Menu.Count();

							for (UINT item = 0; item < itemCount; item++)
							{
								CUIMenuItem menuItem = Menu.GetItemByPos(item);
								// make sure we do not change the state of the menu items as
								// we set the owner drawn style
								menuItem.OwnerDraw = TRUE;
								// set userdata
								menuItem.Tag = (LONG_PTR)hMenu;
							}
						}
					}

					CKPE_COMMON_API void OnMeasureItem(HWND hWindow, LPMEASUREITEMSTRUCT lpMeasureItem) noexcept(true)
					{
						CUIMenu Menu = (HMENU)lpMeasureItem->itemData;
						CUIMenuItem menuItem = Menu.GetItem(lpMeasureItem->itemID);

						if (menuItem.IsSeparate())
						{
							lpMeasureItem->itemWidth = 65;
							lpMeasureItem->itemHeight = 4;
						}
						else
						{
							std::string text = menuItem.GetText();

							Canvas canvas = GetDC(hWindow);
							canvas.Font.Assign(*(ThemeData::GetSingleton()->ThemeFont));

							INT width, height;
							UINT nEdgeWidth = GetSystemMetrics(SM_CXEDGE);

							canvas.TextSize(text.c_str(), width, height);

							lpMeasureItem->itemWidth = std::max(width + Render::generalCheckSize + (nEdgeWidth << 1) + 4, (UINT)65);
							lpMeasureItem->itemHeight = std::max((UINT)height, Render::generalCheckSize);
						}
					}

					CKPE_COMMON_API void OnDrawItem(HWND hWindow, LPDRAWITEMSTRUCT lpDrawItem) noexcept(true)
					{
						CUIMenu Menu = (HMENU)lpDrawItem->itemData;
						CUIMenuItem menuItem = Menu.GetItem(lpDrawItem->itemID);
						Canvas canvas = lpDrawItem->hDC;
						CRECT rc = lpDrawItem->rcItem, rc_shortcut = lpDrawItem->rcItem;

						if (!menuItem.IsSeparate())
						{
							SetBkMode(lpDrawItem->hDC, TRANSPARENT);
							canvas.Font.Assign(*(ThemeData::GetSingleton()->ThemeFont));
							COLORREF clrPrevText;

							BOOL bDisabled = (lpDrawItem->itemState & ODS_DISABLED) == ODS_DISABLED;
							BOOL bSelected = (lpDrawItem->itemState & ODS_SELECTED) == ODS_SELECTED;

							if (bSelected && !bDisabled)
							{
								Render::DrawItem_Focused(canvas, (LPRECT)&rc);
								clrPrevText = SetTextColor(lpDrawItem->hDC, GetThemeSysColor(ThemeColor::ThemeColor_Text_4));
							}
							else
							{
								Render::DrawItem_Normal(canvas, (LPRECT)&rc);
								clrPrevText = SetTextColor(lpDrawItem->hDC, GetThemeSysColor(ThemeColor::ThemeColor_Text_3));
							}

							if ((lpDrawItem->itemState & ODS_CHECKED) == ODS_CHECKED)
								Render::DrawItem_Checkbox(canvas, (LPRECT)&rc, bSelected, bDisabled);

							if (bDisabled)
								canvas.Font.SetStyles({ fsStrikeOut });

							std::string text = menuItem.GetText();
							canvas.TextRect(rc, text.c_str(), DT_CALCRECT | DT_HIDEPREFIX);
							rc.Left += Render::generalCheckSize + 4;
							rc.Top += ((lpDrawItem->rcItem.bottom - lpDrawItem->rcItem.top) - rc.Height) >> 1;
							canvas.TextRect(rc, text.c_str(), DT_VCENTER | DT_HIDEPREFIX);

							text = menuItem.GetShortCutText();
							if (text.length())
							{
								rc_shortcut.Top = rc.Top;
								rc_shortcut.Right -= 4;

								canvas.TextRect(rc_shortcut, text.c_str(), DT_VCENTER | DT_HIDEPREFIX | DT_RIGHT);
							}

							if (bDisabled)
								canvas.Font.SetStyles({});

							SetTextColor(lpDrawItem->hDC, clrPrevText);

							if (menuItem.IsSubMenu())
							{
								CRECT rc_arrow = lpDrawItem->rcItem, rc_exclude;
								rc_arrow.Left += rc_arrow.Width - 20;
								rc_exclude = rc_arrow;
								rc_arrow.Top += ((lpDrawItem->rcItem.bottom - lpDrawItem->rcItem.top) - 20) >> 1;

								Render::DrawItem_Arrow(canvas, (LPRECT)&rc_arrow, bSelected);

								// Excluding the area for drawing the arrow
								ExcludeClipRect(lpDrawItem->hDC, rc_exclude.Left, rc_exclude.Top, rc_exclude.Right, rc_exclude.Bottom);
							}
						}
						else
							Render::DrawItem_Divider(canvas, (LPRECT)&rc);
					}
				}
			}
		}
	}
}