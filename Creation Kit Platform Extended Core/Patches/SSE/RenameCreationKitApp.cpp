// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "RenameCreationKitApp.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			char newWindowClass[250] = { 0 };

			RenameCreationKitAppPatch::RenameCreationKitAppPatch() : Module(GlobalEnginePtr)
			{}

			bool RenameCreationKitAppPatch::HasOption() const
			{
				return false;
			}

			bool RenameCreationKitAppPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* RenameCreationKitAppPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* RenameCreationKitAppPatch::GetName() const
			{
				return "Rename Creation Kit App";
			}

			bool RenameCreationKitAppPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				const_cast<RenameCreationKitAppPatch*>(this)->_versionEditor = eEditorCurrentVersion;
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool RenameCreationKitAppPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Change the default window class name so legacy editors can be opened without using bAllowMultipleEditors
					//
					char* newWindowClass = new char[250];
					sprintf_s(newWindowClass, 250, "Creation Kit %s", allowedEditorVersionStr[(int)_versionEditor].data());
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), (uint8_t*)&newWindowClass, sizeof(newWindowClass));

					return true;
				}

				return false;
			}

			bool RenameCreationKitAppPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}