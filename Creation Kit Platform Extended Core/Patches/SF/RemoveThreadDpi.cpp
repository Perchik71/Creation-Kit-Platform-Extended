// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "RemoveThreadDpi.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
		{
			RemoveThreadDpiPatch::RemoveThreadDpiPatch() : Module(GlobalEnginePtr)
			{}

			bool RemoveThreadDpiPatch::HasOption() const
			{
				return false;
			}

			bool RemoveThreadDpiPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* RemoveThreadDpiPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* RemoveThreadDpiPatch::GetName() const
			{
				return "Remove Thread Dpi";
			}

			bool RemoveThreadDpiPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> RemoveThreadDpiPatch::GetDependencies() const
			{
				return {};
			}

			bool RemoveThreadDpiPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_1_14_70_0;
			}

			bool RemoveThreadDpiPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					for (uint32_t i = 0; i < lpRelocationDatabaseItem->Count(); i++)
						lpRelocator->Patch(_RELDATA_RAV(i), { 0xEB });

					return true;
				}

				return false;
			}

			bool RemoveThreadDpiPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}