// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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
				using namespace Common;

				// The ability to run CK without access to the Internet and/or a network device.
				Relocation(ID{ 297712, 1477214, 2181936 }, Offset{ 0x7E1, 0x89D, 0x8AC }).Write(JMP);

				return true;
			}
		}
	}
}