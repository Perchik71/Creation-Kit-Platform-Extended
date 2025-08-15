// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
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

			bool FixParamsVTXT::DoQuery() const noexcept(true)
			{
				auto ver = VersionLists::GetEditorVersion();
				return (ver >= VersionLists::EDITOR_SKYRIM_SE_1_6_438) &&
					(ver <= VersionLists::EDITOR_SKYRIM_SE_LAST);
			}

			bool FixParamsVTXT::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				Detours::DetourJump(__CKPE_OFFSET(0), 
					(std::uintptr_t)&EditorAPI::Forms::TESObjectLAND::Layers::HKNormalize);

				return true;
			}
		}
	}
}