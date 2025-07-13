// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Starfield.VersionLists.h>
#include <Patches/CKPE.Starfield.Patch.AllowMultipleWindowAndMaster.h>

namespace CKPE
{
	namespace Starfield
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
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_STARFIELD_LAST;
			}

			bool AllowMultipleWindowAndMaster::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 4)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				SafeWrite::Write(__CKPE_OFFSET(0), { 0xE9, 0x5D, 0x01, 0x00, 0x00, 0x90 });
				SafeWrite::Write(__CKPE_OFFSET(1), { 0xEB });
			}
		}
	}
}