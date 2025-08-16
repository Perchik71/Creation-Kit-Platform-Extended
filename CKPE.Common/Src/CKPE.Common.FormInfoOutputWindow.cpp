// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <memory>
#include <CKPE.Common.FormInfoOutputWindow.h>
#include <CKPE.Common.Interface.h>
#include "..\resource.h"

namespace CKPE
{
	namespace Common
	{
		FormInfoOutputWindow::FormInfoOutputWindow() : CUICustomWindow()
		{}

		INT_PTR FormInfoOutputWindow::OpenModal(const HWND hParentWindow)
		{
			return DialogBox((HINSTANCE)Common::Interface::GetSingleton()->GetInstanceDLL(), 
				MAKEINTRESOURCE(IDD_DIALOG1), hParentWindow, WndProc);
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
				const std::uint32_t param = LOWORD(wParam);
				switch (param)
				{
				case IDOK:
				{
					INT_PTR Ret = 0;
					auto HwndFormId = GetDlgItem(Hwnd, IDC_EDIT1);
					auto len = GetWindowTextLength(HwndFormId);

					if (len > 0)
					{
						auto Buf = std::make_unique<char[]>((std::size_t)len + 1);
						if (Buf)
						{
							if (GetWindowText(HwndFormId, Buf.get(), len + 1) > 0)
							{
								char* EndPref = nullptr;
								Ret = (INT_PTR)strtoul(Buf.get(), &EndPref, 16);
							}
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