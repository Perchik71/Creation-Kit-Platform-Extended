// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.AllowMultipleWindowAndMaster.h>

namespace CKPE
{
	namespace SkyrimSE
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

			bool AllowMultipleWindowAndMaster::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool AllowMultipleWindowAndMaster::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool AllowMultipleWindowAndMaster::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				Relocation(ID(277090), 0x2DA).Write({ 0xE9, 0xBA, 0x00, 0x00, 0x00, 0x90 });
				Relocation(ID(326873), 0x7FB).Write({ 0xEB });

				return true;
			}
		}
	}
}