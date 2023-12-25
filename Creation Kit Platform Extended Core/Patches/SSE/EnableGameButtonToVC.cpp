// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "EnableGameButtonToVC.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			EnableGameButtonToVCPatch::EnableGameButtonToVCPatch() : Module(GlobalEnginePtr)
			{}

			bool EnableGameButtonToVCPatch::HasOption() const
			{
				return false;
			}

			bool EnableGameButtonToVCPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* EnableGameButtonToVCPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* EnableGameButtonToVCPatch::GetName() const
			{
				return "Enable Game Button To VC";
			}

			bool EnableGameButtonToVCPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> EnableGameButtonToVCPatch::GetDependencies() const
			{
				return {};
			}

			bool EnableGameButtonToVCPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool EnableGameButtonToVCPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 2);		// Enable push to game button even if version control is disabled
					lpRelocator->Patch(lpRelocationDatabaseItem->At(1), { 0xEB });	// ^

					return true;
				}

				return false;
			}

			bool EnableGameButtonToVCPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}