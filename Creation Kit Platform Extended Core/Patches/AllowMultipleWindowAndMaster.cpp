// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "AllowMultipleWindowAndMaster.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		AllowMultipleWindowAndMasterPatch::AllowMultipleWindowAndMasterPatch() : Module(GlobalEnginePtr)
		{}

		bool AllowMultipleWindowAndMasterPatch::HasOption() const
		{
			return false;
		}

		bool AllowMultipleWindowAndMasterPatch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* AllowMultipleWindowAndMasterPatch::GetOptionName() const
		{
			return nullptr;
		}

		const char* AllowMultipleWindowAndMasterPatch::GetName() const
		{
			return "Allow Multiple Window And Master";
		}

		bool AllowMultipleWindowAndMasterPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
		}

		bool AllowMultipleWindowAndMasterPatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			if (lpRelocationDatabaseItem->Version() == 1)
			{
				lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0xE9, 0xBA, 0x00, 0x00, 0x00, 0x90 });
				lpRelocator->Patch(lpRelocationDatabaseItem->At(1), { 0xEB });

				return true;
			}

			return false;
		}

		bool AllowMultipleWindowAndMasterPatch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}
	}
}