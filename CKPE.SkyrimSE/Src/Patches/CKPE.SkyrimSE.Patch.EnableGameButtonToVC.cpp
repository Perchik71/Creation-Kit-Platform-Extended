// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.EnableGameButtonToVC.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			EnableGameButtonToVC::EnableGameButtonToVC() : Common::Patch()
			{
				SetName("Enable Game Button To VC");
			}

			bool EnableGameButtonToVC::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* EnableGameButtonToVC::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool EnableGameButtonToVC::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> EnableGameButtonToVC::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool EnableGameButtonToVC::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool EnableGameButtonToVC::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				SafeWrite::WriteNop(__CKPE_OFFSET(0), 2);		// Enable push to game button even if version control is disabled
				SafeWrite::Write(__CKPE_OFFSET(1), { 0xEB });	// ^

				return true;
			}
		}
	}
}