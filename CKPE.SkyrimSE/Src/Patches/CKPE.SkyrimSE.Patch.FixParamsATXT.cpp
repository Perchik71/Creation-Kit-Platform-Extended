// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixParamsATXT.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixParamsATXT::FixParamsATXT() : Common::Patch()
			{
				SetName("Fix ATXT params sections");
			}

			bool FixParamsATXT::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixParamsATXT::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixParamsATXT::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixParamsATXT::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixParamsATXT::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixParamsATXT::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for TESObjectLAND when writing an ATXT section, a memory multiple of 8 bytes is allocated, 
				// where 2 bytes are the index, 2 bytes are garbage, 4 bytes are transparency 0.0 to 1.0. 
				// In SSEEdit, the second value is shown as unknown, but in fact it is not pre-zeroed memory.
				// let's write the index as 32-bit, thereby zeroing out the memory.
				//
				SafeWrite::Write(__CKPE_OFFSET(0), { 0x89, 0x8C, 0xC4, 0x00, 0x05, 0x00, 0x00, 0x90 });

				return true;
			}
		}
	}
}