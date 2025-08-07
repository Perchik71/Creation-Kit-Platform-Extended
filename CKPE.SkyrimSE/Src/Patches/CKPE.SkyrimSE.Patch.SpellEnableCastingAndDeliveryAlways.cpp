// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.Asserts.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.SpellEnableCastingAndDeliveryAlways.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			static bool HKEnableWindow(HWND hwndDlg, BOOL bEnable) noexcept(true)
			{
				return EnableWindow(hwndDlg, TRUE);
			}

			SpellEnableCastingAndDeliveryAlways::SpellEnableCastingAndDeliveryAlways() : Common::Patch()
			{
				SetName("Spell Enable Casting And Delivery Always");
			}

			bool SpellEnableCastingAndDeliveryAlways::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* SpellEnableCastingAndDeliveryAlways::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool SpellEnableCastingAndDeliveryAlways::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> SpellEnableCastingAndDeliveryAlways::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool SpellEnableCastingAndDeliveryAlways::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool SpellEnableCastingAndDeliveryAlways::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&HKEnableWindow);	// Spell fix disable Casting and Delivery
				Detours::DetourCall(__CKPE_OFFSET(1), (std::uintptr_t)&HKEnableWindow);	// ^

				return true;
			}
		}
	}
}