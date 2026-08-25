// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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

			bool EnableGameButtonToVC::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool EnableGameButtonToVC::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool EnableGameButtonToVC::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				auto target = ID(175922);
				Relocation(target, 0x479).WriteFill(NOP, 2);	// Enable push to game button even if version control is disabled
				Relocation(target, 0x48C).Write(JMP);			// ^

				return true;
			}
		}
	}
}