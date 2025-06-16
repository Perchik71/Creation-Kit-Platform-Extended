// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.UIListView.h>
#include <CKPE.Common.UIVarCommon.h>
#include <commctrl.h>
#include <vssym32.h>

constexpr auto UI_CONTROL_CONDITION_ID = 0xFA0;
constexpr auto SIZEBUF = 1024;

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace ListView
			{
				static CRECT rc, rc2;
				static Canvas canvas(nullptr);
				static char FileName[SIZEBUF], FileType[SIZEBUF];
				static OnCustomDrawHandler sCustomDrawHandler = nullptr;
				static OnCustomDrawPluginsHandler sCustomDrawPluginsHandler = nullptr;

				CKPE_COMMON_API HTHEME Initialize(HWND hWindow) noexcept(true)
				{
					SetWindowSubclass(hWindow, ListViewSubclass, 0, 0);

					ListView_SetTextColor(hWindow, GetThemeSysColor(ThemeColor::ThemeColor_Text_4));
					ListView_SetTextBkColor(hWindow, GetThemeSysColor(ThemeColor::ThemeColor_ListView_Color));
					ListView_SetBkColor(hWindow, GetThemeSysColor(ThemeColor::ThemeColor_ListView_Color));

					return OpenThemeData(hWindow, VSCLASS_SCROLLBAR);
				}

				CKPE_COMMON_API LRESULT CALLBACK ListViewSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
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
						*(HDC*)(((std::uintptr_t)&canvas + 0x8)) = hdc;

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

					return DefSubclassProc(hWnd, uMsg, wParam, lParam);
				}

				CKPE_COMMON_API void OnCustomDrawItemPlugins(HWND hWindow, LPDRAWITEMSTRUCT lpDrawItem) noexcept(true)
				{
					// If there are no list view items, skip this message. 
					if (lpDrawItem->itemID == -1)
						return;

					RECT rc = lpDrawItem->rcItem;
					Canvas canvas(lpDrawItem->hDC);

					BOOL Selected = (lpDrawItem->itemState & ODS_SELECTED) == ODS_SELECTED;

					canvas.Fill(rc, GetThemeSysColor(ThemeColor::ThemeColor_ListView_Color));

					ListView_GetItemText(lpDrawItem->hwndItem, lpDrawItem->itemID, 0, FileName, SIZEBUF);
					ListView_GetItemText(lpDrawItem->hwndItem, lpDrawItem->itemID, 1, FileType, SIZEBUF);

					if (sCustomDrawPluginsHandler)
						sCustomDrawPluginsHandler(hWindow, lpDrawItem, FileName, FileType);

					// CHECKBOX

					int icon_off = 0;
					HIMAGELIST hImageList = ListView_GetImageList(lpDrawItem->hwndItem, LVSIL_SMALL);
					if (hImageList)
					{
						int cx, cy;
						ImageList_GetIconSize(hImageList, &cx, &cy);

						if ((rc.bottom - rc.top > cy) && (rc.right - rc.left > (cx + 8)))
						{
							icon_off = cx;
							cy = ((rc.bottom - rc.top) - cy) >> 1;

							LVITEMA lvi = { 0 };
							lvi.mask = LVIF_IMAGE;
							lvi.iItem = lpDrawItem->itemID;
							ListView_GetItem(lpDrawItem->hwndItem, &lvi);

							ImageList_Draw(hImageList, lvi.iImage, lpDrawItem->hDC, rc.left + 2, rc.top + cy, ILD_TRANSPARENT);
						}
					}

					// TEXT

					canvas.Font.Assign(*(ThemeData::GetSingleton()->ThemeFont));

					SetBkMode(lpDrawItem->hDC, TRANSPARENT);
					SetTextColor(lpDrawItem->hDC, GetThemeSysColor(ThemeColor::ThemeColor_Text_4));

					CRECT rcText;
					ListView_GetSubItemRect(lpDrawItem->hwndItem, lpDrawItem->itemID, 0, LVIR_LABEL, (LPRECT)&rcText);
					rcText.Inflate(-2, -2);
					rcText.Left += 2;

					canvas.TextRect(rcText, FileName, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);

					ListView_GetSubItemRect(lpDrawItem->hwndItem, lpDrawItem->itemID, 1, LVIR_LABEL, (LPRECT)&rcText);
					rcText.Inflate(-2, -2);
					rcText.Left += 2;

					canvas.TextRect(rcText, FileType, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);

					if (Selected)
						// blend 40%
						canvas.FillWithTransparent(rc, GetThemeSysColor(ThemeColor::ThemeColor_ListView_Owner_Selected), 40);
				}

				CKPE_COMMON_API LRESULT OnCustomDraw(HWND hWindow, LPNMLVCUSTOMDRAW lpListView) noexcept(true)
				{
					if (sCustomDrawHandler)
					{
						bool NeedReturned = false;
						auto Result = sCustomDrawHandler(hWindow, lpListView, NeedReturned);
						if (NeedReturned) return Result;
					}

					switch (lpListView->nmcd.dwDrawStage) 
					{
						//Before the paint cycle begins
					case CDDS_PREPAINT:
						//request notifications for individual listview items
						return CDRF_NOTIFYITEMDRAW;
						//Before an item is drawn
					case CDDS_ITEMPREPAINT:
						return CDRF_NOTIFYSUBITEMDRAW;
						//Before a subitem is drawn
					case CDDS_SUBITEM | CDDS_ITEMPREPAINT: 
					{
						switch (lpListView->nmcd.hdr.idFrom) 
						{
						case UI_CONTROL_CONDITION_ID: 
						{
							if (lpListView->iSubItem == 0 || lpListView->iSubItem == 5)
								lpListView->clrText = GetThemeSysColor(ThemeColor_Text_2);
							else
								lpListView->clrText = GetThemeSysColor(ThemeColor_Text_4);

							return CDRF_NEWFONT;
						}
						default:
							lpListView->clrText = GetThemeSysColor(ThemeColor_Text_4);
							return CDRF_NEWFONT;
						}
					}
					default:
						return CDRF_DODEFAULT;
					}
				}

				CKPE_COMMON_API void InstallCustomDrawHandler(OnCustomDrawHandler handler,
					OnCustomDrawPluginsHandler plugins_handler) noexcept(true)
				{
					sCustomDrawHandler = handler;
				}
			}
		}
	}
}