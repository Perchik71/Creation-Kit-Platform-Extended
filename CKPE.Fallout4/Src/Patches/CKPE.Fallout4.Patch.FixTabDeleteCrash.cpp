// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.FixTabDeleteCrash.h>

#include <commctrl.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			FixTabDeleteCrash::FixTabDeleteCrash() : Common::Patch()
			{
				SetName("Fix uninitialized TCITEMA structure variables");
			}

			bool FixTabDeleteCrash::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixTabDeleteCrash::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixTabDeleteCrash::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixTabDeleteCrash::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixTabDeleteCrash::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixTabDeleteCrash::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Fix for crash when tab control buttons are deleted. Uninitialized TCITEMA structure variables.
				Detours::DetourJump(__CKPE_OFFSET(0), (std::uintptr_t)&sub);

				return true;
			}

			void FixTabDeleteCrash::sub(void* TabControlHandle, std::uint32_t TabIndex) noexcept(true)
			{
				TCITEMA itemInfo = {};

				if (TabCtrl_GetItem((HWND)TabControlHandle, TabIndex, &itemInfo))
					TabCtrl_DeleteItem((HWND)TabControlHandle, TabIndex);
			}
		}
	}
}