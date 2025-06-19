// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <cstdint>
#include <CKPE.Common.AboutWindow.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Graphics.h>
#include "..\resource.h"

namespace CKPE
{
	namespace Common
	{
		INT_PTR CALLBACK AboutWindow::WndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
		{
			static uint32_t AppLogoWidth = 0;
			static uint32_t AppLogoHeight = 0;
			static Bitmap bm;

			BITMAP bitmap{};

			switch (Message)
			{
				case WM_INITDIALOG:
				{
					DWORD dwId = 0;
					switch (std::rand() % 4)
					{
					case 0:
						dwId = IDB_PNG1;
						break;
					case 1:
						dwId = IDB_PNG2;
						break;
					case 2:
						dwId = IDB_PNG3;
						break;
					case 3:
						dwId = IDB_PNG6;
						break;
					}

					bm.LoadFromResource((THandle)Interface::GetSingleton()->GetInstanceDLL(), dwId, L"PNG");
					SendMessageA(GetDlgItem(Hwnd, 40), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bm.Handle);

					// Let's make it so that if the picture is a different size, the window will be size for it
					GetObjectA(bm.Handle, sizeof(bitmap), &bitmap);
					AppLogoWidth = bitmap.bmWidth;
					AppLogoHeight = bitmap.bmHeight;

					Monitor monitor = Screen::GetSingleton()->MonitorFromWindow((THandle)Hwnd);
					CRECT wa = monitor.WorkAreaRect;

					SetWindowPos(Hwnd, nullptr, 0, 0, AppLogoWidth, AppLogoHeight, SWP_NOMOVE | SWP_NOOWNERZORDER);
					SetWindowRgn(Hwnd, CreateRoundRectRgn(0, 0, AppLogoWidth, AppLogoHeight, 30, 30), TRUE);
					ShowWindow(Hwnd, SW_SHOW);
					return (INT_PTR)TRUE;
				}
				case WM_DESTROY:
				{
					return (INT_PTR)TRUE;
				}
			}

			return (INT_PTR)FALSE;
		}

		INT_PTR CALLBACK AboutWindow::WndProc2(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
		{
			if (WndProc(Hwnd, Message, wParam, lParam))
				return (INT_PTR)TRUE;

			switch (Message)
			{
				case WM_ACTIVATE:
				{
					if (LOWORD(wParam) == WA_INACTIVE)
					{
						EditorUI::Hook::HKEndDialog(Hwnd, 1);
						return (INT_PTR)TRUE;
					}
				}
			}

			return (INT_PTR)FALSE;
		}
	}
}