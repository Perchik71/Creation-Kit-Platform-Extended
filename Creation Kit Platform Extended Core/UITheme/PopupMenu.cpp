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
#include "PopupMenu.h"
#include "VarCommon.h"
#include <CommCtrl.h>

namespace Core
{
	namespace UI
	{
		namespace Theme
		{
			namespace PopupMenu
			{
				static std::unordered_map<HWND, HMENU> globalStoragePopupMenu;

				namespace Render
				{
					namespace Themes = Core::UI::Theme;
					constexpr UINT generalCheckSize = 16;

					VOID FIXAPI DrawBackground_NonClientArray(Graphics::CUICanvas& canvas)
					{
						Graphics::CRECT rc[4];

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

						canvas.Fill(rc, 4, Themes::GetThemeSysColor(Themes::ThemeColor_Default));

						// border
						canvas.Frame(Themes::GetThemeSysColor(Themes::ThemeColor_Divider_Color));
					}

					VOID FIXAPI DrawItem_Normal(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						canvas.Fill(*pRect, Themes::GetThemeSysColor(Themes::ThemeColor_Default));
					}

					VOID FIXAPI DrawItem_Focused(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						canvas.Fill(*pRect, Themes::GetThemeSysColor(Themes::ThemeColor_Divider_Highlighter_Pressed));
					}

					VOID FIXAPI DrawItem_Divider(Graphics::CUICanvas& canvas, LPCRECT pRect)
					{
						Graphics::CRECT rc_temp = *pRect;
						canvas.Fill(rc_temp, Themes::GetThemeSysColor(Themes::ThemeColor_Default));
						rc_temp.Offset(0, 1);
						canvas.Pen.Color = Themes::GetThemeSysColor(Themes::ThemeColor_Divider_Color_Ver2);
						canvas.MoveTo(rc_temp.Left, rc_temp.Top);
						canvas.LineTo(rc_temp.Right, rc_temp.Top);
						rc_temp.Offset(0, 1);
						canvas.Pen.Color = Themes::GetThemeSysColor(Themes::ThemeColor_Divider_Highlighter_Ver2);
						canvas.MoveTo(rc_temp.Left, rc_temp.Top);
						canvas.LineTo(rc_temp.Right, rc_temp.Top);
					}

					VOID FIXAPI DrawItem_Checkbox(Core::Classes::UI::CUICanvas& canvas, LPCRECT pRect, BOOL bSelected, BOOL bDisabled)
					{
						Graphics::CRECT rc_temp = *pRect;

						rc_temp.Width = generalCheckSize;
						rc_temp.Height = generalCheckSize;
						rc_temp.Top = (((pRect->bottom - pRect->top) - generalCheckSize) >> 1) + pRect->top;

						COLORREF c_s, c_c;
						if (!bSelected && !bDisabled)
						{
							c_s = Themes::GetThemeSysColor(Themes::ThemeColor_Divider_Highlighter_Pressed);
							c_c = Themes::GetThemeSysColor(Themes::ThemeColor_Default);
						}
						else
						{
							c_s = Themes::GetThemeSysColor(Themes::ThemeColor_Shape);
							c_c = Themes::GetThemeSysColor(Themes::ThemeColor_Divider_Highlighter_Pressed);
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

					VOID FIXAPI DrawItem_Arrow(Core::Classes::UI::CUICanvas& canvas, LPCRECT pRect, BOOL bSelected)
					{
						Graphics::CRECT rc_temp(*pRect);

						std::vector<POINT> p;

						p.resize(3);
						p[0] = { 11 + rc_temp.Left, 7 + rc_temp.Top };
						p[1] = { 14 + rc_temp.Left, 10 + rc_temp.Top };
						p[2] = { 11 + rc_temp.Left, 13 + rc_temp.Top };

						if (bSelected)
						{
							canvas.Pen.Color = Themes::GetThemeSysColor(Themes::ThemeColor_Shape_Pressed);
							canvas.Brush.Color = Themes::GetThemeSysColor(Themes::ThemeColor_Shape_Pressed);
						}
						else
						{
							canvas.Pen.Color = Themes::GetThemeSysColor(Themes::ThemeColor_Shape);
							canvas.Brush.Color = Themes::GetThemeSysColor(Themes::ThemeColor_Shape);
						}

						canvas.Polygon(p);
					}
				}

				VOID FIXAPI Initialize(HWND hWindow)
				{
					SetWindowSubclass(hWindow, PopupMenuSubclass, 0, 0);
				}

				BOOL FIXAPI IsSystemPopupMenu(HWND hWindow, HMENU hMenu)
				{
					return hMenu && (GetSystemMenu(hWindow, FALSE) == hMenu);
				}

				BOOL FIXAPI IsSystemPopupMenuBlindly(HWND hWindow)
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

				LRESULT CALLBACK PopupMenuSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
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
					VOID FIXAPI OnDrawNoClientPopupMenu(HWND hWindow, HDC hDC)
					{
						Graphics::CUICanvas Canvas(hDC);
						Render::DrawBackground_NonClientArray(Canvas);
					}

					VOID FIXAPI OnInitPopupMenu(HWND hWindow, HMENU hMenu)
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
							Core::Classes::UI::CUIMenu Menu = hMenu;
							UINT itemCount = Menu.Count();

							for (UINT item = 0; item < itemCount; item++)
							{
								Graphics::CUIMenuItem menuItem = Menu.GetItemByPos(item);
								// make sure we do not change the state of the menu items as
								// we set the owner drawn style
								menuItem.OwnerDraw = TRUE;
								// set userdata
								menuItem.Tag = (LONG_PTR)hMenu;
							}
						}
					}

