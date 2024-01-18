// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "SkipUpdateCheck.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			SkipUpdateCheckPatch::SkipUpdateCheckPatch() : Module(GlobalEnginePtr)
			{}

			bool SkipUpdateCheckPatch::HasOption() const
			{
				return false;
			}

			bool SkipUpdateCheckPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* SkipUpdateCheckPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* SkipUpdateCheckPatch::GetName() const
			{
				return "Skipping the program update check";
			}

			bool SkipUpdateCheckPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> SkipUpdateCheckPatch::GetDependencies() const
			{
				return {};
			}

			bool SkipUpdateCheckPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool SkipUpdateCheckPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// Skipping the program update check
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0xEB, 0x10 });

					return true;
				}

				return false;
			}

			bool SkipUpdateCheckPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}