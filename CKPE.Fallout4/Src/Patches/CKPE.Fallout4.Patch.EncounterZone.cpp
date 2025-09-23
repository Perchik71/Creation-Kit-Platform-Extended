// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.MessageBox.h>
#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.EncounterZone.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			std::uintptr_t pointer_FixEncounterZone_sub = 0;

			EncounterZone::EncounterZone() : Common::Patch()
			{
				SetName("Fix Encounter Zone");
			}

			bool EncounterZone::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* EncounterZone::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool EncounterZone::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> EncounterZone::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool EncounterZone::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool EncounterZone::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Fix Encounter Zone
				// TBM_GETPOS = wParam must be zero.
				SafeWrite::Write(__CKPE_OFFSET(0), { 0x00 });
				// Fix Encounter Zone
				Detours::DetourCall(__CKPE_OFFSET(1), (std::uintptr_t)&sub);
				pointer_FixEncounterZone_sub = __CKPE_OFFSET(2);

				return true;
			}

			void EncounterZone::sub(void* Class, void* Dialog, std::uint8_t Level) noexcept(true)
			{
				if (!Class)
				{
					MessageBox::OpenWarning(
						"You are making mistakes!\n\n"
						"Set the ID and location and click Ok, only after that you can open and click on this button.");
					return;
				}

				fast_call<void>(pointer_FixEncounterZone_sub, Class, Dialog, Level);
			}
		}
	}
}