					VOID FIXAPI OnMeasureItem(HWND hWindow, LPMEASUREITEMSTRUCT lpMeasureItem)
					{
						Graphics::CUIMenu Menu = (HMENU)lpMeasureItem->itemData;
						Graphics::CUIMenuItem menuItem = Menu.GetItem(lpMeasureItem->itemID);

						if (menuItem.IsSeparate())
						{
							lpMeasureItem->itemWidth = 65;
							lpMeasureItem->itemHeight = 4;
						}
						else
						{
							std::string text = menuItem.Text;

							Graphics::CUICanvas canvas = GetDC(hWindow);
							canvas.Font.Assign(*Core::UI::Theme::ThemeFont);

							INT width, height;
							UINT nEdgeWidth = GetSystemMetrics(SM_CXEDGE);

							canvas.TextSize(text.c_str(), width, height);

							lpMeasureItem->itemWidth = std::max(width + Render::generalCheckSize + (nEdgeWidth << 1) + 4, (UINT)65);
							lpMeasureItem->itemHeight = std::max((UINT)height, Render::generalCheckSize);
						}
					}

					VOID FIXAPI OnDrawItem(HWND hWindow, LPDRAWITEMSTRUCT lpDrawItem)
					{
						Graphics::CUIMenu Menu = (HMENU)lpDrawItem->itemData;
						Graphics::CUIMenuItem menuItem = Menu.GetItem(lpDrawItem->itemID);
						Graphics::CUICanvas canvas = lpDrawItem->hDC;
						Graphics::CRECT rc = lpDrawItem->rcItem, rc_shortcut = lpDrawItem->rcItem;

						if (!menuItem.IsSeparate())
						{
							SetBkMode(lpDrawItem->hDC, TRANSPARENT);
							canvas.Font.Assign(*Core::UI::Theme::ThemeFont);
							COLORREF clrPrevText;

							BOOL bDisabled = (lpDrawItem->itemState & ODS_DISABLED) == ODS_DISABLED;
							BOOL bSelected = (lpDrawItem->itemState & ODS_SELECTED) == ODS_SELECTED;

							if (bSelected && !bDisabled)
							{
								Render::DrawItem_Focused(canvas, (LPRECT)&rc);
								clrPrevText = SetTextColor(lpDrawItem->hDC, Core::UI::Theme::GetThemeSysColor(ThemeColor::ThemeColor_Text_4));
							}
							else
							{
								Render::DrawItem_Normal(canvas, (LPRECT)&rc);
								clrPrevText = SetTextColor(lpDrawItem->hDC, Core::UI::Theme::GetThemeSysColor(ThemeColor::ThemeColor_Text_3));
							}

							if ((lpDrawItem->itemState & ODS_CHECKED) == ODS_CHECKED)
								Render::DrawItem_Checkbox(canvas, (LPRECT)&rc, bSelected, bDisabled);

							if (bDisabled)
								canvas.Font.Styles = { Core::Classes::UI::fsStrikeOut };

							std::string text = menuItem.Text;
							canvas.TextRect(rc, text.c_str(), DT_CALCRECT | DT_HIDEPREFIX);
							rc.Left += Render::generalCheckSize + 4;
							rc.Top += ((lpDrawItem->rcItem.bottom - lpDrawItem->rcItem.top) - rc.Height) >> 1;
							canvas.TextRect(rc, text.c_str(), DT_VCENTER | DT_HIDEPREFIX);

							text = menuItem.ShortCut;
							if (text.length())
							{
								rc_shortcut.Top = rc.Top;
								rc_shortcut.Right -= 4;

								canvas.TextRect(rc_shortcut, text.c_str(), DT_VCENTER | DT_HIDEPREFIX | DT_RIGHT);
							}

							if (bDisabled)
								canvas.Font.Styles = {};

							SetTextColor(lpDrawItem->hDC, clrPrevText);

							if (menuItem.IsSubMenu())
							{
								Core::Classes::UI::CRECT rc_arrow = lpDrawItem->rcItem, rc_exclude;
								rc_arrow.Left += rc_arrow.Width - 20;
								rc_exclude = rc_arrow;
								rc_arrow.Top += ((lpDrawItem->rcItem.bottom - lpDrawItem->rcItem.top) - 20) >> 1;

								Render::DrawItem_Arrow(canvas, (LPRECT)& rc_arrow, bSelected);

								// Excluding the area for drawing the arrow
								ExcludeClipRect(lpDrawItem->hDC, rc_exclude.Left, rc_exclude.Top, rc_exclude.Right, rc_exclude.Bottom);
							}
						}
						else
							Render::DrawItem_Divider(canvas, (LPRECT)& rc);
					}
				}
			}
		}
	}
}