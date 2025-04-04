// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "resource.h"
#include "Core/Engine.h"
#include "Editor API/EditorUI.h"
#include "UIThemePatch.h"
#include "UIThemePatchAdditional.h"
#include "UITheme/VarCommon.h"
#include "UITheme/TimeOfDay.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		using namespace CreationKitPlatformExtended::EditorAPI;
		using namespace CreationKitPlatformExtended::UITheme::TimeOfDay;

		uintptr_t pointer_UIThemePatchAdditional_sub = 0;

		UIThemePatchAdditional::UIThemePatchAdditional() : Module(GlobalEnginePtr)
		{}

		bool UIThemePatchAdditional::HasOption() const
		{
			return false;
		}

		bool UIThemePatchAdditional::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* UIThemePatchAdditional::GetOptionName() const
		{
			return nullptr;
		}

		const char* UIThemePatchAdditional::GetName() const
		{
			return "UI Dark Theme Additional";
		}

		bool UIThemePatchAdditional::HasDependencies() const
		{
			return true;
		}

		Array<String> UIThemePatchAdditional::GetDependencies() const
		{
			return { "UI Dark Theme" };
		}

		bool UIThemePatchAdditional::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return !_READ_OPTION_BOOL("CreationKit", "bUIClassicTheme", false);
		}

		bool UIThemePatchAdditional::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			auto verPatch = lpRelocationDatabaseItem->Version();
			if ((verPatch == 1) || (verPatch == 2))
			{
				UIThemePatch::InitializeCurrentThread();

				pointer_UIThemePatchAdditional_sub = _RELDATA_ADDR(0);
				// replace main toolbar
				lpRelocator->DetourCall(_RELDATA_RAV(1), (uintptr_t)&Comctl32CreateToolbarEx_1);
				lpRelocator->DetourCall(_RELDATA_RAV(2), (uintptr_t)&Comctl32CreateToolbarEx_NavMesh);
				lpRelocator->DetourJump(_RELDATA_RAV(3), (uintptr_t)&HideOldTimeOfDayComponents);
				// replace ImageList_LoadImage for item type
				lpRelocator->DetourCall(_RELDATA_RAV(4), (uintptr_t)&Comctl32ImageList_LoadImageA_1);

				lpRelocator->DetourCall(_RELDATA_RAV(5), (uintptr_t)&HKInitializeTimeOfDay);
				lpRelocator->DetourCall(_RELDATA_RAV(6), (uintptr_t)&HKSetNewValueTimeOfDay);

				if (verPatch != 2)
				{
					lpRelocator->PatchNop(_RELDATA_RAV(7), 7);				// Prevent setting redundant colors in the condition list view NM_CUSTOMDRAW (breaks dark theme)
					lpRelocator->Patch(_RELDATA_RAV(8), { 0x74, 0x20 });	// ^
					// replace ImageList_LoadImage
					lpRelocator->DetourCall(_RELDATA_RAV(9), (uintptr_t)&Comctl32ImageList_LoadImageA_2);	// item type Task Manager
					lpRelocator->DetourCall(_RELDATA_RAV(10), (uintptr_t)&Comctl32ImageList_LoadImageA_3);	// Scripts icons
					lpRelocator->DetourCall(_RELDATA_RAV(11), (uintptr_t)&Comctl32ImageList_LoadImageA_3);	// Scripts icons
				}
				else
				{
					// replace ImageList_LoadImage
					lpRelocator->DetourCall(_RELDATA_RAV(7), (uintptr_t)&Comctl32ImageList_LoadImageA_2);	// item type Task Manager
					lpRelocator->DetourCall(_RELDATA_RAV(8), (uintptr_t)&Comctl32ImageList_LoadImageA_3);	// Scripts icons
					lpRelocator->DetourCall(_RELDATA_RAV(9), (uintptr_t)&Comctl32ImageList_LoadImageA_3);	// Scripts icons
					lpRelocator->DetourCall(_RELDATA_RAV(10), (uintptr_t)&Comctl32ImageList_LoadImageA_3);	// Scripts icons
				}

				return true;
			}
			else if (verPatch == 3)
			{
				UIThemePatch::InitializeCurrentThread();

				// replace toolbar
				lpRelocator->DetourCall(_RELDATA_RAV(0), (uintptr_t)&Comctl32CreateToolbarEx_NavMesh);
				// replace ImageList_LoadImage for item type
				lpRelocator->DetourCall(_RELDATA_RAV(1), (uintptr_t)&Comctl32ImageList_LoadImageA_1);
				// remove set color for treeview
				lpRelocator->PatchNop(_RELDATA_RAV(2), 0x29);
				// replace ImageList_LoadImage
				lpRelocator->DetourCall(_RELDATA_RAV(3), (uintptr_t)&Comctl32ImageList_LoadImageA_2);	// item type
				lpRelocator->DetourCall(_RELDATA_RAV(4), (uintptr_t)&Comctl32ImageList_LoadImageA_3);	// Scripts icons
				lpRelocator->DetourCall(_RELDATA_RAV(5), (uintptr_t)&Comctl32ImageList_LoadImageA_3);	// Scripts icons
			}
			
			return false;
		}

		bool UIThemePatchAdditional::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}

		HWND UIThemePatchAdditional::Comctl32CreateToolbarEx_1(HWND hwnd, DWORD ws, UINT wID, INT nBitmaps,
			HINSTANCE hBMInst, UINT_PTR wBMID, LPCTBBUTTON lpButtons,
			INT iNumButtons, INT dxButton, INT dyButton, INT dxBitmap, INT dyBitmap, UINT uStructSize)
		{
			constexpr static auto UI_EXTMENU_TOGGLE_ANTIALIASING = 51015;

			bool SupportedAA = true;
			HIMAGELIST hImageList;

			auto pNewButtons = std::make_unique<TBBUTTON[]>(iNumButtons);
			auto verEditor = GlobalEnginePtr->GetEditorVersion();

			if ((verEditor >= EDITOR_SKYRIM_SE_1_6_1130) && (verEditor <= EDITOR_FALLOUT_C4_LAST))
			{
				size_t aa_index = 19;
				if (verEditor <= EDITOR_SKYRIM_SE_LAST)
					aa_index++;

				if (verEditor == EDITOR_FALLOUT_C4_1_10_162_0)
					aa_index += 4;

				memcpy(pNewButtons.get(), lpButtons, sizeof(TBBUTTON) * aa_index);

				pNewButtons[aa_index] = (pNewButtons.get())[aa_index - 1];
				pNewButtons[aa_index].iBitmap = 59;
				pNewButtons[aa_index].idCommand = UI_EXTMENU_TOGGLE_ANTIALIASING;
				pNewButtons[aa_index].fsState |= TBSTATE_CHECKED;

				memcpy(&(pNewButtons.get())[aa_index + 1], &lpButtons[aa_index], sizeof(TBBUTTON) * ((size_t)iNumButtons - (aa_index + 1)));
			}
			else
				memcpy(pNewButtons.get(), lpButtons, sizeof(TBBUTTON) * iNumButtons);

			if (UITheme::GetTheme() == UITheme::Theme_Custom)
			{
				auto fname = UITheme::GetFileNameToolbarForCustomTheme();
				if (Utils::FileExists(fname.c_str()))
				{
					hImageList = ImageList_LoadImageA(NULL, fname.c_str(), 16, 0,
						UITheme::GetMaskColorToolbarForCustomTheme(), IMAGE_BITMAP,
						LR_CREATEDIBSECTION | LR_LOADTRANSPARENT | LR_LOADFROMFILE);
					if (!hImageList)
						goto jj1213;
				}
				else
					goto jj1213;
			}
			else
				jj1213:
				hImageList = ImageList_LoadImageA(GlobalEnginePtr->GetInstanceDLL(), MAKEINTRESOURCEA(IDB_BITMAP3), 16, 0,
					RGB(56, 56, 56), IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_LOADTRANSPARENT);

			HWND ret = CreateToolbarEx(hwnd, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, wID, nBitmaps,
				NULL, NULL, pNewButtons.get(), SupportedAA ? (iNumButtons - 1) : (iNumButtons - 2) /*delete two divider*/, dxButton, dyButton,
				dxBitmap, dyBitmap, uStructSize);

			if (ret)
			{
				SendMessageA(ret, TB_SETIMAGELIST, 0, (LPARAM)hImageList);
				SendMessageA(ret, TB_SETBITMAPSIZE, 0, MAKELPARAM(16, 16));
				ShowWindow(ret, SW_SHOWNORMAL);
				SetWindowPos(ret, NULL, 0, 24, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

				Initialization(ret);
			}

			return ret;
		}

		HWND UIThemePatchAdditional::Comctl32CreateToolbarEx_NavMesh(HWND hwnd, DWORD ws, UINT wID, INT nBitmaps,
			HINSTANCE hBMInst, UINT_PTR wBMID, LPTBBUTTON lpButtons,
			INT iNumButtons, INT dxButton, INT dyButton, INT dxBitmap, INT dyBitmap, UINT uStructSize)
		{
			HIMAGELIST hImageList;

			if (UITheme::GetTheme() == UITheme::Theme_Custom)
			{
				auto fname = UITheme::GetFileNameToolbarNavMeshForCustomTheme();
				if (Utils::FileExists(fname.c_str()))
				{
					hImageList = ImageList_LoadImageA(NULL, fname.c_str(), 16, 0,
						UITheme::GetMaskColorToolbarNavMeshForCustomTheme(), IMAGE_BITMAP,
						LR_CREATEDIBSECTION | LR_LOADTRANSPARENT | LR_LOADFROMFILE);
					if (!hImageList)
						goto jj1214;
				}
				else
					goto jj1214;
			}
			else
				jj1214:
				hImageList = ImageList_LoadImageA(GlobalEnginePtr->GetInstanceDLL(), MAKEINTRESOURCEA(IDB_BITMAP7), 16, 0,
					RGB(56, 56, 56), IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_LOADTRANSPARENT);

			// Swap Balance for Opt. with Test navmesh
			/*auto temp = lpButtons[8];
			lpButtons[8] = lpButtons[9];
			lpButtons[9] = temp;*/

			HWND ret = CreateToolbarEx(hwnd, ws, wID, nBitmaps, NULL, NULL, lpButtons, 
				iNumButtons, dxButton, dyButton, dxBitmap, dyBitmap, uStructSize);
			
			SendMessageA(ret, TB_SETIMAGELIST, 0, (LPARAM)hImageList);
			SendMessageA(ret, TB_SETBITMAPSIZE, 0, MAKELPARAM(16, 16));
			ShowWindow(ret, SW_SHOWNORMAL);
			SetWindowPos(ret, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

			return ret;
		}

		HIMAGELIST UIThemePatchAdditional::Comctl32ImageList_LoadImageA_1(HINSTANCE hi, LPCSTR lpbmp, INT cx, INT cGrow,
			COLORREF crMask, UINT uType, UINT uFlags)
		{
			HIMAGELIST hImageList;

			if (UITheme::GetTheme() == UITheme::Theme_Custom)
			{
				auto fname = UITheme::GetFileNameIconsForCustomTheme();
				if (Utils::FileExists(fname.c_str()))
				{
					hImageList = ImageList_LoadImageA(NULL, fname.c_str(), 16, 0,
						UITheme::GetMaskColorIconsForCustomTheme(), IMAGE_BITMAP,
						LR_CREATEDIBSECTION | LR_LOADTRANSPARENT | LR_LOADFROMFILE);
					if (!hImageList)
						goto jj1214;
				}
				else
					goto jj1214;
			}
			else
				jj1214:
				hImageList = ImageList_LoadImageA(GlobalEnginePtr->GetInstanceDLL(), MAKEINTRESOURCEA(IDB_BITMAP4),
					cx, cGrow, crMask, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_LOADTRANSPARENT);

			return hImageList;
		}

		HIMAGELIST UIThemePatchAdditional::Comctl32ImageList_LoadImageA_2(HINSTANCE hi, LPCSTR lpbmp, INT cx, INT cGrow,
			COLORREF crMask, UINT uType, UINT uFlags)
		{
			return ImageList_LoadImageA(GlobalEnginePtr->GetInstanceDLL(), MAKEINTRESOURCEA(IDB_BITMAP8),
				cx, cGrow, crMask, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_LOADTRANSPARENT);
		}

		HIMAGELIST UIThemePatchAdditional::Comctl32ImageList_LoadImageA_3(HINSTANCE hi, LPCSTR lpbmp, INT cx, INT cGrow,
			COLORREF crMask, UINT uType, UINT uFlags)
		{
			return ImageList_LoadImageA(GlobalEnginePtr->GetInstanceDLL(), MAKEINTRESOURCEA(IDB_BITMAP9),
				cx, cGrow, crMask, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_LOADTRANSPARENT);
		}

		void UIThemePatchAdditional::HideOldTimeOfDayComponents()
		{
			// I will hide the old ones, but I will rely on them when sending messages, however, 
			// in the end, I will fake the event to change the time of day

			OldUITimeOfDayComponents.hWndLabel = GetDlgItem(OldUITimeOfDayComponents.hWndToolBar.Handle, 0x16D2);
			OldUITimeOfDayComponents.hWndTrackBar = GetDlgItem(OldUITimeOfDayComponents.hWndToolBar.Handle, 0x16D3);
			OldUITimeOfDayComponents.hWndEdit = GetDlgItem(OldUITimeOfDayComponents.hWndToolBar.Handle, 0x16D4);

			ShowWindow(OldUITimeOfDayComponents.hWndLabel.Handle, SW_HIDE);
			ShowWindow(OldUITimeOfDayComponents.hWndTrackBar.Handle, SW_HIDE);
			ShowWindow(OldUITimeOfDayComponents.hWndEdit.Handle, SW_HIDE);

			POINT Range = 
			{
				(LONG)OldUITimeOfDayComponents.hWndTrackBar.Perform(TBM_GETRANGEMIN, 0, 0),
				(LONG)OldUITimeOfDayComponents.hWndTrackBar.Perform(TBM_GETRANGEMAX, 0, 0)
			};

			NewUITimeOfDayComponents.hWndTrackBar.Perform(TBM_SETRANGE, (WPARAM)TRUE, MAKELPARAM(Range.x, Range.y));
			NewUITimeOfDayComponents.hWndTrackBar.Perform(TBM_SETPOS, (WPARAM)TRUE, (LONG)((10 * (Range.y - Range.x)) / 24));
			NewUITimeOfDayComponents.hWndEdit.Caption = "10.00";
		}

		void UIThemePatchAdditional::HKInitializeTimeOfDay(HWND hDlg, INT nIDDlgItem, FLOAT value, INT a4)
		{
			auto hwndTrackBar = GetDlgItem(hDlg, 0x3F6);
			auto hwndEdit = GetDlgItem(hDlg, 0x3E8);

			INT32 iPos = NewUITimeOfDayComponents.hWndTrackBar.Perform(TBM_GETPOS, 0, 0);

			SendMessage(hwndTrackBar, TBM_SETPOS, TRUE, (LPARAM)iPos);
			SetWindowText(hwndEdit, NewUITimeOfDayComponents.hWndEdit.Caption.c_str());
			EnableWindow(hwndEdit, FALSE);
		}

		void UIThemePatchAdditional::HKSetNewValueTimeOfDay(HWND hDlg, INT nIDDlgItem, FLOAT value, INT a4)
		{
			auto hwndTrackBar = GetDlgItem(hDlg, 0x3F6);
			auto hwndEdit = GetDlgItem(hDlg, 0x3E8);

			INT32 iPos = SendMessage(hwndTrackBar, TBM_GETPOS, 0, 0);

			NewUITimeOfDayComponents.hWndTrackBar.Perform(TBM_SETPOS, TRUE, (LPARAM)iPos);

			CHAR szBuf[24];
			sprintf_s(szBuf, "%.2f", value);
			NewUITimeOfDayComponents.hWndEdit.Caption = szBuf;
			
			((VOID(__fastcall*)(HWND, INT, FLOAT, INT))pointer_UIThemePatchAdditional_sub)(hDlg, nIDDlgItem, value, a4);
		}
	}
}