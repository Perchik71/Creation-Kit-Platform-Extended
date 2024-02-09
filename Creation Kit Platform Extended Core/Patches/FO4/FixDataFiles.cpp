// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixDataFiles.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			FixDataFilesPatch::FixDataFilesPatch() : Module(GlobalEnginePtr)
			{}

			bool FixDataFilesPatch::HasOption() const
			{
				return false;
			}

			bool FixDataFilesPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixDataFilesPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixDataFilesPatch::GetName() const
			{
				return "Fix DataFiles";
			}

			bool FixDataFilesPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixDataFilesPatch::GetDependencies() const
			{
				return {};
			}

			bool FixDataFilesPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixDataFilesPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0xEB });	// skip force 
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(1), 2);		// force load plugin
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(2), 2);		// force delete DataFiles.txt

					return true;
				}

				return false;
			}

			bool FixDataFilesPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}