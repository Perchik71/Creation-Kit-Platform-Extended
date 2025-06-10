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

#include "Core/Engine.h"
#include "Editor API/UI/UIBaseWindow.h"
#include "VarCommon.h"
#include "ListView.h"
#include "Editor API/BSString.h"
#include "Editor API/EditorUI.h"
#include "Editor API/SSE/TESFile.h"
#include "Editor API/SF/TESFileSF.h"
#include "Editor API/SF/TESObjectLIGH.h"

#ifdef _CKPE_WITH_QT5
#	include "Editor API/SF/TESObjectREFR.h"
#else
#	include "Editor API/SSE/TESObjectLIGH.h"
#	include "Editor API/FO4/TESObjectLIGH.h"
#endif // !_CKPE_WITH_QT5

#define UI_CONTROL_CONDITION_ID 0xFA0
#define SIZEBUF 1024

namespace CreationKitPlatformExtended
{
	namespace UITheme
	{
		namespace ListView
		{
			static std::recursive_mutex locker;
			static Graphics::CRECT rc, rc2;
			static Graphics::CUICanvas Canvas(nullptr);

			HTHEME Initialize(HWND hWindow)
			{
				SetWindowSubclass(hWindow, ListViewSubclass, 0, 0);

				ListView_SetTextColor(hWindow, GetThemeSysColor(ThemeColor::ThemeColor_Text_4));
				ListView_SetTextBkColor(hWindow, GetThemeSysColor(ThemeColor::ThemeColor_ListView_Color));
				ListView_SetBkColor(hWindow, GetThemeSysColor(ThemeColor::ThemeColor_ListView_Color));

				return OpenThemeData(hWindow, VSCLASS_SCROLLBAR);
			}

			LRESULT CALLBACK ListViewSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
				UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
			{
				if ((uMsg == WM_SETFOCUS) || (uMsg == WM_KILLFOCUS))
				{
					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);
				}
				else if (uMsg == WM_PAINT)
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

				return DefSubclassProc(hWnd, uMsg, wParam, lParam);
			}

