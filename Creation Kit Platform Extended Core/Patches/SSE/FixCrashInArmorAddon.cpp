// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixCrashInArmorAddon.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t addr1, addr2;

			FixCrashInArmorAddonPatch::FixCrashInArmorAddonPatch() : Module(GlobalEnginePtr)
			{}

			bool FixCrashInArmorAddonPatch::HasOption() const
			{
				return false;
			}

			bool FixCrashInArmorAddonPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixCrashInArmorAddonPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixCrashInArmorAddonPatch::GetName() const
			{
				return "Fix Crash In Armor Addon";
			}

			bool FixCrashInArmorAddonPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixCrashInArmorAddonPatch::GetDependencies() const
			{
				return {};
			}

			bool FixCrashInArmorAddonPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixCrashInArmorAddonPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for crash when selecting more than 64 races in the armor addon dialog.
					// A for loop reads past the end of a stack-allocated buffer.
					//
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 231);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(1), (uintptr_t)&sub_unknown);
					addr1 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(2));
					addr2 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(3));

					return true;
				}

				return false;
			}

			bool FixCrashInArmorAddonPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void FixCrashInArmorAddonPatch::sub_unknown(__int64 a1, HWND DialogWindow)
			{
				((void(__fastcall*)(__int64, HWND))addr1)(a1, DialogWindow);

				if (LRESULT count = SendDlgItemMessageA(DialogWindow, 2017, LB_GETSELCOUNT, 0, 0); count != LB_ERR)
				{
					Array<int> items(count);
					SendDlgItemMessageA(DialogWindow, 2017, LB_GETSELITEMS, count, (LPARAM)items.data());

					for (int item : items)
					{
						auto data = SendDlgItemMessageA(DialogWindow, 2017, LB_GETITEMDATA, item, 0);
						__int64 unknown = *(__int64*)(a1 + 0x38);

						if (data && data != unknown)
							((void(__fastcall*)(__int64, __int64*))addr2)(a1 + 0x1A0, &data);
					}
				}
			}
		}
	}
}