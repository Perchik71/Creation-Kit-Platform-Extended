﻿// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Asserts.h>
#include <CKPE.PathUtils.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Common.UITimeOfDay.h>
#include <CKPE.Common.UIListView.h>
#include <CKPE.Common.ModernTheme.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/Forms/TESObjectLIGH.h>
#include <Patches/CKPE.Fallout4.Patch.ModernThemePatchAdditional.h>
#include "..\CKPE.Common\resource.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			std::uintptr_t pointer_UIThemePatchAdditional_sub = 0;

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
						lpListView->clrText = Common::UI::GetThemeSysColor(Common::UI::ThemeColor_Text_4);
						return CDRF_NEWFONT;
					}

					auto form = (EditorAPI::Forms::TESForm*)lvItem.lParam;
					if ((form->Type == EditorAPI::Forms::TESObjectLIGH::TYPE_ID) && (lpListView->iSubItem == 1))
					{
						if (((lvItem.state & 0xFF) & LVIS_SELECTED) == LVIS_SELECTED)
							goto def_color;

						auto color = ((EditorAPI::Forms::TESObjectLIGH*)form)->GetSpecularColor();
						lpListView->clrText = RGB(color.r, color.g, color.b);
					}
					else
						goto def_color;
				}
				}

				return CDRF_DODEFAULT;
			}

			static LRESULT HkSendMsgChangeColorTextForLayers(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept(true)
			{
				//TreeView_SetBorder(hWnd, TVSBF_XBORDER | TVSBF_YBORDER, 0, 0);
				TreeView_SetLineColor(hWnd, Common::UI::Comctl32GetSysColor(COLOR_WINDOWFRAME));
				TreeView_SetBkColor(hWnd, Common::UI::Comctl32GetSysColor(COLOR_WINDOW));
				return TreeView_SetTextColor(hWnd, Common::UI::Comctl32GetSysColor(COLOR_WINDOWTEXT));
			}

			static COLORREF HkSetTextColorForLayers(HDC hDC, COLORREF c) noexcept(true)
			{
				return SetTextColor(hDC, Common::UI::Comctl32GetSysColor(COLOR_WINDOWTEXT));
			}

			static BOOL WINAPI HkDrawEdgeForLayers(HDC hdc, LPRECT qrc, UINT edge, UINT grfFlags) noexcept(true)
			{
				if (!qrc || !hdc)
					return FALSE;

				Canvas canvas = hdc;

				if ((grfFlags & BF_MIDDLE) == BF_MIDDLE)
					canvas.Fill((CRECT)*qrc, Common::UI::GetThemeSysColor(Common::UI::ThemeColor_TreeView_Color));

				canvas.Pen.Color = Common::UI::GetThemeSysColor(Common::UI::ThemeColor_Default);

				if ((grfFlags & BF_LEFT) == BF_LEFT)
				{
					canvas.MoveTo(qrc->left, qrc->top);
					canvas.LineTo(qrc->left, qrc->bottom - 1);

					if ((grfFlags & BF_ADJUST) == BF_ADJUST)
						qrc->left++;
				}

				if ((grfFlags & BF_TOP) == BF_TOP)
				{
					canvas.MoveTo(qrc->left, qrc->top);
					canvas.LineTo(qrc->right, qrc->top);

					if ((grfFlags & BF_ADJUST) == BF_ADJUST)
						qrc->top++;
				}

				if ((grfFlags & BF_RIGHT) == BF_RIGHT)
				{
					canvas.MoveTo(qrc->right, qrc->top);
					canvas.LineTo(qrc->right, qrc->bottom - 1);

					if ((grfFlags & BF_ADJUST) == BF_ADJUST)
						qrc->right--;
				}

				if ((grfFlags & BF_BOTTOM) == BF_BOTTOM)
				{
					canvas.MoveTo(qrc->left, qrc->bottom - 1);
					canvas.LineTo(qrc->right, qrc->bottom - 1);

					if ((grfFlags & BF_ADJUST) == BF_ADJUST)
						qrc->bottom--;
				}

				if ((grfFlags & BF_DIAGONAL_ENDBOTTOMLEFT) == BF_DIAGONAL_ENDBOTTOMLEFT)
				{
					canvas.MoveTo(qrc->right, qrc->top);
					canvas.LineTo(qrc->left, qrc->bottom - 1);
				}

				if ((grfFlags & BF_DIAGONAL_ENDBOTTOMRIGHT) == BF_DIAGONAL_ENDBOTTOMRIGHT)
				{
					canvas.MoveTo(qrc->left, qrc->top);
					canvas.LineTo(qrc->right, qrc->bottom - 1);
				}

				if ((grfFlags & BF_DIAGONAL_ENDTOPLEFT) == BF_DIAGONAL_ENDTOPLEFT)
				{
					canvas.MoveTo(qrc->right, qrc->bottom - 1);
					canvas.LineTo(qrc->left, qrc->top);
				}

				if ((grfFlags & BF_DIAGONAL_ENDTOPRIGHT) == BF_DIAGONAL_ENDTOPRIGHT)
				{
					canvas.MoveTo(qrc->left, qrc->bottom - 1);
					canvas.LineTo(qrc->right, qrc->top);
				}

				return TRUE;
			}

			static HIMAGELIST HkImageListForLayers_LoadImageA(HINSTANCE hi, LPCSTR lpbmp, INT cx, INT cGrow,
				COLORREF crMask, UINT uType, UINT uFlags) noexcept(true)
			{
				if ((ULONG_PTR)lpbmp == 430) 
					return ::ImageList_LoadImageA((HINSTANCE)Common::Interface::GetSingleton()->GetInstanceDLL(),
						MAKEINTRESOURCEA(IDB_BITMAP10), 16, 0, RGB(63, 63, 63), IMAGE_BITMAP,
						LR_CREATEDIBSECTION | LR_LOADTRANSPARENT);

				return ::ImageList_LoadImageA(hi, lpbmp, cx, cGrow, crMask, uType, uFlags);
			}

			static HWND HkCreateWindowExAForPropGrid(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName,
				DWORD dwStyle, std::int32_t X, std::int32_t Y, std::int32_t nWidth, std::int32_t nHeight,
				HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) noexcept(true)
			{
				auto hWnd = CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight,
					hWndParent, hMenu, hInstance, lpParam);

				SetClassLong(hWnd, GCLP_HBRBACKGROUND, (LONG)::CreateSolidBrush(RGB(90, 0, 0)));
				return hWnd;
			}

			LRESULT ModernThemePatchAdditional::DoCustomDrawListView(HWND hWindow, LPNMLVCUSTOMDRAW lpListView,
				bool& bReturn) noexcept(true)
			{
				// skip it controls
				switch (lpListView->nmcd.hdr.idFrom) 
				{
					// Object list
				case 1041:
					bReturn = true;
					return OnCustomDrawObjectList(hWindow, lpListView);
					// Cell list
				case 1155:
					// Cell object list	
				case 1156:
					bReturn = true;
					return DefSubclassProc(hWindow, WM_NOTIFY, 0, (LPARAM)lpListView);
				}

				return S_OK;
			}

			ModernThemePatchAdditional::ModernThemePatchAdditional() : Common::Patch()
			{
				SetName("UI Dark Theme Additional");
			}

			bool ModernThemePatchAdditional::HasOption() const noexcept(true)
			{
				return true;
			}

			const char* ModernThemePatchAdditional::GetOptionName() const noexcept(true)
			{
				return "CreationKit:bUIDarkTheme";
			}

			bool ModernThemePatchAdditional::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> ModernThemePatchAdditional::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool ModernThemePatchAdditional::DoQuery() const noexcept(true)
			{
				return !CKPE_UserUseWine() && (VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST);
			}

			bool ModernThemePatchAdditional::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();
				if (!Common::UI::IsDarkTheme() || 
					_READ_OPTION_BOOL("CreationKit", "bUIClassicTheme", false) || (verPatch != 2))
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// CinfigureWindow sets now
				Common::ModernTheme::InitializeCurrentThread();

				pointer_UIThemePatchAdditional_sub = __CKPE_OFFSET(0);
				// replace main toolbar
				Detours::DetourCall(__CKPE_OFFSET(1), (std::uintptr_t)&Comctl32CreateToolbarEx_1);
				Detours::DetourCall(__CKPE_OFFSET(2), (std::uintptr_t)&Comctl32CreateToolbarEx_NavMesh);
				Detours::DetourJump(__CKPE_OFFSET(3), (std::uintptr_t)&HideOldTimeOfDayComponents);
				// replace ImageList_LoadImage for item type
				Detours::DetourCall(__CKPE_OFFSET(4), (std::uintptr_t)&Comctl32ImageList_LoadImageA_1);

				Detours::DetourCall(__CKPE_OFFSET(5), (std::uintptr_t)&HKInitializeTimeOfDay);
				Detours::DetourCall(__CKPE_OFFSET(6), (std::uintptr_t)&HKSetNewValueTimeOfDay);

				if (VersionLists::GetEditorVersion() != VersionLists::EDITOR_FALLOUT_C4_1_10_943_1)
				{
					// replace ImageList_LoadImage
					Detours::DetourCall(__CKPE_OFFSET(7), (std::uintptr_t)&Comctl32ImageList_LoadImageA_2);			// item type Task Manager
					Detours::DetourCall(__CKPE_OFFSET(8), (std::uintptr_t)&Comctl32ImageList_LoadImageA_3);			// Scripts icons
					Detours::DetourCall(__CKPE_OFFSET(9), (std::uintptr_t)&Comctl32ImageList_LoadImageA_3);			// Scripts icons

					if (VersionLists::GetEditorVersion() != VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
						Detours::DetourCall(__CKPE_OFFSET(10), (std::uintptr_t)&Comctl32ImageList_LoadImageA_3);	// Scripts icons
				}

				Common::UI::ListView::InstallCustomDrawHandler(&DoCustomDrawListView);

				if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
				{
					// Layers Window
					Detours::DetourCall(__CKPE_OFFSET(11), (std::uintptr_t)&HkSendMsgChangeColorTextForLayers);
					// Rechange color text
					auto rva = __CKPE_OFFSET(12);
					Detours::DetourCall(rva, (std::uintptr_t)&HkSetTextColorForLayers);
					Detours::DetourCall(rva + 0x212, (std::uintptr_t)&HkSetTextColorForLayers);
					Detours::DetourCall(rva + 0x471, (std::uintptr_t)&HkSetTextColorForLayers);
					Detours::DetourCall(rva + 0x48B, (std::uintptr_t)&HkSetTextColorForLayers);
					Detours::DetourCall(rva + 0x547, (std::uintptr_t)&HkSetTextColorForLayers);
					// Skip edge draw
					rva = __CKPE_OFFSET(13);
					Detours::DetourCall(rva, (std::uintptr_t)&HkDrawEdgeForLayers);
					Detours::DetourCall(rva + 0x2C, (std::uintptr_t)&HkDrawEdgeForLayers);
					Detours::DetourCall(rva + 0x46C, (std::uintptr_t)&HkDrawEdgeForLayers);
					// Icons
					Detours::DetourCall(__CKPE_OFFSET(14), (std::uintptr_t)&HkImageListForLayers_LoadImageA);
				}
				else if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_FALLOUT_C4_1_10_982_3)
				{
					// Layers Window
					Detours::DetourCall(__CKPE_OFFSET(11), (std::uintptr_t)&HkSendMsgChangeColorTextForLayers);
					// Rechange color text
					auto rva = __CKPE_OFFSET(12);
					Detours::DetourCall(rva, (std::uintptr_t)&HkSetTextColorForLayers);
					Detours::DetourCall(rva + 0x214, (std::uintptr_t)&HkSetTextColorForLayers);
					Detours::DetourCall(rva + 0x457, (std::uintptr_t)&HkSetTextColorForLayers);
					Detours::DetourCall(rva + 0x471, (std::uintptr_t)&HkSetTextColorForLayers);
					Detours::DetourCall(rva + 0x530, (std::uintptr_t)&HkSetTextColorForLayers);
					// Skip edge draw
					rva = __CKPE_OFFSET(13);
					Detours::DetourCall(rva, (std::uintptr_t)&HkDrawEdgeForLayers);
					Detours::DetourCall(rva + 0x2C, (std::uintptr_t)&HkDrawEdgeForLayers);
					Detours::DetourCall(rva + 0x459, (std::uintptr_t)&HkDrawEdgeForLayers);
					// Icons
					Detours::DetourCall(__CKPE_OFFSET(14), (std::uintptr_t)&HkImageListForLayers_LoadImageA);
					Detours::DetourCall(__CKPE_OFFSET(15), (std::uintptr_t)&HkImageListForLayers_LoadImageA);
				}

				return true;
			}

			HWND ModernThemePatchAdditional::Comctl32CreateToolbarEx_1(HWND hwnd, DWORD ws, UINT wID, INT nBitmaps,
				HINSTANCE hBMInst, UINT_PTR wBMID, LPCTBBUTTON lpButtons,
				INT iNumButtons, INT dxButton, INT dyButton, INT dxBitmap, INT dyBitmap, UINT uStructSize) noexcept(true)
			{
				constexpr static auto UI_EXTMENU_TOGGLE_ANTIALIASING = 51015;

				bool SupportedAA = true;
				HIMAGELIST hImageList;

				auto pNewButtons = std::make_unique<TBBUTTON[]>(iNumButtons);
				auto verEditor = VersionLists::GetEditorVersion();

				size_t aa_index = 19;

				if (verEditor == VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
					aa_index += 4;

				memcpy(pNewButtons.get(), lpButtons, sizeof(TBBUTTON) * aa_index);

				pNewButtons[aa_index] = (pNewButtons.get())[aa_index - 1];
				pNewButtons[aa_index].iBitmap = 59;
				pNewButtons[aa_index].idCommand = UI_EXTMENU_TOGGLE_ANTIALIASING;
				pNewButtons[aa_index].fsState |= TBSTATE_CHECKED;

				memcpy(&(pNewButtons.get())[aa_index + 1], &lpButtons[aa_index], sizeof(TBBUTTON) * 
					((std::size_t)iNumButtons - (aa_index + 1)));

				if (Common::UI::GetTheme() == Common::UI::Theme_Custom)
				{
					auto fname = Common::UI::GetFileNameToolbarForCustomTheme();
					if (PathUtils::FileExists(fname.c_str()))
					{
						hImageList = ImageList_LoadImageW(NULL, fname.c_str(), 16, 0,
							Common::UI::GetMaskColorToolbarForCustomTheme(), IMAGE_BITMAP,
							LR_CREATEDIBSECTION | LR_LOADTRANSPARENT | LR_LOADFROMFILE);
						if (!hImageList)
							goto Skips;
					}
					else
						goto Skips;
				}
				else
					Skips:
					hImageList = ImageList_LoadImageA((HINSTANCE)Common::Interface::GetSingleton()->GetInstanceDLL(),
						MAKEINTRESOURCEA(IDB_BITMAP3), 16, 0, RGB(56, 56, 56), IMAGE_BITMAP,
						LR_CREATEDIBSECTION | LR_LOADTRANSPARENT);

				HWND ret = CreateToolbarEx(hwnd, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, wID, nBitmaps,
					NULL, NULL, pNewButtons.get(), SupportedAA ? (iNumButtons - 1) : (iNumButtons - 2) 
					/*delete two divider*/, dxButton, dyButton, dxBitmap, dyBitmap, uStructSize);

				if (ret)
				{
					SendMessageA(ret, TB_SETIMAGELIST, 0, (LPARAM)hImageList);
					SendMessageA(ret, TB_SETBITMAPSIZE, 0, MAKELPARAM(16, 16));
					ShowWindow(ret, SW_SHOWNORMAL);
					SetWindowPos(ret, NULL, 0, 24, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

					Common::UI::TimeOfDay::Initialization(ret, hwnd);
				}

				return ret;
			}

			HWND ModernThemePatchAdditional::Comctl32CreateToolbarEx_NavMesh(HWND hwnd, DWORD ws, UINT wID, INT nBitmaps,
				HINSTANCE hBMInst, UINT_PTR wBMID, LPTBBUTTON lpButtons,
				INT iNumButtons, INT dxButton, INT dyButton, INT dxBitmap, INT dyBitmap, UINT uStructSize) noexcept(true)
			{
				HIMAGELIST hImageList;

				if (Common::UI::GetTheme() == Common::UI::Theme_Custom)
				{
					auto fname = Common::UI::GetFileNameToolbarNavMeshForCustomTheme();
					if (PathUtils::FileExists(fname.c_str()))
					{
						hImageList = ImageList_LoadImageW(NULL, fname.c_str(), 16, 0,
							Common::UI::GetMaskColorToolbarNavMeshForCustomTheme(), IMAGE_BITMAP,
							LR_CREATEDIBSECTION | LR_LOADTRANSPARENT | LR_LOADFROMFILE);
						if (!hImageList)
							goto Skips;
					}
					else
						goto Skips;
				}
				else
					Skips:
					hImageList = ImageList_LoadImageA((HINSTANCE)Common::Interface::GetSingleton()->GetInstanceDLL(), 
						MAKEINTRESOURCEA(IDB_BITMAP7), 16, 0, RGB(56, 56, 56), IMAGE_BITMAP,
						LR_CREATEDIBSECTION | LR_LOADTRANSPARENT);

				HWND ret = CreateToolbarEx(hwnd, ws, wID, nBitmaps, NULL, NULL, lpButtons,
					iNumButtons, dxButton, dyButton, dxBitmap, dyBitmap, uStructSize);

				SendMessageA(ret, TB_SETIMAGELIST, 0, (LPARAM)hImageList);
				SendMessageA(ret, TB_SETBITMAPSIZE, 0, MAKELPARAM(16, 16));
				ShowWindow(ret, SW_SHOWNORMAL);
				SetWindowPos(ret, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

				return ret;
			}

			HIMAGELIST ModernThemePatchAdditional::Comctl32ImageList_LoadImageA_1(HINSTANCE hi,
				LPCSTR lpbmp, INT cx, INT cGrow, COLORREF crMask, UINT uType, UINT uFlags) noexcept(true)
			{
				HIMAGELIST hImageList;

				if (Common::UI::GetTheme() == Common::UI::Theme_Custom)
				{
					auto fname = Common::UI::GetFileNameIconsForCustomTheme();
					if (PathUtils::FileExists(fname.c_str()))
					{
						hImageList = ImageList_LoadImageW(NULL, fname.c_str(), 16, 0,
							Common::UI::GetMaskColorIconsForCustomTheme(), IMAGE_BITMAP,
							LR_CREATEDIBSECTION | LR_LOADTRANSPARENT | LR_LOADFROMFILE);
						if (!hImageList)
							goto Skips;
					}
					else
						goto Skips;
				}
				else
					Skips:
					hImageList = ImageList_LoadImageA((HINSTANCE)Common::Interface::GetSingleton()->GetInstanceDLL(), 
						MAKEINTRESOURCEA(IDB_BITMAP4), cx, cGrow, crMask, IMAGE_BITMAP, 
						LR_CREATEDIBSECTION | LR_LOADTRANSPARENT);

				return hImageList;
			}

			HIMAGELIST ModernThemePatchAdditional::Comctl32ImageList_LoadImageA_2(HINSTANCE hi, 
				LPCSTR lpbmp, INT cx, INT cGrow, COLORREF crMask, UINT uType, UINT uFlags) noexcept(true)
			{
				return ImageList_LoadImageA((HINSTANCE)Common::Interface::GetSingleton()->GetInstanceDLL(),
					MAKEINTRESOURCEA(IDB_BITMAP8), cx, cGrow, crMask, IMAGE_BITMAP,
					LR_CREATEDIBSECTION | LR_LOADTRANSPARENT);
			}

			HIMAGELIST ModernThemePatchAdditional::Comctl32ImageList_LoadImageA_3(HINSTANCE hi,
				LPCSTR lpbmp, INT cx, INT cGrow, COLORREF crMask, UINT uType, UINT uFlags) noexcept(true)
			{
				return ImageList_LoadImageA((HINSTANCE)Common::Interface::GetSingleton()->GetInstanceDLL(), 
					MAKEINTRESOURCEA(IDB_BITMAP9), cx, cGrow, crMask, IMAGE_BITMAP,
					LR_CREATEDIBSECTION | LR_LOADTRANSPARENT);
			}

			void ModernThemePatchAdditional::HideOldTimeOfDayComponents() noexcept(true)
			{
				// I will hide the old ones, but I will rely on them when sending messages, however, 
				// in the end, I will fake the event to change the time of day

				auto OldUITimeOfDayComponents = Common::UI::TimeOfDay::GetOldUITimeOfDayComponents();
				OldUITimeOfDayComponents->hWndLabel = GetDlgItem(OldUITimeOfDayComponents->hWndToolBar.Handle, 0x16D2);
				OldUITimeOfDayComponents->hWndTrackBar = GetDlgItem(OldUITimeOfDayComponents->hWndToolBar.Handle, 0x16D3);
				OldUITimeOfDayComponents->hWndEdit = GetDlgItem(OldUITimeOfDayComponents->hWndToolBar.Handle, 0x16D4);

				ShowWindow(OldUITimeOfDayComponents->hWndLabel.Handle, SW_HIDE);
				ShowWindow(OldUITimeOfDayComponents->hWndTrackBar.Handle, SW_HIDE);
				ShowWindow(OldUITimeOfDayComponents->hWndEdit.Handle, SW_HIDE);

				POINT Range =
				{
					(LONG)OldUITimeOfDayComponents->hWndTrackBar.Perform(TBM_GETRANGEMIN, 0, 0),
					(LONG)OldUITimeOfDayComponents->hWndTrackBar.Perform(TBM_GETRANGEMAX, 0, 0)
				};

				auto NewUITimeOfDayComponents = Common::UI::TimeOfDay::GetNewUITimeOfDayComponents();
				NewUITimeOfDayComponents->hWndTrackBar.Perform(TBM_SETRANGE, (WPARAM)TRUE, MAKELPARAM(Range.x, Range.y));
				NewUITimeOfDayComponents->hWndTrackBar.Perform(TBM_SETPOS, (WPARAM)TRUE, (LONG_PTR)((10 * (Range.y - Range.x)) / 24));
				NewUITimeOfDayComponents->hWndEdit.SetCaption("10.00");
			}

			void ModernThemePatchAdditional::HKInitializeTimeOfDay(HWND hDlg, INT nIDDlgItem, FLOAT value, 
				INT a4) noexcept(true)
			{
				auto hwndTrackBar = GetDlgItem(hDlg, 0x3F6);
				auto hwndEdit = GetDlgItem(hDlg, 0x3E8);

				auto NewUITimeOfDayComponents = Common::UI::TimeOfDay::GetNewUITimeOfDayComponents();
				INT32 iPos = NewUITimeOfDayComponents->hWndTrackBar.Perform(TBM_GETPOS, 0, 0);

				SendMessage(hwndTrackBar, TBM_SETPOS, TRUE, (LPARAM)iPos);
				SetWindowText(hwndEdit, NewUITimeOfDayComponents->hWndEdit.GetCaption().c_str());
				EnableWindow(hwndEdit, FALSE);
			}

			void ModernThemePatchAdditional::HKSetNewValueTimeOfDay(HWND hDlg, INT nIDDlgItem, FLOAT value, 
				INT a4) noexcept(true)
			{
				auto hwndTrackBar = GetDlgItem(hDlg, 0x3F6);
				auto hwndEdit = GetDlgItem(hDlg, 0x3E8);

				INT32 iPos = SendMessage(hwndTrackBar, TBM_GETPOS, 0, 0);

				auto NewUITimeOfDayComponents = Common::UI::TimeOfDay::GetNewUITimeOfDayComponents();
				NewUITimeOfDayComponents->hWndTrackBar.Perform(TBM_SETPOS, TRUE, (LPARAM)iPos);

				CHAR szBuf[24];
				sprintf_s(szBuf, "%.2f", value);
				NewUITimeOfDayComponents->hWndEdit.SetCaption(szBuf);

				((void(__fastcall*)(HWND, INT, FLOAT, INT))pointer_UIThemePatchAdditional_sub)(hDlg, nIDDlgItem, value, a4);
			}
		}
	}
}