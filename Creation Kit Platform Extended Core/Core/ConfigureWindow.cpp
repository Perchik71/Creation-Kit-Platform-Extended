// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "resource.h"
#include "Engine.h"
#include "ConfigureWindow.h"
#include "Editor API/EditorUI.h"
#include "Editor API/UI/UIGraphics.h"
#include "UITheme/VarCommon.h"
#include "Patches/UIThemePatch.h"
#include "..\version\resource_version2.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		INT_PTR CALLBACK ConfigureWindow::WndProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) {
			static HFONT CaptionFont = nullptr;
			static HBITMAP CKPEFemale = nullptr;
			static HBITMAP ErrorImage = nullptr;

			switch (message)
			{
			case WM_INITDIALOG:
			{
				if (UITheme::IsDarkTheme())
					Patches::UIThemePatch::ApplyThemeForWindow(hwndDlg);

				SetClassLongPtrA(hwndDlg, GCLP_HICON, NULL);
				SetClassLongPtrA(hwndDlg, GCLP_HICONSM, NULL);

				CaptionFont = CreateFontA(48, 0, 0, 0, FW_EXTRALIGHT, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
					DEFAULT_PITCH, "Segoe UI Variable Display");

				auto WndCaption = GetDlgItem(hwndDlg, IDC_STATIC_INSTALL_TITLE);
				SendMessageA(WndCaption, WM_SETFONT, (WPARAM)CaptionFont, TRUE);

				auto WndVersion = GetDlgItem(hwndDlg, IDC_STATIC_INSTALL_VERSION_RUNTIME);
				if (WndVersion)
				{
					char Buffer[96];
					sprintf_s(Buffer, "CKPE Runtime Version: %s", VER_FILE_VERSION_STR);
					SetWindowText(WndVersion, Buffer);
				}

				//auto response = cpr::Get(cpr::Url{ "https://api.github.com/users/perchik71/events/public" });
//
				//MessageBoxA(0, response.text.c_str(), "", 0);


				return TRUE;
			}
			case WM_NOTIFY:
			{
				switch (((LPNMHDR)lParam)->code)
				{
				case NM_CLICK:
				case NM_RETURN:
				{
					PNMLINK pNMLink = (PNMLINK)lParam;
					LITEM   item = pNMLink->item;

					if (item.iLink == 0)
					{
						SHELLEXECUTEINFOW ExecInfo;
						ZeroMemory(&ExecInfo, sizeof(ExecInfo));

						ExecInfo.cbSize = sizeof(ExecInfo);
						ExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
						ExecInfo.lpVerb = L"open";
						ExecInfo.lpFile = item.szUrl;
						ExecInfo.lpParameters = L"";
						ExecInfo.nShow = SW_SHOW;

						ShellExecuteExW(&ExecInfo);
					}
				}
				}

				return TRUE;
			}
			case WM_DESTROY:
			{
				DeleteObject(CaptionFont);

				return TRUE;
			}
			case WM_COMMAND:
			{
				switch (wParam)
				{
				case IDOK:
				case IDCANCEL:
				{
					EndDialog(hwndDlg, wParam);
					return TRUE;
				}
				}
			}
			}
			return FALSE;
		};

		void ConfigureWindow::Show()
		{
			if (_READ_OPTION_BOOL("Functionality", "bShowSettingOnLaunch", true))
			{
				if (UITheme::IsDarkTheme())
					Patches::UIThemePatch::InitializeCurrentThread();

				DialogBoxA(GlobalEnginePtr->GetInstanceDLL(), MAKEINTRESOURCEA(IDD_DIALOGINSTALL), NULL, &WndProc);

				// No show again
				_WRITE_OPTION_BOOL("Functionality", "bShowSettingOnLaunch", false);
				// Save settings
				//GlobalCKPEConfigPtr->Save();

				MessageBoxA(0, "The settings are applied, run the Creation Kit again.\n"
					"Attention: If you are using MO2, do not forget to confirm the changes.", 
					"Creation Kit Platform Extended Configurator", MB_ICONINFORMATION | MB_OK);
				Utils::Quit();
			}
		}


		//using namespace CreationKitPlatformExtended::EditorAPI;
		//using namespace ::Core::Classes::UI;
		//using namespace CreationKitPlatformExtended::UITheme;

		//INT_PTR CALLBACK AboutWindow::WndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
		//{
		//	static uint32_t AppLogoWidth = 448;
		//	static uint32_t AppLogoHeight = 360;
		//	static HBITMAP	BackgroundImage;

		//	BITMAP	bitmap;

		//	switch (Message)
		//	{
		//		case WM_INITDIALOG:
		//		{
		//			switch (Core::GetShortExecutableTypeFromFull(Core::GlobalEnginePtr->GetEditorVersion()))
		//			{
		//			case Core::EDITOR_SHORT_SKYRIM_SE:
		//				BackgroundImage = LoadImageFromResource(Core::GlobalEnginePtr->GetInstanceDLL(), IDB_PNG1, "PNG");
		//				SendMessage(GetDlgItem(Hwnd, 40), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)BackgroundImage);
		//				break;
		//			case Core::EDITOR_SHORT_FALLOUT_C4:
		//				BackgroundImage = LoadImageFromResource(Core::GlobalEnginePtr->GetInstanceDLL(), IDB_PNG2, "PNG");
		//				SendMessage(GetDlgItem(Hwnd, 40), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)BackgroundImage);
		//				break;
		//			case Core::EDITOR_SHORT_STARFIELD:
		//				BackgroundImage = LoadImageFromResource(Core::GlobalEnginePtr->GetInstanceDLL(), IDB_PNG3, "PNG");
		//				SendMessage(GetDlgItem(Hwnd, 40), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)BackgroundImage);
		//				break;
		//			}

		//			// Let's make it so that if the picture is a different size, the window will be size for it
		//			GetObject(BackgroundImage, sizeof(bitmap), &bitmap);
		//			AppLogoWidth = bitmap.bmWidth;
		//			AppLogoHeight = bitmap.bmHeight;

		//			CUIMonitor monitor = Screen.MonitorFromWindow(Hwnd);
		//			CRECT wa = monitor.WorkAreaRect;

		//			MoveWindow(Hwnd,
		//				wa.Left + ((wa.Width - AppLogoWidth) >> 1),
		//				wa.Top + ((wa.Height - AppLogoHeight) >> 1),
		//				AppLogoWidth, AppLogoHeight, TRUE);
		//			ShowWindow(Hwnd, SW_SHOW);
		//			return (INT_PTR)TRUE;
		//		}
		//		case WM_DESTROY:
		//		{
		//			return (INT_PTR)TRUE;
		//		}
		//	}

		//	return (INT_PTR)FALSE;
		//}

		//INT_PTR CALLBACK AboutWindow::WndProc2(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
		//{
		//	if (WndProc(Hwnd, Message, wParam, lParam))
		//		return (INT_PTR)TRUE;

		//	switch (Message)
		//	{
		//		case WM_ACTIVATE:
		//		{
		//			if (LOWORD(wParam) == WA_INACTIVE)
		//			{
		//				EditorUI::HKEndDialog(Hwnd, 1);
		//				return (INT_PTR)TRUE;
		//			}
		//		}
		//	}

		//	return (INT_PTR)FALSE;
		//}
	}
}