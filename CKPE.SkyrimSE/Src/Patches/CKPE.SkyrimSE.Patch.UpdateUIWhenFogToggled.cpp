// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.MainWindow.h>
#include <Patches/CKPE.SkyrimSE.Patch.UpdateUIWhenFogToggled.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			std::uintptr_t pointer_UpdateUIWhenFogToggled_sub = 0;

			UpdateUIWhenFogToggled::UpdateUIWhenFogToggled() : Common::Patch()
			{
				SetName("Update UI When Fog Toggled");
			}

			bool UpdateUIWhenFogToggled::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* UpdateUIWhenFogToggled::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool UpdateUIWhenFogToggled::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> UpdateUIWhenFogToggled::GetDependencies() const noexcept(true)
			{
				return { "Main Window" };
			}

			bool UpdateUIWhenFogToggled::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool UpdateUIWhenFogToggled::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Update the UI options when fog is toggled
				Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&sub);
				pointer_UpdateUIWhenFogToggled_sub = __CKPE_OFFSET(1);

				return true;
			}

			void UpdateUIWhenFogToggled::sub(std::int64_t a1, bool Enable) noexcept(true)
			{
				// Modify the global setting itself then update UI to match
				((void(__fastcall*)(std::int64_t, bool))pointer_UpdateUIWhenFogToggled_sub)(a1, Enable);

				CheckMenuItem(GetMenu(MainWindow::Singleton->Handle), Common::EditorUI::UI_EDITOR_TOGGLEFOG,
					Enable ? MF_CHECKED : MF_UNCHECKED);
			}
		}
	}
}