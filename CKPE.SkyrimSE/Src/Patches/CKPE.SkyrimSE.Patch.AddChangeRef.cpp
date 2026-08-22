// Copyright © 2023-2025 aka CKPE team. All rights reserved.
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

#include <format>

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
				Common::Relocation(Common::ID(278484), 0x9A5).WriteCall(HKInsertMenuA);
				*(std::uintptr_t*)&EditorAPI::Forms::TESObjectREFR::SetParentWithRedraw = Common::ID(284410).Address();
				Common::Relocation(Common::ID(278484), 0xD9D).WriteCall(HKDeleteMenu);

				return true;
			}

			bool AddChangeRef::SupportsAddressLibrary() const noexcept(true)
			{
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