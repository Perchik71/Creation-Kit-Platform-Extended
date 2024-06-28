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

		bool AllowMultipleWindowAndMasterPatch::HasDependencies() const
		{
			return false;
		}

		Array<String> AllowMultipleWindowAndMasterPatch::GetDependencies() const
		{
			return {};
		}

		bool AllowMultipleWindowAndMasterPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return true;
		}

		bool AllowMultipleWindowAndMasterPatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			auto verPatch = lpRelocationDatabaseItem->Version();

			ScopeRelocator text;

			switch (verPatch)
			{
			case 1:
				lpRelocator->Patch(_RELDATA_RAV(0), { 0xE9, 0xBA, 0x00, 0x00, 0x00, 0x90 });
				break;
			case 2:
				lpRelocator->Patch(_RELDATA_RAV(0), { 0xE9, 0xDE, 0x00, 0x00, 0x00, 0x90 });
				break;
			case 3:
				lpRelocator->Patch(_RELDATA_RAV(0), { 0xE9, 0xD4, 0x00, 0x00, 0x00, 0x90 });
				break;
			case 4:
				lpRelocator->Patch(_RELDATA_RAV(0), { 0xE9, 0x5B, 0x01, 0x00, 0x00, 0x90 });
				break;
			default:
				return false;
			}

			lpRelocator->Patch(_RELDATA_RAV(1), { 0xEB });

			return true;
		}

		bool AllowMultipleWindowAndMasterPatch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}
	}
}