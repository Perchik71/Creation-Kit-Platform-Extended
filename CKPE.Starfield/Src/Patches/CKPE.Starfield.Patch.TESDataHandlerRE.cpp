// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Starfield.VersionLists.h>
#include <EditorAPI/TESDataHandler.h>
#include <Patches/CKPE.Starfield.Patch.TESDataHandlerRE.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			std::uintptr_t pointer_TESDataHandler_data2 = 0;

			TESDataHandlerRE::TESDataHandlerRE() : Common::Patch()
			{
				SetName("TESDataHandler");
			}

			bool TESDataHandlerRE::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* TESDataHandlerRE::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool TESDataHandlerRE::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> TESDataHandlerRE::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool TESDataHandlerRE::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_STARFIELD_LAST;
			}

			bool TESDataHandlerRE::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				EditorAPI::TESDataHandler::Singleton = __CKPE_OFFSET(0);
				pointer_TESDataHandler_data2 = __CKPE_OFFSET(1);
				EditorAPI::TESDataHandler::UserModdedSingleton = (std::uintptr_t)&pointer_TESDataHandler_data2;

				return true;
			}
		}
	}
}