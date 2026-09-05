// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/Forms/TESObjectCELL.h>
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

				Relocation(ID{ 632497, 1994724 }, Offset{ 0x2A1, 0x500 }).WriteCall(sub);
				pointer_PreVisPatch_sub = Relocation(ID{ 611970, 1519646 }).Address();

				return true;
			}

			void* PreVis::sub(void* a1) noexcept(true)
			{
				EditorAPI::Forms::TESForm* Ret = fast_call<EditorAPI::Forms::TESForm*>
					(pointer_PreVisPatch_sub, a1);

				if (Ret && (Ret->Type != EditorAPI::Forms::TESObjectCELL::TYPE_ID))
					return nullptr;

				return Ret;
			}
		}
	}
}