			VOID OnCustomDrawItemPlugins(HWND hWindow, LPDRAWITEMSTRUCT lpDrawItem)
			{
				// If there are no list view items, skip this message. 
				if (lpDrawItem->itemID == -1)
					return;

				RECT rc = lpDrawItem->rcItem;
				Graphics::CUICanvas Canvas(lpDrawItem->hDC);

				BOOL Selected = (lpDrawItem->itemState & ODS_SELECTED) == ODS_SELECTED;

				Canvas.Fill(rc, GetThemeSysColor(ThemeColor::ThemeColor_ListView_Color));

				EditorAPI::BSString FileName;
				EditorAPI::BSString FileType;

				if (!FileName.Reserved(SIZEBUF) || !FileType.Reserved(SIZEBUF))
					return;

				ListView_GetItemText(lpDrawItem->hwndItem, lpDrawItem->itemID, 0, const_cast<LPSTR>(FileName.Get()), SIZEBUF);
				ListView_GetItemText(lpDrawItem->hwndItem, lpDrawItem->itemID, 1, const_cast<LPSTR>(FileType.Get()), SIZEBUF);

				if (Core::GetShortExecutableTypeFromFull(Core::GlobalEnginePtr->GetEditorVersion()) == Core::EDITOR_SHORT_STARFIELD)
				{
					using namespace EditorAPI::Starfield;

					auto type = TESFile::GetTypeFile((EditorAPI::BSString::Utils::GetRelativeDataPath() + FileName).Get());

					//_CONSOLE("%s %X", (EditorAPI::BSString::Utils::GetRelativeDataPath() + FileName).Get(), type);

					if ((type & TESFile::FILE_RECORD_ESM) == TESFile::FILE_RECORD_ESM)
					{
						if ((type & TESFile::FILE_RECORD_LIGHT) == TESFile::FILE_RECORD_LIGHT)
							Canvas.FillWithTransparent(rc, RGB(0, 255, 0), 10);
						else if ((type & TESFile::FILE_RECORD_MID) == TESFile::FILE_RECORD_MID)
							Canvas.FillWithTransparent(rc, RGB(255, 128, 0), 10);
						else
							Canvas.FillWithTransparent(rc, RGB(255, 0, 0), 10);
					}
				}
				else
				{
					// In principle, this API is the same for all games, so it's safe to take Skyrim
					using namespace EditorAPI::SkyrimSpectialEdition;

					auto type = TESFile::GetTypeFile((EditorAPI::BSString::Utils::GetRelativeDataPath() + FileName).Get());
					if ((type & TESFile::FILE_RECORD_ESM) == TESFile::FILE_RECORD_ESM)
						Canvas.FillWithTransparent(rc, RGB(255, 0, 0), 10);
					else if ((type & TESFile::FILE_RECORD_ESL) == TESFile::FILE_RECORD_ESL)
						Canvas.FillWithTransparent(rc, RGB(0, 255, 0), 10);
				}

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

				Canvas.Font.Assign(*ThemeFont);

				SetBkMode(lpDrawItem->hDC, TRANSPARENT);
				SetTextColor(lpDrawItem->hDC, GetThemeSysColor(ThemeColor::ThemeColor_Text_4));

				Graphics::CRECT rcText;
				ListView_GetSubItemRect(lpDrawItem->hwndItem, lpDrawItem->itemID, 0, LVIR_LABEL, (LPRECT)&rcText);
				rcText.Inflate(-2, -2);
				rcText.Left += 2;

				Canvas.TextRect(rcText, FileName.Get(), DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);

				ListView_GetSubItemRect(lpDrawItem->hwndItem, lpDrawItem->itemID, 1, LVIR_LABEL, (LPRECT)&rcText);
				rcText.Inflate(-2, -2);
				rcText.Left += 2;

				Canvas.TextRect(rcText, FileType.Get(), DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);

				if (Selected)
					// blend 40%
					Canvas.FillWithTransparent(rc, GetThemeSysColor(ThemeColor::ThemeColor_ListView_Owner_Selected), 40);
			}

#ifdef _CKPE_WITH_QT5
			static LRESULT OnCustomDrawObjectListQt5(HWND hWindow, LPNMLVCUSTOMDRAW lpListView)
			{
				// Not suitable 
				// https://github.com/Perchik71/Creation-Kit-Platform-Extended/issues/58
#if 0
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
						LVITEM lvItem;
						ZeroMemory(&lvItem, sizeof(LVITEM));
						lvItem.iItem = lpListView->nmcd.dwItemSpec;
						lvItem.mask = LVIF_PARAM;
						if (!ListView_GetItem(lpListView->nmcd.hdr.hwndFrom, &lvItem))
							return CDRF_DODEFAULT;

						auto form = (EditorAPI::Starfield::TESForm*)lvItem.lParam;
						if ((form->Type == EditorAPI::Starfield::TESForm::ftLight) && (lpListView->iSubItem == 1))
						{
							auto color = ((EditorAPI::Starfield::TESObjectLIGH*)form)->GetSpecularColor();
							if (color.r > 127 || color.g > 127 || color.b > 127)
								lpListView->clrText = 0;	// black
							else
								lpListView->clrText = GetThemeSysColor(ThemeColor_Text_4);

							lpListView->clrTextBk = *(COLORREF*)color.v;
						
							return CDRF_NEWFONT;
						}
						else
						{
							lpListView->clrText = GetThemeSysColor(ThemeColor_Text_4);
							lpListView->clrTextBk = GetThemeSysColor(ThemeColor_Edit_Color);
						}

						return CDRF_DODEFAULT;
					}
				}
#else
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
						LVITEM lvItem;
						ZeroMemory(&lvItem, sizeof(LVITEM));
						lvItem.iItem = lpListView->nmcd.dwItemSpec;
						lvItem.mask = LVIF_PARAM | LVIF_STATE;
						if (!ListView_GetItem(lpListView->nmcd.hdr.hwndFrom, &lvItem))
						{
						def_color:
							lpListView->clrText = GetThemeSysColor(ThemeColor_Text_4);
							return CDRF_NEWFONT;
						}

