// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/EditorUI.h"
#include "ComPatch.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		using namespace CreationKitPlatformExtended::EditorAPI;

		ComPatch::ComPatch() : Module(GlobalEnginePtr)
		{}

		bool ComPatch::HasOption() const
		{
			return false;
		}

		bool ComPatch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* ComPatch::GetOptionName() const
		{
			return nullptr;
		}

		const char* ComPatch::GetName() const
		{
			return "COM";
		}

		bool ComPatch::HasDependencies() const
		{
			return false;
		}

		Array<String> ComPatch::GetDependencies() const
		{
			return {};
		}

		bool ComPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return true;
		}

		bool ComPatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			if (lpRelocationDatabaseItem->Version() == 1)
			{
				PatchIAT(HKCoInitializeEx, "COMBASE.DLL", "CoInitializeEx");
				PatchIAT(HKCoInitialize, "OLE32.DLL", "CoInitialize");

				return true;
			}
			
			return false;
		}

		bool ComPatch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}

		HRESULT ComPatch::HKCoInitialize(LPVOID pvReserved)
		{
			return CoInitializeEx(pvReserved, COINITBASE_MULTITHREADED);
		}

		HRESULT ComPatch::HKCoInitializeEx(LPVOID pvReserved, DWORD dwCoInit)
		{
			return CoInitializeEx(pvReserved, COINITBASE_MULTITHREADED);
		}
	}
}