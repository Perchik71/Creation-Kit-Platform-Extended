// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "resource.h"
#include "Engine.h"
#include "AboutWindow.h"
#include "Editor API/EditorUI.h"
#include "Editor API/UI/UIGraphics.h"
#include "UITheme/VarCommon.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		using namespace CreationKitPlatformExtended::EditorAPI;
		using namespace ::Core::Classes::UI;
		using namespace CreationKitPlatformExtended::UITheme;

		INT_PTR CALLBACK AboutWindow::WndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
		{
			static uint32_t AppLogoWidth = 448;
			static uint32_t AppLogoHeight = 360;
			static HBITMAP	BackgroundImage;

			BITMAP	bitmap;

			switch (Message)
			{
				case WM_INITDIALOG:
				{
					if (GlobalEnginePtr->GetEditorVersion() <= EDITOR_SKYRIM_SE_LAST)
						BackgroundImage = LoadImageFromResource(Core::GlobalEnginePtr->GetInstanceDLL(), IDB_PNG1, "PNG");
					else
						BackgroundImage = LoadImageFromResource(Core::GlobalEnginePtr->GetInstanceDLL(), IDB_PNG2, "PNG");
				
					SendMessage(GetDlgItem(Hwnd, 40), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)BackgroundImage);

					// Let's make it so that if the picture is a different size, the window will be size for it
					GetObject(BackgroundImage, sizeof(bitmap), &bitmap);
					AppLogoWidth = bitmap.bmWidth;
					AppLogoHeight = bitmap.bmHeight;

					CUIMonitor monitor = Screen.MonitorFromWindow(Hwnd);
					CRECT wa = monitor.WorkAreaRect;

					MoveWindow(Hwnd,
						wa.Left + ((wa.Width - AppLogoWidth) >> 1),
						wa.Top + ((wa.Height - AppLogoHeight) >> 1),
						AppLogoWidth, AppLogoHeight, TRUE);
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
						EditorUI::HKEndDialog(Hwnd, 1);
						return (INT_PTR)TRUE;
					}
				}
			}

			return (INT_PTR)FALSE;
		}
	}
}