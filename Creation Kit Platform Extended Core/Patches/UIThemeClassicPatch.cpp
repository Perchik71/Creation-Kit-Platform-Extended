// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/EditorUI.h"
#include "Editor API/UI/UIBaseWindow.h"
#include "UIThemeClassicPatch.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		UIThemeClassicPatch::UIThemeClassicPatch() : Module(GlobalEnginePtr)
		{}

		bool UIThemeClassicPatch::HasOption() const
		{
			return true;
		}

		bool UIThemeClassicPatch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* UIThemeClassicPatch::GetOptionName() const
		{
			return "CreationKit:bUIClassicTheme";
		}

		const char* UIThemeClassicPatch::GetName() const
		{
			return "UI Classic Theme";
		}

		bool UIThemeClassicPatch::HasDependencies() const
		{
			return false;
		}

		Array<String> UIThemeClassicPatch::GetDependencies() const
		{
			return {};
		}

		bool UIThemeClassicPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return true;
		}

		bool UIThemeClassicPatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			// Initialise common controls.
			INITCOMMONCONTROLSEX icc;
			memset(&icc, 0, sizeof(INITCOMMONCONTROLSEX));
			icc.dwSize = sizeof(icc);
			icc.dwICC = ICC_WIN95_CLASSES;
			InitCommonControlsEx(&icc);

			InitializeThread();

			return true;
		}

		bool UIThemeClassicPatch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}

		void UIThemeClassicPatch::InitializeThread()
		{
			SetWindowsHookExA(WH_CALLWNDPROC, CallWndProcCallback, NULL, GetCurrentThreadId());
		}

		LRESULT CALLBACK UIThemeClassicPatch::CallWndProcCallback(INT nCode, WPARAM wParam, LPARAM lParam)
		{
			if (nCode == HC_ACTION) 
			{
				auto messageData = reinterpret_cast<CWPSTRUCT*>(lParam);
				if ((messageData->message == WM_CREATE) || (messageData->message == WM_INITDIALOG))
					SetWindowTheme(messageData->hwnd, L"", L"");
			}

			return CallNextHookEx(NULL, nCode, wParam, lParam);
		}
	}
}