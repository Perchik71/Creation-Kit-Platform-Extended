// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <commctrl.h>
#include <CKPE.Common.UIBaseWindow.h>
#include <CKPE.Common.UIVarCommon.h>
#include <CKPE.Common.UIMDIClient.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			namespace MDIClient
			{
				static HBRUSH generalMDIBackgroundBrush;

				CKPE_COMMON_API void Initialize(HWND hWindow) noexcept(true)
				{
					generalMDIBackgroundBrush = ::CreateSolidBrush(GetThemeSysColor(ThemeColor::ThemeColor_MDIWindow));
					SetWindowSubclass(hWindow, MDIClientSubclass, 0, 0);
					SetClassLongPtrA(hWindow, GCLP_HBRBACKGROUND, (LONG_PTR)generalMDIBackgroundBrush);
				}

				CKPE_COMMON_API LRESULT CALLBACK MDIClientSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
					UINT_PTR uIdSubclass, DWORD_PTR dwRefData) noexcept(true)
				{
					switch (uMsg)
					{
					case WM_PAINT:
					{
						if (HDC hdc = GetDC(hWnd); hdc)
						{
							RECT windowArea;
							GetClientRect(hWnd, &windowArea);

							FillRect(hdc, &windowArea, generalMDIBackgroundBrush);
							ReleaseDC(hWnd, hdc);
						}
					}
					break;
					}

					return DefSubclassProc(hWnd, uMsg, wParam, lParam);
				}
			}
		}
	}
}