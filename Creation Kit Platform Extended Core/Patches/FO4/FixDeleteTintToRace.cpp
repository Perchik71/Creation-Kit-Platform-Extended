// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixDeleteTintToRace.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			FixDeleteTintToRacePatch::FixDeleteTintToRacePatch() : Module(GlobalEnginePtr)
			{}

			bool FixDeleteTintToRacePatch::HasOption() const
			{
				return false;
			}

			bool FixDeleteTintToRacePatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixDeleteTintToRacePatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixDeleteTintToRacePatch::GetName() const
			{
				return "Fixed delete group tinting to race";
			}

			bool FixDeleteTintToRacePatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixDeleteTintToRacePatch::GetDependencies() const
			{
				return {};
			}

			bool FixDeleteTintToRacePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixDeleteTintToRacePatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// Fixed when you delete a group tinting to race window
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0x4D, 0x8B, 0x47, 0x8, 0x4C, 0x89, 0xE2 });
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(1), (uintptr_t)&sub);
					lpRelocator->Patch(lpRelocationDatabaseItem->At(2), { 0xEB, 0x18 });

					return true;
				}

				return false;
			}

			bool FixDeleteTintToRacePatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void FixDeleteTintToRacePatch::sub(int64_t count, int64_t item_id, HWND listview)
			{
				if (!count) return;
				for (auto i = 0; i < count; i++)
					ListView_DeleteItem(listview, item_id);
			}
		}
	}
}