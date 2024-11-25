// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/EditorUI.h"
#include "Editor API/SSE/TESObjectREFR.h"
#include "AddChangeRef.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
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
				return true;
			}

			Array<String> AddChangeRefPatch::GetDependencies() const
			{
				return { "Replace BSPointerHandle And Manager" };
			}

			bool AddChangeRefPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return (eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_1_6_438) &&
					(eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST);
			}

			bool AddChangeRefPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&HKInsertMenuA);

					*(uintptr_t*)&EditorAPI::SkyrimSpectialEdition::TESObjectREFR::SetParentWithRedraw =
						lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(2), (uintptr_t)&HKDeleteMenu);

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

			bool AddChangeRefPatch::HKDeleteMenu(HMENU hMenu, UINT uPosition, UINT uFlags)
			{
				DeleteMenu(hMenu, uPosition, uFlags);
				return DeleteMenu(hMenu, EditorAPI::EditorUI::UI_EDITOR_CHANGEBASEFORM, MF_BYCOMMAND);
			}
		}
	}
}