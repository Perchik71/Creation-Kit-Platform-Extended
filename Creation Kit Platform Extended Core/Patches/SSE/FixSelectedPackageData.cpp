// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixSelectedPackageData.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixSelectedPackageDataPatch::FixSelectedPackageDataPatch() : Module(GlobalEnginePtr)
			{}

			bool FixSelectedPackageDataPatch::HasOption() const
			{
				return false;
			}

			bool FixSelectedPackageDataPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixSelectedPackageDataPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixSelectedPackageDataPatch::GetName() const
			{
				return "Fix Selected Package Data";
			}

			bool FixSelectedPackageDataPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixSelectedPackageDataPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for a package's "Selected Package Data" combo box not having a selected value when using a Topic type. 
					// Pointer<->Form ID truncation.
					//
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 7);

					return true;
				}

				return false;
			}

			bool FixSelectedPackageDataPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}