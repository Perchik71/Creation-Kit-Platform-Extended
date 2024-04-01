// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "TESFormRE.h"
#include "Editor API/FO4/TESFormF4.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			extern uintptr_t pointer_TESForm_sub;
			extern uintptr_t pointer_TESForm_data;
		}
	}

	namespace Patches
	{
		namespace Fallout4
		{
			using namespace EditorAPI::Fallout4;

			TESFormREPatch::TESFormREPatch() : Module(GlobalEnginePtr)
			{}

			bool TESFormREPatch::HasOption() const
			{
				return false;
			}

			bool TESFormREPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* TESFormREPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* TESFormREPatch::GetName() const
			{
				return "TESForm";
			}

			bool TESFormREPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> TESFormREPatch::GetDependencies() const
			{
				return {};
			}

			bool TESFormREPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool TESFormREPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					pointer_TESForm_sub = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));
					pointer_TESForm_data = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0));

					return true;
				}

				return false;
			}

			bool TESFormREPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}