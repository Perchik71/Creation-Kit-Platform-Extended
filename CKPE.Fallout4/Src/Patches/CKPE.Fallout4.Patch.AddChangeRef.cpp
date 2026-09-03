// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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
				using namespace Common;

				Relocation(ID{ 470821, 1939247 }, Offset{ 0x5A2, 0x9C8 }).WriteCall(&HKInsertMenuA);
				
				auto rel = ID(458423).Address();
				*(std::uintptr_t*)&EditorAPI::Forms::TESObjectREFR::SetParentWithRedraw = rel;
				*(std::uintptr_t*)&EditorAPI::Forms::TESObjectREFR_Extremly_NG::SetParentWithRedraw = rel;
				*(std::uintptr_t*)&EditorAPI::Forms::TESObjectREFR_Extremly::SetParentWithRedraw = rel;

				return true;
			}
		}
	}
}