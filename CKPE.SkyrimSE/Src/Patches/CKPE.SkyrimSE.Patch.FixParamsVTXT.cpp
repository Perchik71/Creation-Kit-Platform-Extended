// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/Forms/TESObjectLAND.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixParamsVTXT.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixParamsVTXT::FixParamsVTXT() : Common::Patch()
			{
				SetName("Fix VTXT params sections");
			}

			bool FixParamsVTXT::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixParamsVTXT::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixParamsVTXT::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixParamsVTXT::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixParamsVTXT::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixParamsVTXT::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() >= VersionLists::EDITOR_SKYRIM_SE_1_6_438;
			}

			bool FixParamsVTXT::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				Relocation(ID(1027934)).WriteJump(&EditorAPI::Forms::TESObjectLAND::Layers::HKNormalize);

				return true;
			}
		}
	}
}