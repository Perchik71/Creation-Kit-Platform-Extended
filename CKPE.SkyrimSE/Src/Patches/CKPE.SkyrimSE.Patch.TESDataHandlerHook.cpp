// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/TESDataHandler.h>
#include <Patches/CKPE.SkyrimSE.Patch.TESDataHandlerHook.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			TESDataHandlerHook::TESDataHandlerHook() : Common::Patch()
			{
				SetName("TESDataHandler");
			}

			bool TESDataHandlerHook::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* TESDataHandlerHook::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool TESDataHandlerHook::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> TESDataHandlerHook::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool TESDataHandlerHook::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool TESDataHandlerHook::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();
				static auto active_data = __CKPE_OFFSET(1);

				EditorAPI::TESDataHandler::Singleton = __CKPE_OFFSET(0);
				EditorAPI::TESDataHandler::UserModdedSingleton = (std::uintptr_t)&active_data;

				return true;
			}
		}
	}
}