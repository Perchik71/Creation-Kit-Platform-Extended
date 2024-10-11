// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "RemoveUselessMessagesSF.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
		{
			RemoveUselessMessagesPatch::RemoveUselessMessagesPatch() : Module(GlobalEnginePtr)
			{}

			bool RemoveUselessMessagesPatch::HasOption() const
			{
				return false;
			}

			bool RemoveUselessMessagesPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* RemoveUselessMessagesPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* RemoveUselessMessagesPatch::GetName() const
			{
				return "Remove Useless Messages";
			}

			bool RemoveUselessMessagesPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> RemoveUselessMessagesPatch::GetDependencies() const
			{
				return {};
			}

			bool RemoveUselessMessagesPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return (eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_1_14_70_0) &&
					(eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_LAST);
			}

			bool RemoveUselessMessagesPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				auto verPatch = lpRelocationDatabaseItem->Version();

				if (verPatch == 1)
				{
					ScopeRelocator text;

					for (uint32_t i = 0; i < lpRelocationDatabaseItem->Count(); i++)
						lpRelocator->PatchNop(_RELDATA_RAV(i), 5);

					return true;
				}

				return false;
			}

			bool RemoveUselessMessagesPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}