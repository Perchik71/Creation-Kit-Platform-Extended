// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/Forms/TESObjectREFR.h>
#include <Patches/CKPE.Fallout4.Patch.AddChangeRef.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			static bool HKInsertMenuA(HMENU hMenu, std::uint32_t uPosition, std::uint32_t uFlags,
				std::uintptr_t uIDNewItem, const char* lpNewItem) noexcept(true)
			{
				InsertMenuA(hMenu, uPosition, uFlags, uIDNewItem, lpNewItem);
				return InsertMenuA(hMenu, uPosition, uFlags,
					Common::EditorUI::UI_EDITOR_CHANGEBASEFORM, Common::EditorUI::UI_EDITOR_CHANGEBASEFORM_STR);
			}

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
				return false;
			}

			std::vector<std::string> AddChangeRef::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool AddChangeRef::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool AddChangeRef::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto _interface = CKPE::Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&HKInsertMenuA);

				*(std::uintptr_t*)&EditorAPI::Forms::TESObjectREFR::SetParentWithRedraw = __CKPE_OFFSET(1);
				*(std::uintptr_t*)&EditorAPI::Forms::TESObjectREFR_Extremly_NG::SetParentWithRedraw = __CKPE_OFFSET(1);
				*(std::uintptr_t*)&EditorAPI::Forms::TESObjectREFR_Extremly::SetParentWithRedraw = __CKPE_OFFSET(1);

				return true;
			}
		}
	}
}