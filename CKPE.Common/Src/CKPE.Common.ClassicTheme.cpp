// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <uxtheme.h>
#include <commctrl.h>
#include <CKPE.Common.ClassicTheme.h>

namespace CKPE
{
	namespace Common
	{
		static HHOOK g_hkWndProcClassicHandle = nullptr;

		static LRESULT CALLBACK CallWndProcCallbackClassicTheme(INT nCode, WPARAM wParam, LPARAM lParam) noexcept(true)
		{
			if (nCode == HC_ACTION)
			{
				auto messageData = reinterpret_cast<CWPSTRUCT*>(lParam);
				if ((messageData->message == WM_CREATE) || (messageData->message == WM_INITDIALOG))
					SetWindowTheme(messageData->hwnd, L"", L"");
			}

			return CallNextHookEx(g_hkWndProcClassicHandle, nCode, wParam, lParam);
		}

		void ClassicTheme::InitializeCurrentThread() noexcept(true)
		{
			InitializeThread(GetCurrentThreadId());
		}

		void ClassicTheme::InitializeThread(uint32_t u32ThreadId) noexcept(true)
		{
			g_hkWndProcClassicHandle = SetWindowsHookExA(WH_CALLWNDPROC, CallWndProcCallbackClassicTheme, 
				nullptr, u32ThreadId);
		}

		void ClassicTheme::Hook::Initialize() noexcept(true)
		{
			// Initialise common controls.
			INITCOMMONCONTROLSEX icc;
			memset(&icc, 0, sizeof(INITCOMMONCONTROLSEX));
			icc.dwSize = sizeof(icc);
			icc.dwICC = ICC_WIN95_CLASSES;
			InitCommonControlsEx(&icc);

			InitializeCurrentThread();
		}
	}
}