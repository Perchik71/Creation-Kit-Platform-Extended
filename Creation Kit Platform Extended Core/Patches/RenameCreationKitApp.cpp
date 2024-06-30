// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "RenameCreationKitApp.h"
#ifdef _CKPE_WITH_QT5
#include <QtCore/qstring.h>
#endif // !_CKPE_WITH_QT5

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
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

		bool RenameCreationKitAppPatch::HasDependencies() const
		{
			return false;
		}

		Array<String> RenameCreationKitAppPatch::GetDependencies() const
		{
			return {};
		}

		bool RenameCreationKitAppPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			const_cast<RenameCreationKitAppPatch*>(this)->_versionEditor = eEditorCurrentVersion;
			return true;
		}

		bool RenameCreationKitAppPatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			auto verPatch = lpRelocationDatabaseItem->Version();

			if ((verPatch == 1) || (verPatch == 2))
			{
				//
				// Change the default window class name so legacy editors can be opened without using bAllowMultipleEditors
				//
				char* newWindowClass = new char[250];
				sprintf_s(newWindowClass, 250, "Creation Kit %s", allowedEditorVersionStr[(int)_versionEditor].data());
				lpRelocator->PatchStringRef(_RELDATA_RAV(0), newWindowClass);

#ifdef _CKPE_WITH_QT5
				if (verPatch == 2)
				{
					// Initial QCoreApplication
					lpRelocator->PatchString(_RELDATA_RAV(5), "Creation Kit");
					// In Qt, it would be necessary to give the name of the window initially, for something acceptable
					lpRelocator->Patch(_RELDATA_RAV(1), (uint8_t*)"Creation Kit\0", 13);
					// Cut a useless entry [Branch: <some>, Version: <CKVer>]
					lpRelocator->PatchNop(_RELDATA_RAV(2), 0xA7);
					lpRelocator->PatchNop(_RELDATA_RAV(4), 0x6);
					// Cut a useless entry [Admin]
					lpRelocator->DetourCall(_RELDATA_RAV(3), &QString::sprintf);
				}
#endif // !_CKPE_WITH_QT5

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