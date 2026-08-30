// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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
			using TUpdateUIWhenFogToggled_sub = void(std::int64_t, bool);
			
			static std::function<TUpdateUIWhenFogToggled_sub> UpdateUIWhenFogToggled_sub;

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

			bool UpdateUIWhenFogToggled::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool UpdateUIWhenFogToggled::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool UpdateUIWhenFogToggled::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				// Update the UI options when fog is toggled
				UpdateUIWhenFogToggled_sub = reinterpret_cast<TUpdateUIWhenFogToggled_sub*>
					(Relocation(ID(235802), 0x19).WriteCall(&sub));

				return true;
			}

			void UpdateUIWhenFogToggled::sub(std::int64_t a1, bool Enable) noexcept(true)
			{
				// Modify the global setting itself then update UI to match
				UpdateUIWhenFogToggled_sub(a1, Enable);

				CheckMenuItem(GetMenu(MainWindow::Singleton->Handle), Common::EditorUI::UI_EDITOR_TOGGLEFOG,
					Enable ? MF_CHECKED : MF_UNCHECKED);
			}
		}
	}
}