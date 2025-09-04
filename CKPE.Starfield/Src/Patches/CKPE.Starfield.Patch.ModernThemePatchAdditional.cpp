// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Asserts.h>
#include <CKPE.PathUtils.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.UITimeOfDay.h>
#include <CKPE.Common.UIListView.h>
#include <CKPE.Common.ModernTheme.h>
#include <CKPE.Starfield.VersionLists.h>
#include <EditorAPI/Forms/TESObjectREFR.h>
#include <EditorAPI/Forms/TESObjectLIGH.h>
#include <Patches/CKPE.Starfield.Patch.ModernThemePatchAdditional.h>
#include "..\..\CKPE.Common\resource.h"

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
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

						auto color = ((EditorAPI::Forms::TESObjectLIGH*)form)->SpecularColor;
						lpListView->clrText = RGB(color.r, color.g, color.b);
					}
					else
						goto def_color;
				}
				}

				return CDRF_DODEFAULT;
			}

			static LRESULT OnCustomDrawCellList(HWND hWindow, LPNMLVCUSTOMDRAW lpListView)
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
						lpListView->clrText = Common::UI::GetThemeSysColor(Common::UI::ThemeColor_Text_4);

					return CDRF_DODEFAULT;
				}
				}

				return CDRF_DODEFAULT;
			}

			static LRESULT OnCustomDrawCellObjectList(HWND hWindow, LPNMLVCUSTOMDRAW lpListView)
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

					auto FormRef = (EditorAPI::Forms::TESObjectREFR*)lvItem.lParam;
					if (FormRef)
					{
						auto FadeNode = FormRef->GetFadeNode();
						if (FadeNode)
						{
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

					return CDRF_DODEFAULT;
				}
				}

				return CDRF_DODEFAULT;
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
					bReturn = true;
					return OnCustomDrawCellList(hWindow, lpListView);
					// Cell object list	
				case 1156:
					bReturn = true;
					return OnCustomDrawCellObjectList(hWindow, lpListView);
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
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_STARFIELD_LAST;
			}

			bool ModernThemePatchAdditional::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();
				if (!Common::UI::IsDarkTheme() || 
					_READ_OPTION_BOOL("CreationKit", "bUIClassicTheme", false) || (verPatch != 3))
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// CinfigureWindow sets now
				Common::ModernTheme::InitializeCurrentThread();

				// replace toolbar
				Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&Comctl32CreateToolbarEx_NavMesh);
				// replace ImageList_LoadImage for item type
				Detours::DetourCall(__CKPE_OFFSET(1), (std::uintptr_t)&Comctl32ImageList_LoadImageA_1);
				// remove set color for treeview
				SafeWrite::WriteNop(__CKPE_OFFSET(2), 0x29);
				// replace ImageList_LoadImage
				Detours::DetourCall(__CKPE_OFFSET(3), (std::uintptr_t)&Comctl32ImageList_LoadImageA_2);	// item type
				Detours::DetourCall(__CKPE_OFFSET(4), (std::uintptr_t)&Comctl32ImageList_LoadImageA_3);	// Scripts icons
				Detours::DetourCall(__CKPE_OFFSET(5), (std::uintptr_t)&Comctl32ImageList_LoadImageA_3);	// Scripts icons

				Common::UI::ListView::InstallCustomDrawHandler(&DoCustomDrawListView);

				return true;
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
		}
	}
}