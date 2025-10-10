// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.FixRecursiveSorting.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			FixRecursiveSorting::FixRecursiveSorting() : Common::Patch()
			{
				SetName("Fixed recursive sorting function");
			}

			bool FixRecursiveSorting::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixRecursiveSorting::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixRecursiveSorting::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixRecursiveSorting::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixRecursiveSorting::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixRecursiveSorting::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Fix for crash (recursive sorting function stack overflow) when saving certain ESP files (i.e SimSettlements.esp)
				Detours::DetourJump(__CKPE_OFFSET(0), (std::uintptr_t)&sub<class TESForm*>);
				SafeWrite::Write(__CKPE_OFFSET(1), { 0xC3 });

				return true;
			}
		}
	}
}