// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.Relocation.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/TESDataHandler.h>
#include <Patches/CKPE.Fallout4.Patch.TESDataHandlerHook.h>

namespace CKPE
{
	namespace Fallout4
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
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool TESDataHandlerHook::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				static auto data = Relocation(ID{ 314871, 1538742 }).Address();
				static auto active_data = Relocation(ID(583335)).Address();

				EditorAPI::TESDataHandler::Singleton = reinterpret_cast<std::uintptr_t>(std::addressof(data));
				EditorAPI::TESDataHandler::UserModdedSingleton = reinterpret_cast<std::uintptr_t>(std::addressof(active_data));

				return true;
			}
		}
	}
}