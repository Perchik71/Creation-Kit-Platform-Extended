// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <EditorAPI/IDs.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.LogWindow.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/T/TESForm.h>
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

				EditorAPI::Forms::TESForm::EnumFormIDs = (EditorAPI::Forms::TESForm::ENUM_FORM_ID*)Relocation(EditorAPI::IDs::Functions::data_3BAF440).Get();
				EditorAPI::Forms::TESForm::FindFormByFormID = Relocation<decltype(EditorAPI::Forms::TESForm::FindFormByFormID)>(EditorAPI::IDs::Functions::TESForm__GetFormByNumericID).Get();
				EditorAPI::Forms::TESForm::SetFormEditorIDImpl = Relocation<decltype(EditorAPI::Forms::TESForm::SetFormEditorIDImpl)>(EditorAPI::IDs::Functions::sub_8C5050).Get();

				return true;
			}
		}
	}
}