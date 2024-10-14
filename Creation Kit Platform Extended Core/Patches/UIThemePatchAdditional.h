// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Module.h"
#include "Core/Relocator.h"
#include "Core/RelocationDatabase.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		using namespace CreationKitPlatformExtended::Core;

		class UIThemePatchAdditional : public Module
		{
		public:
			UIThemePatchAdditional();

			virtual bool HasOption() const;
			virtual bool HasCanRuntimeDisabled() const;
			virtual const char* GetOptionName() const;
			virtual const char* GetName() const;
			virtual bool HasDependencies() const;
			virtual Array<String> GetDependencies() const;

			static HWND Comctl32CreateToolbarEx_1(HWND hwnd, DWORD ws, UINT wID, INT nBitmaps, 
				HINSTANCE hBMInst, UINT_PTR wBMID, LPCTBBUTTON lpButtons,
				INT iNumButtons, INT dxButton, INT dyButton, INT dxBitmap, INT dyBitmap, UINT uStructSize);
			static HWND Comctl32CreateToolbarEx_NavMesh(HWND hwnd, DWORD ws, UINT wID, INT nBitmaps,
				HINSTANCE hBMInst, UINT_PTR wBMID, LPTBBUTTON lpButtons,
				INT iNumButtons, INT dxButton, INT dyButton, INT dxBitmap, INT dyBitmap, UINT uStructSize);
			static HIMAGELIST Comctl32ImageList_LoadImageA_1(HINSTANCE hi, LPCSTR lpbmp, INT cx, INT cGrow,
				COLORREF crMask, UINT uType, UINT uFlags);
			static HIMAGELIST Comctl32ImageList_LoadImageA_2(HINSTANCE hi, LPCSTR lpbmp, INT cx, INT cGrow,
				COLORREF crMask, UINT uType, UINT uFlags);
			static void HideOldTimeOfDayComponents();
			static void HKInitializeTimeOfDay(HWND hDlg, INT nIDDlgItem, FLOAT value, INT a4);
			static void HKSetNewValueTimeOfDay(HWND hDlg, INT nIDDlgItem, FLOAT value, INT a4);
		protected:
			virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const;
			virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
		private:
			UIThemePatchAdditional(const UIThemePatchAdditional&) = default;
			UIThemePatchAdditional& operator=(const UIThemePatchAdditional&) = default;
		};
	}
}