// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
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
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				EditorAPI::Forms::TESForm::EnumFormIDs =
					(EditorAPI::Forms::TESForm::ENUM_FORM_ID*)(__CKPE_OFFSET(0));
				EditorAPI::Forms::TESForm::FindFormByFormID = 
					decltype(EditorAPI::Forms::TESForm::FindFormByFormID)(__CKPE_OFFSET(1));
				EditorAPI::Forms::TESForm::SetFormEditorIDImpl =
					decltype(EditorAPI::Forms::TESForm::SetFormEditorIDImpl)(__CKPE_OFFSET(3));

				return true;
			}
		}
	}
}