						auto form = (EditorAPI::Starfield::TESForm*)lvItem.lParam;
						if ((form->Type == EditorAPI::Starfield::TESForm::ftLight) && (lpListView->iSubItem == 1))
						{
							if (((lvItem.state & 0xFF) & LVIS_SELECTED) == LVIS_SELECTED)
								goto def_color;

							auto color = ((EditorAPI::Starfield::TESObjectLIGH*)form)->GetSpecularColor();
							lpListView->clrText = RGB(color.r, color.g, color.b);
						}
						else
							goto def_color;

						return CDRF_NEWFONT;
					}
				}
#endif

				return CDRF_DODEFAULT;
			}
		
			static LRESULT OnCustomDrawCellListQt5(HWND hWindow, LPNMLVCUSTOMDRAW lpListView)
			{
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
					// I have this field of eternal green color
					if (lpListView->iSubItem == 8)
						lpListView->clrText = RGB(0, 255, 0);
					else
						lpListView->clrText = GetThemeSysColor(ThemeColor_Text_4);

					return CDRF_DODEFAULT;
				}
				}

				return CDRF_DODEFAULT;
			}

			static LRESULT OnCustomDrawCellObjectListQt5(HWND hWindow, LPNMLVCUSTOMDRAW lpListView)
			{
				switch (lpListView->nmcd.dwDrawStage)
				{
					//Before the paint cycle begins
					case CDDS_PREPAINT:
					//request notifications for individual listview items
						return CDRF_NOTIFYITEMDRAW;
					//Before an item is drawn
					case CDDS_ITEMPREPAINT:
					//	return CDRF_NOTIFYSUBITEMDRAW;
					//Before a subitem is drawn
					case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
					{
						// Unfortunately, it is not possible to call the native function because of Qt.
						// I will return the coloring forms.

						LVITEM lvItem;
						ZeroMemory(&lvItem, sizeof(LVITEM));
						lvItem.iItem = lpListView->nmcd.dwItemSpec;
						lvItem.mask = LVIF_PARAM;
						if (!ListView_GetItem(lpListView->nmcd.hdr.hwndFrom, &lvItem))
							return CDRF_DODEFAULT;

						auto FormRef = (EditorAPI::Starfield::TESObjectREFR*)lvItem.lParam;
						if (FormRef)
						{
							//if (thisVirtualCall<EditorAPI::Starfield::TESObjectREFR*>(0x2D0, FormRef))
							{
								auto FadeNode = FormRef->GetFadeNode();
								if (FadeNode)
								{
									//Core::fastCall<void>(0x6029B0, FadeNode);

									if (FadeNode->QNotVisible())
									{
										lpListView->clrText = RGB(0, 255, 255);	// blue-gray
										return CDRF_NEWFONT;
									}
									else if (FadeNode->QAppCulled())
									{
										lpListView->clrText = RGB(255, 0, 255); // acrid pink
										return CDRF_NEWFONT;
									}
								}
							}
						}

						return CDRF_DODEFAULT;
					}
				}

				return CDRF_DODEFAULT;
			}

			static BOOL OnCustomDrawQt5(HWND hWindow, LPNMLVCUSTOMDRAW lpListView, LRESULT& lpResult)
			{
				switch (lpListView->nmcd.hdr.idFrom) 
				{
				// Object list
				case 1041:
					lpResult = OnCustomDrawObjectListQt5(hWindow, lpListView);
					return TRUE;
				// Cell list
				case 1155:	
					lpResult = OnCustomDrawCellListQt5(hWindow, lpListView);
					return TRUE;
				// Cell object list	
				case 1156:		
					lpResult = OnCustomDrawCellObjectListQt5(hWindow, lpListView);
					return TRUE;
				}

				return FALSE;
			}
