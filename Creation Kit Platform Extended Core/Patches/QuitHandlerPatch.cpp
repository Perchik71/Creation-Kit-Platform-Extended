// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "QuitHandlerPatch.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		QuitHandlerPatch::QuitHandlerPatch() : Module(GlobalEnginePtr)
		{}

		bool QuitHandlerPatch::HasOption() const
		{
			return false;
		}

		bool QuitHandlerPatch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* QuitHandlerPatch::GetOptionName() const
		{
			return nullptr;
		}

		const char* QuitHandlerPatch::GetName() const
		{
			return "Quit Handler";
		}

		bool QuitHandlerPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return true;
		}

		bool QuitHandlerPatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			if (lpRelocationDatabaseItem->Version() == 1)
			{
				for (uint32_t nId = 0; nId < lpRelocationDatabaseItem->Count(); nId++)
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(nId),
						(uintptr_t)&CreationKitPlatformExtended::Utils::Quit);

				return true;
			}
			
			return false;
		}

		bool QuitHandlerPatch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}
	}
}