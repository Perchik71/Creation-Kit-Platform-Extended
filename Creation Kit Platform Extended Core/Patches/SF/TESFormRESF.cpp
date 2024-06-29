// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "TESFormRESF.h"
#include "Editor API/SF/TESForm.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
		{
			using namespace EditorAPI::Starfield;

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
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_LAST;
			}

			bool TESFormREPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					*(uintptr_t*)&TESForm::GetFormByNumericID = _RELDATA_ADDR(0);
					*(uintptr_t*)&TESForm::GetFormByEditorID = _RELDATA_ADDR(1);

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