#else
			static LRESULT OnCustomDrawObjectList(HWND hWindow, LPNMLVCUSTOMDRAW lpListView)
			{
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
					LVITEM lvItem;
					ZeroMemory(&lvItem, sizeof(LVITEM));
					lvItem.iItem = lpListView->nmcd.dwItemSpec;
					lvItem.mask = LVIF_PARAM | LVIF_STATE;
					if (!ListView_GetItem(lpListView->nmcd.hdr.hwndFrom, &lvItem))
					{
					def_color:
						lpListView->clrText = GetThemeSysColor(ThemeColor_Text_4);
						return CDRF_NEWFONT;
					}

					if (Core::GlobalEnginePtr->GetEditorVersion() <= Core::EDITOR_SKYRIM_SE_LAST)
					{
						auto form = (EditorAPI::SkyrimSpectialEdition::TESForm*)lvItem.lParam;
						if ((form->Type == EditorAPI::SkyrimSpectialEdition::TESForm::ftLight) && (lpListView->iSubItem == 1))
						{
							if (((lvItem.state & 0xFF) & LVIS_SELECTED) == LVIS_SELECTED)
								goto def_color;

							auto color = ((EditorAPI::SkyrimSpectialEdition::TESObjectLIGH*)form)->GetSpecularColor();
							lpListView->clrText = RGB(color.r, color.g, color.b);
						}
						else
							goto def_color;
					}
					else if (Core::GlobalEnginePtr->GetEditorVersion() <= Core::EDITOR_FALLOUT_C4_LAST)
					{
						auto form = (EditorAPI::Fallout4::TESForm*)lvItem.lParam;
						if ((form->Type == EditorAPI::Fallout4::TESForm::ftLight) && (lpListView->iSubItem == 1))
						{
							if (((lvItem.state & 0xFF) & LVIS_SELECTED) == LVIS_SELECTED)
								goto def_color;

							auto color = ((EditorAPI::Fallout4::TESObjectLIGH*)form)->GetSpecularColor();
							lpListView->clrText = RGB(color.r, color.g, color.b);
						}
						else
							goto def_color;
					}
					else
						goto def_color;
				}
				}

				return CDRF_DODEFAULT;
			}
#endif // !_CKPE_WITH_QT5

			LRESULT OnCustomDraw(HWND hWindow, LPNMLVCUSTOMDRAW lpListView)
			{
#ifndef _CKPE_WITH_QT5
				// skip it controls
				switch (lpListView->nmcd.hdr.idFrom) {
				// Object list
				case 1041:
					return OnCustomDrawObjectList(hWindow, lpListView);
				// Cell list
				case 1155:
				// Cell object list	
				case 1156:
					return DefSubclassProc(hWindow, WM_NOTIFY, 0, (LPARAM)lpListView);
				}
#else
				LRESULT Result = CDRF_DODEFAULT;
				if (OnCustomDrawQt5(hWindow, lpListView, Result))
					return Result;
#endif // !_CKPE_WITH_QT5

				//Graphics::CUICanvas Canvas(lpListView->nmcd.hdc);

				switch (lpListView->nmcd.dwDrawStage) {
				//Before the paint cycle begins
				case CDDS_PREPAINT: {
					//request notifications for individual listview items
					return CDRF_NOTIFYITEMDRAW;
				}
				//Before an item is drawn
				case CDDS_ITEMPREPAINT: {
					return CDRF_NOTIFYSUBITEMDRAW;
				}
				//Before a subitem is drawn
				case CDDS_SUBITEM | CDDS_ITEMPREPAINT: {
					switch (lpListView->nmcd.hdr.idFrom) {
					case UI_CONTROL_CONDITION_ID: {
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
		}
	}
}