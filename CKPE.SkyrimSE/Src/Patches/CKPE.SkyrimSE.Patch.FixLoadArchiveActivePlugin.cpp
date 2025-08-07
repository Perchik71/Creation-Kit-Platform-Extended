// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/TESDataHandler.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixLoadArchiveActivePlugin.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			std::uintptr_t pointer_FixLoadArchiveActivePlugin_sub = 0;

			FixLoadArchiveActivePlugin::FixLoadArchiveActivePlugin() : Common::Patch()
			{
				SetName("Fix Load Archive Active Plugin");
			}

			bool FixLoadArchiveActivePlugin::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixLoadArchiveActivePlugin::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixLoadArchiveActivePlugin::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> FixLoadArchiveActivePlugin::GetDependencies() const noexcept(true)
			{
				return { "TESDataHandler" };
			}

			bool FixLoadArchiveActivePlugin::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixLoadArchiveActivePlugin::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();
				if ((verPatch != 1) && (verPatch != 2))
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fixing the loading of archives of the active plugin
				// return filename active plugin without latest send to loaded
				//

				if (verPatch == 1)
				{
					Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&sub);
					Detours::DetourCall(__CKPE_OFFSET(1), (std::uintptr_t)&sub);
				}
				else if (verPatch == 2)
				{
					Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&sub);
					Detours::DetourCall(__CKPE_OFFSET(1), (std::uintptr_t)&sub);
					Detours::DetourCall(__CKPE_OFFSET(2), (std::uintptr_t)&sub);
					Detours::DetourCall(__CKPE_OFFSET(3), (std::uintptr_t)&sub2);

					pointer_FixLoadArchiveActivePlugin_sub = __CKPE_OFFSET(4);
				}

				return true;
			}

			const char* FixLoadArchiveActivePlugin::sub() noexcept(true)
			{
				return EditorAPI::TESDataHandler::Singleton->ActiveMod->GetFileName().c_str();
			}

			std::uint32_t FixLoadArchiveActivePlugin::sub2() noexcept(true)
			{
				return ((std::uint32_t(__fastcall*)(const EditorAPI::TESFile*))pointer_FixLoadArchiveActivePlugin_sub)
					(EditorAPI::TESDataHandler::Singleton->ActiveMod);
			}
		}
	}
}