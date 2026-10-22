// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/T/TES.h>
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
				using namespace Common;

				EditorAPI::TES::Singleton = Relocation<const EditorAPI::TES**>(EditorAPI::IDs::TES::Singleton).Get();

				static const EditorAPI::Setting* sSetting_SkyView = Relocation<const EditorAPI::Setting*>(EditorAPI::IDs::Sky::SkyViewSetting).Get();
				static const EditorAPI::Setting* sSetting_FogEnabled = Relocation<const EditorAPI::Setting*>(EditorAPI::IDs::Sky::FogEnabledSetting).Get();
				static const EditorAPI::Setting* sSetting_ShowMarkers = Relocation<const EditorAPI::Setting*>(EditorAPI::IDs::TES::ShowMarkersSetting).Get();

				EditorAPI::Sky::Setting_SkyView = std::addressof(sSetting_SkyView);
				EditorAPI::Sky::Setting_FogEnabled = std::addressof(sSetting_FogEnabled);
				EditorAPI::TES::Setting_ShowMarkers = std::addressof(sSetting_ShowMarkers);

				return true;
			}
		}
	}
}