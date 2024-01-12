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
			class MainWindow : public BaseWindow, public Classes::CUIMainWindow
			{
			public:
				constexpr static auto UI_EXTMENU_ID = 51001;
				constexpr static auto UI_EXTMENU_SHOWLOG = 51002;
				constexpr static auto UI_EXTMENU_CLEARLOG = 51003;
				constexpr static auto UI_EXTMENU_AUTOSCROLL = 51004;
				constexpr static auto UI_EXTMENU_DUMPRTTI = 51005;
				constexpr static auto UI_EXTMENU_LOADEDESPINFO = 51006;
				constexpr static auto UI_EXTMENU_HARDCODEDFORMS = 51007;
				constexpr static auto UI_EXTMENU_SDM = 51008;

				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;
				virtual bool HasDependencies() const;
				virtual Array<String> GetDependencies() const;

				MainWindow();

				void CreateExtensionMenu(HWND MainWindow, HMENU MainMenu);
				inline HMENU GetExtensionMenuHandle() const { return ExtensionMenuHandle; }

				static LRESULT CALLBACK HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				HMENU ExtensionMenuHandle;
			};

			extern MainWindow* GlobalMainWindowPtr;
		}
	}
}