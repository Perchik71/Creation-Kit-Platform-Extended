// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <windows.h>
#include <commctrl.h>
#include <CKPE.Common.Patch.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			class ModernThemePatchAdditional : public Common::Patch
			{
				static LRESULT DoCustomDrawListView(HWND hWindow, LPNMLVCUSTOMDRAW lpListView,
					bool& bReturn) noexcept(true);

				ModernThemePatchAdditional(const ModernThemePatchAdditional&) = delete;
				ModernThemePatchAdditional& operator=(const ModernThemePatchAdditional&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				ModernThemePatchAdditional();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				static HWND Comctl32CreateToolbarEx_1(HWND hwnd, DWORD ws, UINT wID, INT nBitmaps,
					HINSTANCE hBMInst, UINT_PTR wBMID, LPCTBBUTTON lpButtons,
					INT iNumButtons, INT dxButton, INT dyButton, INT dxBitmap, INT dyBitmap, UINT uStructSize) noexcept(true);
				static HWND Comctl32CreateToolbarEx_NavMesh(HWND hwnd, DWORD ws, UINT wID, INT nBitmaps,
					HINSTANCE hBMInst, UINT_PTR wBMID, LPTBBUTTON lpButtons,
					INT iNumButtons, INT dxButton, INT dyButton, INT dxBitmap, INT dyBitmap, UINT uStructSize) noexcept(true);
				static HIMAGELIST Comctl32ImageList_LoadImageA_1(HINSTANCE hi, LPCSTR lpbmp, INT cx, INT cGrow,
					COLORREF crMask, UINT uType, UINT uFlags) noexcept(true);
				static HIMAGELIST Comctl32ImageList_LoadImageA_2(HINSTANCE hi, LPCSTR lpbmp, INT cx, INT cGrow,
					COLORREF crMask, UINT uType, UINT uFlags) noexcept(true);
				static HIMAGELIST Comctl32ImageList_LoadImageA_3(HINSTANCE hi, LPCSTR lpbmp, INT cx, INT cGrow,
					COLORREF crMask, UINT uType, UINT uFlags) noexcept(true);
				static void HideOldTimeOfDayComponents() noexcept(true);
				static void HKInitializeTimeOfDay(HWND hDlg, INT nIDDlgItem, FLOAT value, INT a4) noexcept(true);
				static void HKSetNewValueTimeOfDay(HWND hDlg, INT nIDDlgItem, FLOAT value, INT a4) noexcept(true);
			};
		}
	}
}