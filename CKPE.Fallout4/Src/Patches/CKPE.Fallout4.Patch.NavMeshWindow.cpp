// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.NavMeshWindow.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			NavMeshWindow::NavMeshWindow() : Common::PatchBaseWindow()
			{
				SetName("NavMesh Window");
				Singleton = this;
			}

			bool NavMeshWindow::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* NavMeshWindow::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool NavMeshWindow::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> NavMeshWindow::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool NavMeshWindow::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool NavMeshWindow::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto _interface = Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				*(std::uintptr_t*)&_oldWndProc = Detours::DetourClassJump(__CKPE_OFFSET(0), (std::uintptr_t)&HKWndProc);

				return true;
			}

			INT_PTR CALLBACK NavMeshWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				if (Message == WM_INITDIALOG)
				{
					NavMeshWindow::Singleton->m_hWnd = Hwnd;
					return CallWindowProc(NavMeshWindow::Singleton->GetOldWndProc(), Hwnd, Message, wParam, lParam);
				}
				else if ((Message == WM_COMMAND) && (LOWORD(wParam) == 40770))
				{
					if (MessageBoxA(0, "Do you really want to produce a balance to optimize navmesh?", "Confirmation",
						MB_YESNO | MB_ICONQUESTION) != IDYES)
						return S_OK;
				}

				return CallWindowProc(NavMeshWindow::Singleton->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}
		}
	}
}