// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Utils.h>
#include <EditorAPI/IDs.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/T/TESObjectCELL.h>
#include <Patches/CKPE.Fallout4.Patch.PreVis.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			std::uintptr_t pointer_PreVisPatch_sub = 0;

			PreVis::PreVis() : Common::Patch()
			{
				SetName("PreVis");
			}

			bool PreVis::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* PreVis::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool PreVis::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> PreVis::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool PreVis::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool PreVis::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				// This function returns Form, for preprocessing visibility, however, 
				// sometimes this function returns a Form that is not a Cell. This is an error, 
				// because in the body it is further revealedand coordinates in the world space are obtained.

				using namespace Common;

				Relocation(EditorAPI::IDs::Functions::sub_48D8D0, Offset{ 0x2A1, 0x500 }).WriteCall(sub);

				return true;
			}

			void* PreVis::sub(std::uint32_t a_formid) noexcept(true)
			{
				auto form = EditorAPI::Forms::TESForm::FindFormByFormID(a_formid);
				return (!form || !form->Is<EditorAPI::Forms::TESObjectCELL>()) ? nullptr : form;
			}
		}
	}
}