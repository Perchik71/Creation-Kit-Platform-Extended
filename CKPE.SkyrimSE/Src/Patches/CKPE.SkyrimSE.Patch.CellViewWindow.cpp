// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.Asserts.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.CellViewWindow.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			CellViewWindow* GlobalCellViewWindowPtr = nullptr;
			uintptr_t pointer_CellViewWindow_sub1 = 0;
			uintptr_t pointer_CellViewWindow_sub2 = 0;
			uintptr_t pointer_CellViewWindow_sub3 = 0;
			char* str_CellViewWindow_Filter = nullptr;
			char* str_CellViewWindow_FilterUser = nullptr;

			CellViewWindow::CellViewWindow() : Common::PatchBaseWindow()
			{
				SetName("Cell View Window");
			}

			bool CellViewWindow::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* CellViewWindow::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool CellViewWindow::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> CellViewWindow::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool CellViewWindow::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CellViewWindow::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Assert if D3D11 FL11 features are not supported
				//
				//Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&sub);

				return true;
			}

			/*void CellViewWindow::sub() noexcept(true)
			{
				CKPE_ASSERT_MSG(false,
					"Creation Kit renderer initialization failed because your graphics card doesn't support D3D11 Feature Level 11 (FL11_0).\n"
					"Updating your drivers may fix this.");
			}*/
		}
	}
}