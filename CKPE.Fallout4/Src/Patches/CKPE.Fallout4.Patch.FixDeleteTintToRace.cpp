// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.FixDeleteTintToRace.h>

#include <commctrl.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			FixDeleteTintToRace::FixDeleteTintToRace() : Common::Patch()
			{
				SetName("Fixed delete group tinting to race");
			}

			bool FixDeleteTintToRace::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixDeleteTintToRace::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixDeleteTintToRace::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixDeleteTintToRace::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixDeleteTintToRace::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixDeleteTintToRace::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto ver = db->GetVersion();
				if ((ver < 2) && (ver > 3))
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Fixed when you delete a group tinting to race window
				SafeWrite::Write(__CKPE_OFFSET(0), { 0x4D, 0x8B, 0x47, 0x8, 0x4C, 0x89, 0xE2 });
				Detours::DetourCall(__CKPE_OFFSET(1), (uintptr_t)&sub);
				SafeWrite::Write(__CKPE_OFFSET(2), { 0xEB, 0x18 });

				return true;
			}

			void FixDeleteTintToRace::sub(std::int64_t count, std::int64_t item_id, std::uintptr_t listview) noexcept(true)
			{
				if (!count) return;
				for (auto i = 0; i < count; i++)
					ListView_DeleteItem((HWND)listview, item_id);
			}
		}
	}
}