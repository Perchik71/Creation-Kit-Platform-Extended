// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "RunNetworkDisable.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			RunNetworkDisablePatch::RunNetworkDisablePatch() : Module(GlobalEnginePtr)
			{}

			bool RunNetworkDisablePatch::HasOption() const
			{
				return false;
			}

			bool RunNetworkDisablePatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* RunNetworkDisablePatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* RunNetworkDisablePatch::GetName() const
			{
				return "Run Network Disable";
			}

			bool RunNetworkDisablePatch::HasDependencies() const
			{
				return false;
			}

			Array<String> RunNetworkDisablePatch::GetDependencies() const
			{
				return {};
			}

			bool RunNetworkDisablePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return (eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST) && 
					(eEditorCurrentVersion != EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_1_10_943_1);
			}

			bool RunNetworkDisablePatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// The ability to run CK without access to the Internet and/or a network device.
					lpRelocator->Patch(_RELDATA_RAV(0), { 0xEB });

					return true;
				}

				return false;
			}

			bool RunNetworkDisablePatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}