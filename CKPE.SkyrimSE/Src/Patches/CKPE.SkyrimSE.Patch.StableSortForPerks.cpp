// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Asserts.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.StableSortForPerks.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			StableSortForPerks::StableSortForPerks() : Common::Patch()
			{
				SetName("Stable Sort For Perks");
			}

			bool StableSortForPerks::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* StableSortForPerks::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool StableSortForPerks::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> StableSortForPerks::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool StableSortForPerks::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool StableSortForPerks::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool StableSortForPerks::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				// Stable sort for perk entry window
				Relocation(ID(389022), 0x39).WriteCall(&ArrayQuickSortRecursive<class BGSEntryPointPerkEntry*, true>);

				return true;
			}
		}
	}
}