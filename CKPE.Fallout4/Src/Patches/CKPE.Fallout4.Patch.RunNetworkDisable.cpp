// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.RunNetworkDisable.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			RunNetworkDisable::RunNetworkDisable() : Common::Patch()
			{
				SetName("Run Network Disable");
			}

			bool RunNetworkDisable::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* RunNetworkDisable::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool RunNetworkDisable::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> RunNetworkDisable::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool RunNetworkDisable::DoQuery() const noexcept(true)
			{
				auto ver = VersionLists::GetEditorVersion();
				return (ver <= VersionLists::EDITOR_FALLOUT_C4_LAST) &&
					(ver != VersionLists::EDITOR_FALLOUT_C4_1_10_943_1);
			}

			bool RunNetworkDisable::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// The ability to run CK without access to the Internet and/or a network device.
				SafeWrite::Write(__CKPE_OFFSET(0), { 0xEB });

				return true;
			}
		}
	}
}