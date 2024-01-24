// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixEncounterZone.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			uintptr_t pointer_FixEncounterZone_sub = 0;

			FixEncounterZonePatch::FixEncounterZonePatch() : Module(GlobalEnginePtr)
			{}

			bool FixEncounterZonePatch::HasOption() const
			{
				return false;
			}

			bool FixEncounterZonePatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixEncounterZonePatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixEncounterZonePatch::GetName() const
			{
				return "Fix Encounter Zone";
			}

			bool FixEncounterZonePatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixEncounterZonePatch::GetDependencies() const
			{
				return {};
			}

			bool FixEncounterZonePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixEncounterZonePatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// Fix Encounter Zone
					// TBM_GETPOS = wParam must be zero.
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0x00 });
					// Fix Encounter Zone
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(1), (uintptr_t)&sub);

					pointer_FixEncounterZone_sub = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(2));

					return true;
				}

				return false;
			}

			bool FixEncounterZonePatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void FixEncounterZonePatch::sub(void* Class, HWND Dialog, uint8_t Level)
			{
				if (!Class)
				{
					MessageBoxA(0,
						"You are making mistakes!\n"
						"\nSet the ID and location and click Ok, only after that you can open and click on this button.",
						"Warning", MB_OK | MB_ICONWARNING);
					return;
				}

				fastCall<void>(pointer_FixEncounterZone_sub, Class, Dialog, Level);
			}
		}
	}
}