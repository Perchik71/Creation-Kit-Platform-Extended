// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "resource.h"
#include "Engine.h"
#include "FormInfoOutputWindow.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		using namespace ::Core::Classes::UI;

		FormInfoOutputWindow::FormInfoOutputWindow() : CUICustomWindow()
		{}

		INT_PTR FormInfoOutputWindow::OpenModal(const HWND hParentWindow)
		{
			return DialogBox(GlobalEnginePtr->GetInstanceDLL(), MAKEINTRESOURCE(IDD_DIALOG1), hParentWindow, WndProc);
		}

		INT_PTR CALLBACK FormInfoOutputWindow::WndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
		{
			switch (Message)
			{
				case WM_INITDIALOG:
				{
					SetWindowText(GetDlgItem(Hwnd, IDC_EDIT1), "");
				}
				return (INT_PTR)TRUE;
				case WM_CLOSE:
				{
					EndDialog(Hwnd, 0);
				}
				case WM_COMMAND:
				{
					const uint32_t param = LOWORD(wParam);
					switch (param)
					{
						case IDOK:
						{
							INT_PTR Ret = 0;
							auto HwndFormId = GetDlgItem(Hwnd, IDC_EDIT1);
							auto len = GetWindowTextLength(HwndFormId);
							
							if (len > 0)
							{
								char* Buf = (char*)GlobalMemoryManagerPtr->MemAlloc((size_t)len + 1);
								if (Buf)
								{
									if (GetWindowText(HwndFormId, Buf, len + 1) > 0)
									{
										char* EndPref = nullptr;
										Ret = (INT_PTR)strtoul(Buf, &EndPref, 16);
									}

									GlobalMemoryManagerPtr->MemFree(Buf);
								}
							}

							EndDialog(Hwnd, Ret);
						}
						return (INT_PTR)TRUE;
						case IDCANCEL:
						{
							EndDialog(Hwnd, 0);
						}
						return (INT_PTR)TRUE;
					}
				}
				return (INT_PTR)TRUE;
			}

			return (INT_PTR)FALSE;
		}
	}
}