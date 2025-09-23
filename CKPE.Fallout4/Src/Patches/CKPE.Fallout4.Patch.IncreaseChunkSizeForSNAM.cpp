// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.IncreaseChunkSizeForSNAM.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			constexpr const uint32_t uiMaxChunkSizeSNAM = 0x800;

			IncreaseChunkSizeForSNAM::IncreaseChunkSizeForSNAM() : Common::Patch()
			{
				SetName("Increase chunk size for SNAM");
			}

			bool IncreaseChunkSizeForSNAM::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* IncreaseChunkSizeForSNAM::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool IncreaseChunkSizeForSNAM::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> IncreaseChunkSizeForSNAM::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool IncreaseChunkSizeForSNAM::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool IncreaseChunkSizeForSNAM::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Increasing the size for the SNAM chunk from 512 to 2048
				SafeWrite::Write(__CKPE_OFFSET(0), (std::uint8_t*)&uiMaxChunkSizeSNAM, 4);

				return true;
			}
		}
	}
}