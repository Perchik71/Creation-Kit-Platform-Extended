// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Starfield.VersionLists.h>
#include <Patches/CKPE.Starfield.Patch.RTDynamicCastCrash.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			std::uintptr_t pointer_RTDynamicCastCrashPatch_sub1 = 0;

			RTDynamicCastCrash::RTDynamicCastCrash() : Common::Patch()
			{
				SetName("RTDynamicCast Crash");
			}

			bool RTDynamicCastCrash::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* RTDynamicCastCrash::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool RTDynamicCastCrash::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> RTDynamicCastCrash::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool RTDynamicCastCrash::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() >= VersionLists::EDITOR_STARFIELD_1_14_70_0;
			}

			bool RTDynamicCastCrash::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				pointer_RTDynamicCastCrashPatch_sub1 = Detours::DetourClassJump(__CKPE_OFFSET(0), (std::uintptr_t)&sub);

				return true;
			}

			bool RTDynamicCastCrash::sub(std::int64_t a1, std::int64_t a2, std::int64_t a3, std::int64_t a4)
			{
				// Fixing an error inside dynamic_cast.

				__try
				{
					return fast_call<bool>(pointer_RTDynamicCastCrashPatch_sub1, a1, a2, a3, a4);
				}
				__except (1)
				{
					return false;
				}
			}
		}
	}
}