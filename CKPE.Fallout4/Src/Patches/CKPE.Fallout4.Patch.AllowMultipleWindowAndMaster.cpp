// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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
				using namespace Common;

				const auto rel = Relocation(ID{ 501160, 1942406 }, Offset{ 0x1E3, 0x1EA });
				if (VersionLists::GetEditorVersion() > VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
					rel.Write({ 0xE9, 0xDE, 0x00, 0x00, 0x00, 0x90 });
				else
					rel.Write({ 0xE9, 0xD4, 0x00, 0x00, 0x00, 0x90 });

				Relocation(ID{ 495791, 2054943 }, Offset{ 0x6F7, 0x6F7, 0x703 }).Write(JMP);

				return true;
			}
		}
	}
}