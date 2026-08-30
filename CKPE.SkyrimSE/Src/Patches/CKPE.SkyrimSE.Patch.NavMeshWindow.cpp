// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.MessageBox.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.NavMeshWindow.h>

namespace CKPE
{
	namespace SkyrimSE
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

			bool NavMeshWindow::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool NavMeshWindow::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool NavMeshWindow::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				*(std::uintptr_t*)&_oldWndProc = Relocation(ID(4516)).WriteJump(&HKWndProc);

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
					if (MessageBox::OpenQuestion("Do you really want to produce a balance to optimize navmesh?") != 
						MessageBox::Result::mrYes)
						return S_OK;
				}

				return CallWindowProc(NavMeshWindow::Singleton->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}
		}
	}
}