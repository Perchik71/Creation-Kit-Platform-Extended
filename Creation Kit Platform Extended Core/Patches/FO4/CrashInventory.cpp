// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "CrashInventory.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			CrashInventoryPatch::CrashInventoryPatch() : Module(GlobalEnginePtr)
			{}

			bool CrashInventoryPatch::HasOption() const
			{
				return false;
			}

			bool CrashInventoryPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* CrashInventoryPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* CrashInventoryPatch::GetName() const
			{
				return "Crash Inventory";
			}

			bool CrashInventoryPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> CrashInventoryPatch::GetDependencies() const
			{
				return {};
			}

			bool CrashInventoryPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool CrashInventoryPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// Some kind of hat is going on, idk know why, I'll just cut it
					lpRelocator->Patch(_RELDATA_RAV(0), { 0xE9, 0x9D, 0x00, 0x00, 0x00, 0x90 });

					return true;
				}
				else if (lpRelocationDatabaseItem->Version() == 2)
				{
					// Some kind of hat is going on, idk why, I'll just cut it
					lpRelocator->Patch(_RELDATA_RAV(0), { 0xE9, 0x9E, 0x00, 0x00, 0x00, 0x90 });

					return true;
				}

				return false;
			}

			bool CrashInventoryPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}