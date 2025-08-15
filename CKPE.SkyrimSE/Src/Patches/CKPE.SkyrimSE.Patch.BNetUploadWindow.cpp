// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.BNetUploadWindow.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			std::uintptr_t pointer_BNetUploadWindow_sub = 0;

			BNetUploadWindow::BNetUploadWindow() : Common::PatchBaseWindow()
			{
				SetName("Upload PS4 modded without assets");
				Singleton = this;
			}

			bool BNetUploadWindow::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* BNetUploadWindow::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool BNetUploadWindow::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> BNetUploadWindow::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool BNetUploadWindow::DoQuery() const noexcept(true)
			{
				// Upload for the editor only version CK 1.6.1130 or newer
				return VersionLists::GetEditorVersion() >= VersionLists::EDITOR_SKYRIM_SE_1_6_1130;
			}

			bool BNetUploadWindow::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto _interface = Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				*(std::uintptr_t*)&_oldWndProc = Detours::DetourClassJump(__CKPE_OFFSET(0), (std::uintptr_t)&HKWndProc);

				Detours::DetourCall(__CKPE_OFFSET(1), (std::uintptr_t)&sub);
				pointer_BNetUploadWindow_sub = __CKPE_OFFSET(2);

				return true;
			}

			void BNetUploadWindow::sub(std::int64_t a1, std::int64_t a2)
			{
				if (BNetUploadWindow::Singleton->_ExcludeAssetsFiles.Checked)
					return;

				fast_call<void>(pointer_BNetUploadWindow_sub, a1, a2);
			}

			INT_PTR CALLBACK BNetUploadWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				if (Message == WM_INITDIALOG)
				{
					auto _This = BNetUploadWindow::Singleton.GetSingleton();

					_This->m_hWnd = Hwnd;
					_This->_Platform_PC = GetDlgItem(Hwnd, UI_PLATFORM_PC_CHECK);
					_This->_Platform_XBOX = GetDlgItem(Hwnd, UI_PLATFORM_XBOX_CHECK);
					_This->_Platform_PS4 = GetDlgItem(Hwnd, UI_PLATFORM_PS4_CHECK);
					_This->_ModName_Platform_PC = GetDlgItem(Hwnd, UI_PLATFORM_PC_MODNAME);
					_This->_Archives_Platform_PC = GetDlgItem(Hwnd, UI_PLATFORM_PC_ARCHIVE);
					_This->_IncludeArchives_Platform_PC = GetDlgItem(Hwnd, UI_INCLUDE_UPLOAD_ARCHIVE_PC_CHECK);
					_This->_CreateESL_Platform_PC = GetDlgItem(Hwnd, UI_PLATFORM_PC_ESL_CHECK);
					_This->_Browse_Platform_PC = GetDlgItem(Hwnd, UI_PLATFORM_PC_BROWSE);
					_This->_Generate_Platform_PC = GetDlgItem(Hwnd, UI_PLATFORM_PC_GENERATE);
					_This->_ModName_Platform_XBOX = GetDlgItem(Hwnd, UI_PLATFORM_XBOX_MODNAME);
					_This->_Archives_Platform_XBOX = GetDlgItem(Hwnd, UI_PLATFORM_XBOX_ARCHIVE);
					_This->_IncludeArchives_Platform_XBOX = GetDlgItem(Hwnd, UI_INCLUDE_UPLOAD_ARCHIVE_XBOX_CHECK);
					_This->_CreateESL_Platform_XBOX = GetDlgItem(Hwnd, UI_PLATFORM_XBOX_ESL_CHECK);
					_This->_Browse_Platform_XBOX = GetDlgItem(Hwnd, UI_PLATFORM_XBOX_BROWSE);
					_This->_Generate_Platform_XBOX = GetDlgItem(Hwnd, UI_PLATFORM_XBOX_GENERATE);
					_This->_ModName_Platform_PS4 = GetDlgItem(Hwnd, UI_PLATFORM_PS4_MODNAME);
					_This->_ExcludeAssetsFiles = GetDlgItem(Hwnd, UI_EXCLUDE_UPLOAD_ASSETS_FILES_CHECK);
					_This->_CreateESL_Platform_PS4 = GetDlgItem(Hwnd, UI_PLATFORM_PS4_ESL_CHECK);
					_This->_Browse_Platform_PS4 = GetDlgItem(Hwnd, UI_PLATFORM_PS4_BROWSE);
				}
				else if (Message == WM_COMMAND)
				{
					switch (LOWORD(wParam))
					{
					case UI_PLATFORM_PC_CHECK:
					{
						auto _This = BNetUploadWindow::Singleton.GetSingleton();
						auto State = _This->_Platform_PC.Checked;
						auto StateExcludeAssets = _This->_ExcludeAssetsFiles.Checked;

						if (!State)
						{
							_This->_CreateESL_Platform_PC.Checked = false;
							_This->_IncludeArchives_Platform_PC.Checked = false;

							// Calling the standard behavior that Bethesda has programmed
							Common::EditorUI::Hook::HKSendMessageA(Hwnd, WM_COMMAND,
								MAKEWPARAM(UI_INCLUDE_UPLOAD_ARCHIVE_PC_CHECK, 0), 0);
						}

						_This->_ModName_Platform_PC.Enabled = State;
						_This->_IncludeArchives_Platform_PC.Enabled = StateExcludeAssets ? false : State;
						_This->_CreateESL_Platform_PC.Enabled = State;
						_This->_Browse_Platform_PC.Enabled = State;
					}
					return S_OK;
					case UI_PLATFORM_XBOX_CHECK:
					{
						auto _This = BNetUploadWindow::Singleton.GetSingleton();
						auto State = _This->_Platform_XBOX.Checked;
						auto StateExcludeAssets = _This->_ExcludeAssetsFiles.Checked;

						if (!State)
						{
							_This->_CreateESL_Platform_XBOX.Checked = false;
							_This->_IncludeArchives_Platform_XBOX.Checked = false;

							// Calling the standard behavior that Bethesda has programmed
							Common::EditorUI::Hook::HKSendMessageA(Hwnd, WM_COMMAND,
								MAKEWPARAM(UI_INCLUDE_UPLOAD_ARCHIVE_XBOX_CHECK, 0), 0);
						}

						_This->_ModName_Platform_XBOX.Enabled = State;
						_This->_IncludeArchives_Platform_XBOX.Enabled = StateExcludeAssets ? false : State;
						_This->_CreateESL_Platform_XBOX.Enabled = State;
						_This->_Browse_Platform_XBOX.Enabled = State;
					}
					return S_OK;
					case UI_PLATFORM_PS4_CHECK:
					{
						auto _This = BNetUploadWindow::Singleton.GetSingleton();
						auto State = _This->_Platform_PS4.Checked;

						_This->_ModName_Platform_PS4.Enabled = State;
						_This->_CreateESL_Platform_PS4.Enabled = State;
						_This->_Browse_Platform_PS4.Enabled = State;
						_This->_ExcludeAssetsFiles.Enabled = State;

						if (!State)
						{
							_This->_ExcludeAssetsFiles.Checked = false;
							Common::EditorUI::Hook::HKSendMessageA(Hwnd, WM_COMMAND,
								MAKEWPARAM(UI_EXCLUDE_UPLOAD_ASSETS_FILES_CHECK, 0), 0);
						}
					}
					return S_OK;
					case UI_EXCLUDE_UPLOAD_ASSETS_FILES_CHECK:
					{
						auto _This = BNetUploadWindow::Singleton.GetSingleton();
						auto State = _This->_ExcludeAssetsFiles.Checked;

						if (State)
						{
							// Unchecked assets for other platforms
							_This->_IncludeArchives_Platform_PC.Checked = false;
							_This->_IncludeArchives_Platform_XBOX.Checked = false;

							// Calling the standard behavior that Bethesda has programmed
							Common::EditorUI::Hook::HKSendMessageA(Hwnd, WM_COMMAND,
								MAKEWPARAM(UI_INCLUDE_UPLOAD_ARCHIVE_PC_CHECK, 0), 0);
							Common::EditorUI::Hook::HKSendMessageA(Hwnd, WM_COMMAND,
								MAKEWPARAM(UI_INCLUDE_UPLOAD_ARCHIVE_XBOX_CHECK, 0), 0);
						}

						// Will not allow the user to change the parameters if this option is enabled

						if (_This->_Platform_PC.Checked)
							_This->_IncludeArchives_Platform_PC.Enabled = !State;
						if (_This->_Platform_XBOX.Checked)
							_This->_IncludeArchives_Platform_XBOX.Enabled = !State;
					}
					return S_OK;
					}
				}

				return CallWindowProc(BNetUploadWindow::Singleton->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}
		}
	}
}