// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixSelectedPackageData.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixSelectedPackageData::FixSelectedPackageData() : Common::Patch()
			{
				SetName("Fix Selected Package Data");
			}

			bool FixSelectedPackageData::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixSelectedPackageData::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixSelectedPackageData::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixSelectedPackageData::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixSelectedPackageData::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixSelectedPackageData::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixSelectedPackageData::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for a package's "Selected Package Data" combo box not having a selected value when using a Topic type. 
				// Pointer<->Form ID truncation.
				//
				Relocation(ID(155359), 0x104).WriteFill(NOP, 7);

				return true;
			}
		}
	}
}