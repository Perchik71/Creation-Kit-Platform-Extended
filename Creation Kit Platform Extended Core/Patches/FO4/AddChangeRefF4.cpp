// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/EditorUI.h"
#include "Editor API/FO4/TESObjectREFR.h"
#include "AddChangeRefF4.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			AddChangeRefPatch::AddChangeRefPatch() : Module(GlobalEnginePtr)
			{}

			bool AddChangeRefPatch::HasOption() const
			{
				return false;
			}

			bool AddChangeRefPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* AddChangeRefPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* AddChangeRefPatch::GetName() const
			{
				return "Add Change Ref";
			}

			bool AddChangeRefPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> AddChangeRefPatch::GetDependencies() const
			{
				return {};
			}

			bool AddChangeRefPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool AddChangeRefPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&HKInsertMenuA);

					*(uintptr_t*)&EditorAPI::Fallout4::TESObjectREFR::SetParentWithRedraw = _RELDATA_ADDR(1);
					*(uintptr_t*)&EditorAPI::Fallout4::TESObjectREFR_Extremly_NG::SetParentWithRedraw = _RELDATA_ADDR(1);
					*(uintptr_t*)&EditorAPI::Fallout4::TESObjectREFR_Extremly::SetParentWithRedraw = _RELDATA_ADDR(1);

					return true;
				}

				return false;
			}

			bool AddChangeRefPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			bool AddChangeRefPatch::HKInsertMenuA(HMENU hMenu, uint32_t uPosition, uint32_t uFlags,
				uintptr_t uIDNewItem, const char* lpNewItem)
			{
				InsertMenuA(hMenu, uPosition, uFlags, uIDNewItem, lpNewItem);
				return InsertMenuA(hMenu, uPosition, uFlags, 
					EditorAPI::EditorUI::UI_EDITOR_CHANGEBASEFORM, EditorAPI::EditorUI::UI_EDITOR_CHANGEBASEFORM_STR);
			}
		}
	}
}