// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/TES.h>
#include <Patches/CKPE.Fallout4.Patch.TESHook.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			TESHook::TESHook() : Common::Patch()
			{
				SetName("TES");
			}

			bool TESHook::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* TESHook::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool TESHook::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> TESHook::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool TESHook::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() != VersionLists::EDITOR_FALLOUT_C4_1_10_943_1;
			}

			bool TESHook::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto _interface = CKPE::Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				EditorAPI::TES::Singleton = (const EditorAPI::TES**)__CKPE_OFFSET(0);

				static const EditorAPI::Setting* sSetting_SkyView = (const EditorAPI::Setting*)__CKPE_OFFSET(1);
				static const EditorAPI::Setting* sSetting_FogEnabled = (const EditorAPI::Setting*)__CKPE_OFFSET(2);
				static const EditorAPI::Setting* sSetting_ShowMarkers = (const EditorAPI::Setting*)__CKPE_OFFSET(3);

				EditorAPI::Sky::Setting_SkyView = &sSetting_SkyView;
				EditorAPI::Sky::Setting_FogEnabled = &sSetting_FogEnabled;
				EditorAPI::TES::Setting_ShowMarkers = &sSetting_ShowMarkers;

				return true;
			}
		}
	}
}