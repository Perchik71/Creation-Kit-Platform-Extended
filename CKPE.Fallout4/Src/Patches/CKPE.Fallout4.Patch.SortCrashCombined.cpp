// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.SortCrashCombined.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			std::uintptr_t pointer_SortCrashCombinedPatch_sub = 0;

			SortCrashCombined::SortCrashCombined() : Common::Patch()
			{
				SetName("Sort Crash Combined");
			}

			bool SortCrashCombined::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* SortCrashCombined::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool SortCrashCombined::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> SortCrashCombined::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool SortCrashCombined::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool SortCrashCombined::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				*(std::uintptr_t*)&pointer_SortCrashCombinedPatch_sub =
					Detours::DetourClassJump(__CKPE_OFFSET(0), &sub);

				return true;
			}

			std::int32_t SortCrashCombined::sub(const void* lsb, const void* rsb, std::size_t size) noexcept(true)
			{
				if (!lsb)
					return -1;
				else if (!rsb)
					return 1;
				else
					return fast_call<std::int32_t>(pointer_SortCrashCombinedPatch_sub, lsb, rsb, size);
			}
		}
	}
}