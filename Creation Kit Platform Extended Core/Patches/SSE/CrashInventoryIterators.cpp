// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "CrashInventoryIterators.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_CrashInventoryIterators_sub = 0;

			CrashInventoryIteratorsPatch::CrashInventoryIteratorsPatch() : Module(GlobalEnginePtr)
			{}

			bool CrashInventoryIteratorsPatch::HasOption() const
			{
				return false;
			}

			bool CrashInventoryIteratorsPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* CrashInventoryIteratorsPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* CrashInventoryIteratorsPatch::GetName() const
			{
				return "Crash Inventory Iterators";
			}

			bool CrashInventoryIteratorsPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashInventoryIteratorsPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for the "Class" edit dialog not filling in the "Training" checkbox.
					// Also hide the unused "Recharge" option.
					//
					Detours::X64::DetourClassVTable(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0)), 
						&sub, 86);

					return true;
				}

				return false;
			}

			bool CrashInventoryIteratorsPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void CrashInventoryIteratorsPatch::sub(TESForm* Class, HWND WindowHandle)
			{
				//auto cellRefList = CreateCellPersistentMapCopy(a1);
				//int status = 1;

				//// Unknown init function
				//for (TESObjectREFR_CK* refr : cellRefList)
				//{
				//	if (status != 1)
				//		break;

				//	// Automatically decrements ref count
				//	status = ((int(__fastcall*)(__int64, TESObjectREFR_CK**))OFFSET(0x1BC88B0, 1530))(*(__int64*)a2, &refr);
				//}

				//return status;
			}
		}
	}
}