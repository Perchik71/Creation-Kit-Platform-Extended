// Copyright © 2023-2025 aka CKPE team. All rights reserved.
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

			bool TESDataHandlerHook::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool TESDataHandlerHook::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool TESDataHandlerHook::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				static auto active_data = ID(223341).Address();

				EditorAPI::TESDataHandler::Singleton = ID(476691).Address();
				EditorAPI::TESDataHandler::UserModdedSingleton = (std::uintptr_t)&active_data;

				return true;
			}
		}
	}
}