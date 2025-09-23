// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.FixCrashMapMarkerCmd.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			std::uintptr_t pointer_FixCrashMapMarkerCmd_sub = 0;

			FixCrashMapMarkerCmd::FixCrashMapMarkerCmd() : Common::Patch()
			{
				SetName("Fixed MapMaker command line option");
			}

			bool FixCrashMapMarkerCmd::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixCrashMapMarkerCmd::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixCrashMapMarkerCmd::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixCrashMapMarkerCmd::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixCrashMapMarkerCmd::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixCrashMapMarkerCmd::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Fix for crash when using the -MapMaker command line option. Nullptr camera passed to 
				// BSGraphics::State::SetCameraData.
				Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&sub);
				pointer_FixCrashMapMarkerCmd_sub = __CKPE_OFFSET(1);

				return true;
			}

			void FixCrashMapMarkerCmd::sub(std::int64_t a1, std::int64_t a2, std::int64_t a3) noexcept(true)
			{
				if (a2)
					fast_call<void>(pointer_FixCrashMapMarkerCmd_sub, a1, a2, a3);
			}
		}
	}
}