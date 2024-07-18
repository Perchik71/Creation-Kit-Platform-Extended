// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "RunAppWithoutNet.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
		{
			RunAppWithoutNetPatch::RunAppWithoutNetPatch() : Module(GlobalEnginePtr)
			{}

			bool RunAppWithoutNetPatch::HasOption() const
			{
				return false;
			}

			bool RunAppWithoutNetPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* RunAppWithoutNetPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* RunAppWithoutNetPatch::GetName() const
			{
				return "Run app without net";
			}

			bool RunAppWithoutNetPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> RunAppWithoutNetPatch::GetDependencies() const
			{
				return {};
			}

			bool RunAppWithoutNetPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return true;
			}

			bool RunAppWithoutNetPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				auto verPatch = lpRelocationDatabaseItem->Version();

				if (verPatch == 1)
				{
					lpRelocator->Patch(_RELDATA_RAV(0), { 0x90, 0xE9 });

					return true;
				}

				return false;
			}

			bool RunAppWithoutNetPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}