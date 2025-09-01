// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Common.AboutWindow.h>
#include <CKPE.Starfield.VersionLists.h>
#include <Patches/CKPE.Starfield.Patch.AboutWindow.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			static std::uintptr_t HKQT5_HkAboutDlgEvent() noexcept(true)
			{
				return (std::uintptr_t)&AboutWindow::QT5Show;
			}

			AboutWindow::AboutWindow() : Common::PatchBaseWindow()
			{
				SetName("About Window");
				Singleton = this;
			}

			bool AboutWindow::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* AboutWindow::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool AboutWindow::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> AboutWindow::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool AboutWindow::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_STARFIELD_LAST;
			}

			bool AboutWindow::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto _interface = CKPE::Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				Detours::DetourCall(__CKPE_OFFSET(0), (uintptr_t)&HKQT5_HkAboutDlgEvent);

				return true;
			}

			LRESULT CALLBACK AboutWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam) noexcept(true)
			{
				return CallWindowProc(Common::AboutWindow::WndProc2, Hwnd, Message, wParam, lParam);
			}

			void AboutWindow::QT5Show() noexcept(true)
			{
				Common::EditorUI::Hook::HKDialogBoxParamA(GetModuleHandleA(nullptr), MAKEINTRESOURCEA(0x64), nullptr,
					(std::uintptr_t)HKWndProc, 0);
			}
		}
	}
}