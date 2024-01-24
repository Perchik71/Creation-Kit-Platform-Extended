// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "..\BaseWindow.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			class LayersWindow : public BaseWindow, public Classes::CUIBaseWindow
			{
			public:
				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;
				virtual bool HasDependencies() const;
				virtual Array<String> GetDependencies() const;

				LayersWindow();

				static LRESULT CALLBACK HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
				static LRESULT sub(HWND TVHwnd, UINT Message, WPARAM wParam, LPARAM lParam);

				static bool MoveWindowBody(HWND hWnd, int X, int Y, int nWidth, int nHeight, bool bRepaint);
				static bool MoveWindowHeader(HWND hWnd, int X, int Y, int nWidth, int nHeight, bool bRepaint);
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				::Core::Classes::UI::CUIBaseControl PushbuttonNewLayer;
				::Core::Classes::UI::CUIBaseControl	PushbuttonAddSelection;
				::Core::Classes::UI::CUIBaseControl EdittextFilter;
				::Core::Classes::UI::CUIBaseControl PushbuttonPlus;
			};

			extern LayersWindow* GlobalLayersWindowPtr;
		}
	}
}