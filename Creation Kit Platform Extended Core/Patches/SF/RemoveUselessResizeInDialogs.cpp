// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "RemoveUselessResizeInDialogs.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
		{
			RemoveUselessResizeInDialogsPatch::RemoveUselessResizeInDialogsPatch() : Module(GlobalEnginePtr)
			{}

			bool RemoveUselessResizeInDialogsPatch::HasOption() const
			{
				return false;
			}

			bool RemoveUselessResizeInDialogsPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* RemoveUselessResizeInDialogsPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* RemoveUselessResizeInDialogsPatch::GetName() const
			{
				return "Remove Useless Resize In Dialogs";
			}

			bool RemoveUselessResizeInDialogsPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> RemoveUselessResizeInDialogsPatch::GetDependencies() const
			{
				return {};
			}

			bool RemoveUselessResizeInDialogsPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_1_14_70_0;
			}

			bool RemoveUselessResizeInDialogsPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					for (uint32_t i = 0; i < lpRelocationDatabaseItem->Count(); i++)
						lpRelocator->PatchNop(_RELDATA_RAV(i), 6);

					return true;
				}

				return false;
			}

			bool RemoveUselessResizeInDialogsPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}