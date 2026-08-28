// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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
			using TFixLoadArchiveActivePlugin_sub = std::uint32_t(const EditorAPI::TESFile*);

			static std::function<TFixLoadArchiveActivePlugin_sub> FixLoadArchiveActivePlugin_sub;

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

			bool FixLoadArchiveActivePlugin::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixLoadArchiveActivePlugin::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixLoadArchiveActivePlugin::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fixing the loading of archives of the active plugin
				// return filename active plugin without latest send to loaded
				//

				auto target = ID(277090);
				Relocation(target, Offset{ 0xA1B, 0xA59 }).WriteCall(&sub);
				Relocation(target, Offset{ 0xA34, 0xA72 }).WriteCall(&sub);
				
				if (VersionLists::GetEditorVersion() >= VersionLists::EDITOR_SKYRIM_SE_1_6_1378_1)
				{
					Relocation(target, 0xA99).WriteCall(&sub);
					Relocation(target, 0xAAA).WriteCall(&sub2);

					FixLoadArchiveActivePlugin_sub = Relocation<TFixLoadArchiveActivePlugin_sub>(ID(205782)).Get();
				}

				return true;
			}

			const char* FixLoadArchiveActivePlugin::sub() noexcept(true)
			{
				return EditorAPI::TESDataHandler::Singleton->ActiveMod->GetFileName().c_str();
			}

			std::uint32_t FixLoadArchiveActivePlugin::sub2() noexcept(true)
			{
				return FixLoadArchiveActivePlugin_sub(EditorAPI::TESDataHandler::Singleton->ActiveMod);
			}
		}
	}
}