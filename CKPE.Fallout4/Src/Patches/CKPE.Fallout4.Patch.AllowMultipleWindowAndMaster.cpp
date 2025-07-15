// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.AllowMultipleWindowAndMaster.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			AllowMultipleWindowAndMaster::AllowMultipleWindowAndMaster() : Common::Patch()
			{
				SetName("Allow Multiple Window And Master");
			}

			bool AllowMultipleWindowAndMaster::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* AllowMultipleWindowAndMaster::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool AllowMultipleWindowAndMaster::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> AllowMultipleWindowAndMaster::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool AllowMultipleWindowAndMaster::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool AllowMultipleWindowAndMaster::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto ver = db->GetVersion();
				if ((ver < 2) && (ver > 3))
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				if (ver == 2)
					SafeWrite::Write(__CKPE_OFFSET(0), { 0xE9, 0xDE, 0x00, 0x00, 0x00, 0x90 });
				else
					SafeWrite::Write(__CKPE_OFFSET(0), { 0xE9, 0xD4, 0x00, 0x00, 0x00, 0x90 });

				SafeWrite::Write(__CKPE_OFFSET(1), { 0xEB });

				return true;
			}
		}
	}
}