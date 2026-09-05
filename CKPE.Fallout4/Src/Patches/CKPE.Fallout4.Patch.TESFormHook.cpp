// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.LogWindow.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/Forms/TESForm.h>
#include <Patches/CKPE.Fallout4.Patch.TESFormHook.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			TESFormHook::TESFormHook() : Common::Patch()
			{
				SetName("TESForm");
			}

			bool TESFormHook::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* TESFormHook::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool TESFormHook::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> TESFormHook::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool TESFormHook::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool TESFormHook::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				EditorAPI::Forms::TESForm::EnumFormIDs = (EditorAPI::Forms::TESForm::ENUM_FORM_ID*)Relocation(ID{ 1106870, 1618347 }).Get();
				EditorAPI::Forms::TESForm::FindFormByFormID = Relocation<decltype(EditorAPI::Forms::TESForm::FindFormByFormID)>(ID{ 611970, 1519646 }).Get();
				EditorAPI::Forms::TESForm::SetFormEditorIDImpl = Relocation<decltype(EditorAPI::Forms::TESForm::SetFormEditorIDImpl)>(ID{ 655843, 1618628 }).Get();

				return true;
			}
		}
	}
}