// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/SSE/TESForm.h"
#include "TESFormSSE_RE.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			using namespace EditorAPI::SkyrimSpectialEdition;

			TESFormSSE_REPatch::TESFormSSE_REPatch() : Module(GlobalEnginePtr)
			{}

			bool TESFormSSE_REPatch::HasOption() const
			{
				return false;
			}

			bool TESFormSSE_REPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* TESFormSSE_REPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* TESFormSSE_REPatch::GetName() const
			{
				return "TESForm";
			}

			bool TESFormSSE_REPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> TESFormSSE_REPatch::GetDependencies() const
			{
				return {};
			}

			bool TESFormSSE_REPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_1_6_1130;
			}

			bool TESFormSSE_REPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					*(uintptr_t*)&TESForm::GetFormTypeStr = _RELDATA_ADDR(0);

					return true;
				}

				return false;
			}

			bool TESFormSSE_REPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}