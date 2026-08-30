// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
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
			static bool HKEnableWindow(HWND hwndDlg, [[maybe_unused]] BOOL bEnable) noexcept(true)
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

			bool SpellEnableCastingAndDeliveryAlways::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool SpellEnableCastingAndDeliveryAlways::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool SpellEnableCastingAndDeliveryAlways::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				auto target = ID(233123);
				Relocation(target, 0xD0).WriteCall(&HKEnableWindow);	// Spell fix disable Casting and Delivery
				Relocation(target, 0xF8).WriteCall(&HKEnableWindow);	// ^

				return true;
			}
		}
	}
}