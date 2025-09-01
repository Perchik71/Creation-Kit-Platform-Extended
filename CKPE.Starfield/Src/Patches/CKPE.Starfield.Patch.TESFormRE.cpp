// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Starfield.VersionLists.h>
#include <EditorAPI/Forms/TESForm.h>
#include <Patches/CKPE.Starfield.Patch.TESFormRE.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			TESFormRE::TESFormRE() : Common::Patch()
			{
				SetName("TESForm");
			}

			bool TESFormRE::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* TESFormRE::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool TESFormRE::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> TESFormRE::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool TESFormRE::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_STARFIELD_LAST;
			}

			bool TESFormRE::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				*(std::uintptr_t*)&EditorAPI::Forms::TESForm::FindFormByFormID = __CKPE_OFFSET(0);
				*(std::uintptr_t*)&EditorAPI::Forms::TESForm::FindFormByEditorID = __CKPE_OFFSET(1);

				return true;
			}
		}
	}
}