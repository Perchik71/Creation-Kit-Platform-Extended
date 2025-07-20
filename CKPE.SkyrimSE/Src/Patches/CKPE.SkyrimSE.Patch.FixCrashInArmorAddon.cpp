// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixCrashInArmorAddon.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			typedef void(*TFixCrashInArmorAddonSub1)(std::int64_t, std::int64_t);
			typedef void(*TFixCrashInArmorAddonSub2)(std::int64_t, std::int64_t*);
		
			static TFixCrashInArmorAddonSub1 FixCrashInArmorAddonSub1;
			static TFixCrashInArmorAddonSub2 FixCrashInArmorAddonSub2;

			FixCrashInArmorAddon::FixCrashInArmorAddon() : Common::Patch()
			{
				SetName("Fix Crash In Armor Addon");
			}

			bool FixCrashInArmorAddon::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixCrashInArmorAddon::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixCrashInArmorAddon::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixCrashInArmorAddon::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixCrashInArmorAddon::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixCrashInArmorAddon::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for crash when selecting more than 64 races in the armor addon dialog.
				// A for loop reads past the end of a stack-allocated buffer.
				//
				SafeWrite::WriteNop(__CKPE_OFFSET(0), 231);
				Detours::DetourJump(__CKPE_OFFSET(1), (std::uintptr_t)&sub);
				FixCrashInArmorAddonSub1 = (TFixCrashInArmorAddonSub1)__CKPE_OFFSET(2);
				FixCrashInArmorAddonSub2 = (TFixCrashInArmorAddonSub2)__CKPE_OFFSET(3);

				return true;
			}

			void FixCrashInArmorAddon::sub(std::int64_t a1, std::int64_t DialogWindow) noexcept(true)
			{
				FixCrashInArmorAddonSub1(a1, DialogWindow);

				if (LRESULT count = SendDlgItemMessageA((HWND)DialogWindow, 2017, LB_GETSELCOUNT, 0, 0); count != LB_ERR)
				{
					std::vector<std::int32_t> items(count);
					SendDlgItemMessageA((HWND)DialogWindow, 2017, LB_GETSELITEMS, count, (LPARAM)items.data());

					for (std::int32_t item : items)
					{
						auto data = SendDlgItemMessageA((HWND)DialogWindow, 2017, LB_GETITEMDATA, item, 0);
						auto unknown = *(std::int64_t*)(a1 + 0x38);

						if (data && data != unknown)
							FixCrashInArmorAddonSub2(a1 + 0x1A0, &data);
					}
				}
			}
		}
	}
}