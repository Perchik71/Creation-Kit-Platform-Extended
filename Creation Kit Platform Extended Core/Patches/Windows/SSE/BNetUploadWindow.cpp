// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "BNetUploadWindow.h"
#include "Editor API/EditorUI.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			constexpr uint16_t UI_PLATFORM_PC_CHECK = 6333;
			constexpr uint16_t UI_PLATFORM_XBOX_CHECK = 6336;
			constexpr uint16_t UI_PLATFORM_PS4_CHECK = 6339;
			constexpr uint16_t UI_PLATFORM_PC_MODNAME = 6334;
			constexpr uint16_t UI_PLATFORM_PC_BROWSE = 3681;
			constexpr uint16_t UI_PLATFORM_PC_ESL_CHECK = 1007;
			constexpr uint16_t UI_PLATFORM_PC_ARCHIVE = 6335;
			constexpr uint16_t UI_INCLUDE_UPLOAD_ARCHIVE_PC_CHECK = 1010;
			constexpr uint16_t UI_PLATFORM_PC_GENERATE = 3682;
			constexpr uint16_t UI_PLATFORM_XBOX_MODNAME = 6337;
			constexpr uint16_t UI_PLATFORM_XBOX_BROWSE = 3683;
			constexpr uint16_t UI_PLATFORM_XBOX_ESL_CHECK = 1008;
			constexpr uint16_t UI_PLATFORM_XBOX_ARCHIVE = 6338;
			constexpr uint16_t UI_INCLUDE_UPLOAD_ARCHIVE_XBOX_CHECK = 1011;
			constexpr uint16_t UI_PLATFORM_XBOX_GENERATE = 3684;
			constexpr uint16_t UI_PLATFORM_PS4_MODNAME = 6340;
			constexpr uint16_t UI_PLATFORM_PS4_BROWSE = 3685;
			constexpr uint16_t UI_PLATFORM_PS4_ESL_CHECK = 1009;
			constexpr uint16_t UI_EXCLUDE_UPLOAD_ASSETS_FILES_CHECK = 1012;

			BNetUploadWindow* GlobalBNetUploadWindowPtr = nullptr;
			uintptr_t pointer_BNetUploadWindow_sub = 0;

			bool BNetUploadWindow::HasOption() const
			{
				return false;
			}

			bool BNetUploadWindow::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* BNetUploadWindow::GetOptionName() const
			{
				return nullptr;
			}

			const char* BNetUploadWindow::GetName() const
			{
				return "Upload PS4 modded without assets";
			}

			bool BNetUploadWindow::HasDependencies() const
			{
				return false;
			}

			Array<String> BNetUploadWindow::GetDependencies() const
			{
				return {};
			}

			bool BNetUploadWindow::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				// Upload for the editor only version CK 1.6.1130
				return eEditorCurrentVersion == EDITOR_SKYRIM_SE_1_6_1130;
			}

			bool BNetUploadWindow::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					*(uintptr_t*)&_oldWndProc =
						Detours::X64::DetourFunctionClass(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0)), 
							(uintptr_t)&HKWndProc);

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(1), (uintptr_t)&sub);
					pointer_BNetUploadWindow_sub = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(2));

					return true;
				}

				return false;
			}

			bool BNetUploadWindow::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			BNetUploadWindow::BNetUploadWindow() : BaseWindow(), Classes::CUIBaseWindow()
			{
				Assert(!GlobalBNetUploadWindowPtr);
				GlobalBNetUploadWindowPtr = this;
			}

			void BNetUploadWindow::sub(int64_t a1, int64_t a2)
			{
				if (GlobalBNetUploadWindowPtr->_ExcludeAssetsFiles.Checked)
					return;

				fastCall<void>(pointer_BNetUploadWindow_sub, a1, a2);
			}

			LRESULT CALLBACK BNetUploadWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				if (Message == WM_INITDIALOG) 
				{
					GlobalBNetUploadWindowPtr->m_hWnd = Hwnd;
	
					GlobalBNetUploadWindowPtr->_Platform_PC = GetDlgItem(Hwnd, UI_PLATFORM_PC_CHECK);
					GlobalBNetUploadWindowPtr->_Platform_XBOX = GetDlgItem(Hwnd, UI_PLATFORM_XBOX_CHECK);
					GlobalBNetUploadWindowPtr->_Platform_PS4 = GetDlgItem(Hwnd, UI_PLATFORM_PS4_CHECK);
					GlobalBNetUploadWindowPtr->_ModName_Platform_PC = GetDlgItem(Hwnd, UI_PLATFORM_PC_MODNAME);
					GlobalBNetUploadWindowPtr->_Archives_Platform_PC = GetDlgItem(Hwnd, UI_PLATFORM_PC_ARCHIVE);
					GlobalBNetUploadWindowPtr->_IncludeArchives_Platform_PC = GetDlgItem(Hwnd, UI_INCLUDE_UPLOAD_ARCHIVE_PC_CHECK);
					GlobalBNetUploadWindowPtr->_CreateESL_Platform_PC = GetDlgItem(Hwnd, UI_PLATFORM_PC_ESL_CHECK);
					GlobalBNetUploadWindowPtr->_Browse_Platform_PC = GetDlgItem(Hwnd, UI_PLATFORM_PC_BROWSE);
					GlobalBNetUploadWindowPtr->_Generate_Platform_PC = GetDlgItem(Hwnd, UI_PLATFORM_PC_GENERATE);
					GlobalBNetUploadWindowPtr->_ModName_Platform_XBOX = GetDlgItem(Hwnd, UI_PLATFORM_XBOX_MODNAME);
					GlobalBNetUploadWindowPtr->_Archives_Platform_XBOX = GetDlgItem(Hwnd, UI_PLATFORM_XBOX_ARCHIVE);
					GlobalBNetUploadWindowPtr->_IncludeArchives_Platform_XBOX = GetDlgItem(Hwnd, UI_INCLUDE_UPLOAD_ARCHIVE_XBOX_CHECK);
					GlobalBNetUploadWindowPtr->_CreateESL_Platform_XBOX = GetDlgItem(Hwnd, UI_PLATFORM_XBOX_ESL_CHECK);
					GlobalBNetUploadWindowPtr->_Browse_Platform_XBOX = GetDlgItem(Hwnd, UI_PLATFORM_XBOX_BROWSE);
					GlobalBNetUploadWindowPtr->_Generate_Platform_XBOX = GetDlgItem(Hwnd, UI_PLATFORM_XBOX_GENERATE);
					GlobalBNetUploadWindowPtr->_ModName_Platform_PS4 = GetDlgItem(Hwnd, UI_PLATFORM_PS4_MODNAME);
					GlobalBNetUploadWindowPtr->_ExcludeAssetsFiles = GetDlgItem(Hwnd, UI_EXCLUDE_UPLOAD_ASSETS_FILES_CHECK);
					GlobalBNetUploadWindowPtr->_CreateESL_Platform_PS4 = GetDlgItem(Hwnd, UI_PLATFORM_PS4_ESL_CHECK);
					GlobalBNetUploadWindowPtr->_Browse_Platform_PS4 = GetDlgItem(Hwnd, UI_PLATFORM_PS4_BROWSE);
				}
				else if (Message == WM_COMMAND)
				{
					switch (LOWORD(wParam))
					{
						case UI_PLATFORM_PC_CHECK:
						{
							auto State = GlobalBNetUploadWindowPtr->_Platform_PC.Checked;
							auto StateExcludeAssets = GlobalBNetUploadWindowPtr->_ExcludeAssetsFiles.Checked;
							
							if (!State)
							{
								GlobalBNetUploadWindowPtr->_CreateESL_Platform_PC.Checked = false;
								GlobalBNetUploadWindowPtr->_IncludeArchives_Platform_PC.Checked = false;

								// Calling the standard behavior that Bethesda has programmed
								EditorAPI::EditorUI::HKSendMessageA(Hwnd, WM_COMMAND,
									MAKEWPARAM(UI_INCLUDE_UPLOAD_ARCHIVE_PC_CHECK, 0), 0);
							}

							GlobalBNetUploadWindowPtr->_ModName_Platform_PC.Enabled = State;
							GlobalBNetUploadWindowPtr->_IncludeArchives_Platform_PC.Enabled = StateExcludeAssets ? false : State;
							GlobalBNetUploadWindowPtr->_CreateESL_Platform_PC.Enabled = State;
							GlobalBNetUploadWindowPtr->_Browse_Platform_PC.Enabled = State;
						}
						return S_OK;
						case UI_PLATFORM_XBOX_CHECK:
						{
							auto State = GlobalBNetUploadWindowPtr->_Platform_XBOX.Checked;
							auto StateExcludeAssets = GlobalBNetUploadWindowPtr->_ExcludeAssetsFiles.Checked;

							if (!State)
							{
								GlobalBNetUploadWindowPtr->_CreateESL_Platform_XBOX.Checked = false;
								GlobalBNetUploadWindowPtr->_IncludeArchives_Platform_XBOX.Checked = false;

								// Calling the standard behavior that Bethesda has programmed
								EditorAPI::EditorUI::HKSendMessageA(Hwnd, WM_COMMAND,
									MAKEWPARAM(UI_INCLUDE_UPLOAD_ARCHIVE_XBOX_CHECK, 0), 0);
							}

							GlobalBNetUploadWindowPtr->_ModName_Platform_XBOX.Enabled = State;
							GlobalBNetUploadWindowPtr->_IncludeArchives_Platform_XBOX.Enabled = StateExcludeAssets ? false : State;
							GlobalBNetUploadWindowPtr->_CreateESL_Platform_XBOX.Enabled = State;
							GlobalBNetUploadWindowPtr->_Browse_Platform_XBOX.Enabled = State;
						}
						return S_OK;
						case UI_PLATFORM_PS4_CHECK:
						{
							auto State = GlobalBNetUploadWindowPtr->_Platform_PS4.Checked;

							GlobalBNetUploadWindowPtr->_ModName_Platform_PS4.Enabled = State;	
							GlobalBNetUploadWindowPtr->_CreateESL_Platform_PS4.Enabled = State;
							GlobalBNetUploadWindowPtr->_Browse_Platform_PS4.Enabled = State;
							GlobalBNetUploadWindowPtr->_ExcludeAssetsFiles.Enabled = State;
								
							if (!State)
							{
								GlobalBNetUploadWindowPtr->_ExcludeAssetsFiles.Checked = false;
								EditorAPI::EditorUI::HKSendMessageA(Hwnd, WM_COMMAND,
									MAKEWPARAM(UI_EXCLUDE_UPLOAD_ASSETS_FILES_CHECK, 0), 0);
							}
						}
						return S_OK;
						case UI_EXCLUDE_UPLOAD_ASSETS_FILES_CHECK:
						{
							auto State = GlobalBNetUploadWindowPtr->_ExcludeAssetsFiles.Checked;

							if (State)
							{
								// Unchecked assets for other platforms
								GlobalBNetUploadWindowPtr->_IncludeArchives_Platform_PC.Checked = false;
								GlobalBNetUploadWindowPtr->_IncludeArchives_Platform_XBOX.Checked = false;

								// Calling the standard behavior that Bethesda has programmed
								EditorAPI::EditorUI::HKSendMessageA(Hwnd, WM_COMMAND,
									MAKEWPARAM(UI_INCLUDE_UPLOAD_ARCHIVE_PC_CHECK, 0), 0);
								EditorAPI::EditorUI::HKSendMessageA(Hwnd, WM_COMMAND,
									MAKEWPARAM(UI_INCLUDE_UPLOAD_ARCHIVE_XBOX_CHECK, 0), 0);
							}

							// Will not allow the user to change the parameters if this option is enabled

							if (GlobalBNetUploadWindowPtr->_Platform_PC.Checked)
								GlobalBNetUploadWindowPtr->_IncludeArchives_Platform_PC.Enabled = !State;
							if (GlobalBNetUploadWindowPtr->_Platform_XBOX.Checked)
								GlobalBNetUploadWindowPtr->_IncludeArchives_Platform_XBOX.Enabled = !State;
						}
						return S_OK;
					}
				}

				return CallWindowProc(GlobalBNetUploadWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}
		}
	}
}