// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "resource.h"
#include "Core/Engine.h"
#include "Editor API/EditorUI.h"
#include "UIThemePatch.h"
#include "UIThemePatchAdditional.h"
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
			return false;
		}

		Array<String> UIThemePatchAdditional::GetDependencies() const
		{
			return {};
		}

		bool UIThemePatchAdditional::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return true;
		}

		bool UIThemePatchAdditional::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			if (lpRelocationDatabaseItem->Version() == 1)
			{
				UIThemePatch::InitializeThread();

				pointer_UIThemePatchAdditional_sub = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0));
				// replace main toolbar
				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(1), (uintptr_t)&Comctl32CreateToolbarEx_1);
				lpRelocator->DetourJump(lpRelocationDatabaseItem->At(2), (uintptr_t)&HideOldTimeOfDayComponents);
				// replace ImageList_LoadImage for item type
				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(3), (uintptr_t)&Comctl32ImageList_LoadImageA_1);

				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(4), (uintptr_t)&HKInitializeTimeOfDay);
				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(5), (uintptr_t)&HKSetNewValueTimeOfDay);

				lpRelocator->PatchNop(lpRelocationDatabaseItem->At(6), 7);				// Prevent setting redundant colors in the condition list view NM_CUSTOMDRAW (breaks dark theme)
				lpRelocator->Patch(lpRelocationDatabaseItem->At(7), { 0x74, 0x20 });	// ^

				return true;
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
			HIMAGELIST hImageList = ImageList_LoadImageA(GlobalEnginePtr->GetInstanceDLL(), MAKEINTRESOURCEA(IDB_BITMAP3), 16, 0,
				RGB(56, 56, 56), IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_LOADTRANSPARENT);

			HWND ret = CreateToolbarEx(hwnd, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, wID, nBitmaps,
				NULL, NULL, lpButtons, iNumButtons - 2 /*delete two divider*/, dxButton, dyButton, dxBitmap, dyBitmap, uStructSize);

			SendMessageA(ret, TB_SETIMAGELIST, 0, (LPARAM)hImageList);
			SendMessageA(ret, TB_SETBITMAPSIZE, 0, MAKELPARAM(16, 16));
			ShowWindow(ret, SW_SHOWNORMAL);
			SetWindowPos(ret, NULL, 0, 24, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

			Initialization(ret);

			return ret;
		}

		HIMAGELIST UIThemePatchAdditional::Comctl32ImageList_LoadImageA_1(HINSTANCE hi, LPCSTR lpbmp, INT cx, INT cGrow,
			COLORREF crMask, UINT uType, UINT uFlags)
		{
			return ImageList_LoadImageA(GlobalEnginePtr->GetInstanceDLL(), MAKEINTRESOURCEA(IDB_BITMAP4),
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