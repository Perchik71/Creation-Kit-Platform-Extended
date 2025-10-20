// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.FixSky.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			FixSky::FixSky() : Common::Patch()
			{
				SetName("Fix Sky");
			}

			bool FixSky::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixSky::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixSky::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixSky::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixSky::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixSky::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();
				if ((verPatch != 1) && (verPatch != 2))
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				if (verPatch == 1)
				{
					SafeWrite::Write(__CKPE_OFFSET(0), { 0xEB, 0x4D, 0x90 });
					SafeWrite::Write(__CKPE_OFFSET(1), { 0x48, 0x85, 0xC9, 0x74, 0xB5,
						0x48, 0x8B, 0x01, 0xEB, 0xAA });
				}
				else
				{
					SafeWrite::Write(__CKPE_OFFSET(0), { 0xEB, 0x54, 0x90 });
					SafeWrite::Write(__CKPE_OFFSET(1), { 0x48, 0x85, 0xC9, 0x74, 0xAE,
						0x48, 0x8B, 0x01, 0xEB, 0xA3 });
				}

				return true;
			}
		}
	}
}