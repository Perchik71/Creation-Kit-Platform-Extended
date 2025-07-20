// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixSpellEffectsDuration.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixSpellEffectsDuration::FixSpellEffectsDuration() : Common::Patch()
			{
				SetName("Fix a spell effect duration");
			}

			bool FixSpellEffectsDuration::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixSpellEffectsDuration::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixSpellEffectsDuration::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixSpellEffectsDuration::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixSpellEffectsDuration::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixSpellEffectsDuration::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
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