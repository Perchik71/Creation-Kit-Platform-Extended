// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
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
			using TFixCrashInArmorAddonSub1 = void(std::int64_t, std::int64_t);
			using TFixCrashInArmorAddonSub2 = void(std::int64_t, std::int64_t*);
		
			static std::function<TFixCrashInArmorAddonSub1> FixCrashInArmorAddonSub1;
			static std::function<TFixCrashInArmorAddonSub2> FixCrashInArmorAddonSub2;

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

			bool FixCrashInArmorAddon::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixCrashInArmorAddon::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixCrashInArmorAddon::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for crash when selecting more than 64 races in the armor addon dialog.
				// A for loop reads past the end of a stack-allocated buffer.
				//
				
				Relocation(ID(338178), 0x288).WriteFill(NOP, 231);
				Relocation(ID(498780)).WriteJump(&sub);

				FixCrashInArmorAddonSub1 = Relocation<TFixCrashInArmorAddonSub1>(ID(338178)).Get();
				FixCrashInArmorAddonSub2 = Relocation<TFixCrashInArmorAddonSub2>(ID(279783)).Get();

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