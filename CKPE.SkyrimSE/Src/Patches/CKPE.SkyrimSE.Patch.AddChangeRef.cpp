// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/Forms/TESObjectREFR.h>
#include <Patches/CKPE.SkyrimSE.Patch.AddChangeRef.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			AddChangeRef::AddChangeRef() : Common::Patch()
			{
				SetName("Add Change Ref");
			}

			bool AddChangeRef::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* AddChangeRef::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool AddChangeRef::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> AddChangeRef::GetDependencies() const noexcept(true)
			{
				return { "Replace BSPointerHandle And Manager" };
			}

			bool AddChangeRef::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() >= VersionLists::EDITOR_SKYRIM_SE_1_6_438;
			}

			bool AddChangeRef::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&HKInsertMenuA);

				*(std::uintptr_t*)&EditorAPI::Forms::TESObjectREFR::SetParentWithRedraw =
					__CKPE_OFFSET(1);

				Detours::DetourCall(__CKPE_OFFSET(2), (std::uintptr_t)&HKDeleteMenu);

				return true;
			}

			bool AddChangeRef::HKInsertMenuA(void* hMenu, std::uint32_t uPosition, std::uint32_t uFlags,
				std::uintptr_t uIDNewItem, const char* lpNewItem)
			{
				InsertMenuA((HMENU)hMenu, uPosition, uFlags, uIDNewItem, lpNewItem);
				return InsertMenuA((HMENU)hMenu, uPosition, uFlags,
					Common::EditorUI::UI_EDITOR_CHANGEBASEFORM, Common::EditorUI::UI_EDITOR_CHANGEBASEFORM_STR);
			}

			bool AddChangeRef::HKDeleteMenu(void* hMenu, std::uint32_t uPosition, std::uint32_t uFlags)
			{
				DeleteMenu((HMENU)hMenu, uPosition, uFlags);
				return DeleteMenu((HMENU)hMenu, Common::EditorUI::UI_EDITOR_CHANGEBASEFORM, MF_BYCOMMAND);
			}
		}
	}
}