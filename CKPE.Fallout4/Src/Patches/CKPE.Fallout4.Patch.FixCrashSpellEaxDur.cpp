// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.FixCrashSpellEaxDur.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			FixCrashSpellEaxDur::FixCrashSpellEaxDur() : Common::Patch()
			{
				SetName("Fix crash spell effect duration");
			}

			bool FixCrashSpellEaxDur::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixCrashSpellEaxDur::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixCrashSpellEaxDur::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixCrashSpellEaxDur::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixCrashSpellEaxDur::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixCrashSpellEaxDur::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for crash when editing a spell effect with a large (>= 1'000'000'000) duration.
				// WARNING: Stack padding allows the buffer to be up to 12 bytes, 10 are originally reserved.
				//
				SafeWrite::Write(__CKPE_OFFSET(0), { 0xBA, 0x0C, 0x00, 0x00, 0x00 });
				SafeWrite::Write(__CKPE_OFFSET(1), { 0xBA, 0x0C, 0x00, 0x00, 0x00 });

				return true;
			}
		}
	}
}