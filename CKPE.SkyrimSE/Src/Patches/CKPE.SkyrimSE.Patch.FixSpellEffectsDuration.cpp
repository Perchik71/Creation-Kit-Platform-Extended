// Copyright © 2023-2025 aka CKPE team. All rights reserved.
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

			bool FixSpellEffectsDuration::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixSpellEffectsDuration::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixSpellEffectsDuration::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for crash when editing a spell effect with a large (>= 1'000'000'000) duration. 
				// WARNING: Stack padding allows the buffer to be up to 12 bytes, 10 are originally reserved.
				//
				auto target = ID(232442);
				Relocation(target, 0x1BD).Write({ 0xBA, 0x0C, 0x00, 0x00, 0x00 });
				Relocation(target, 0x364).Write({ 0xBA, 0x0C, 0x00, 0x00, 0x00 });
				
				return true;
			}
		}
	}
}