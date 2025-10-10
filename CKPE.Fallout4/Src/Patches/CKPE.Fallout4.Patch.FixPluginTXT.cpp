// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.FixPluginTXT.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			FixPluginTXT::FixPluginTXT() : Common::Patch()
			{
				SetName("Fix for crash when plugins.txt is present");
			}

			bool FixPluginTXT::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixPluginTXT::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixPluginTXT::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixPluginTXT::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixPluginTXT::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixPluginTXT::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Fix for crash when plugins.txt is present in the game root folder. 
				// Buffer overflow in ArchiveManager::OpenMasterArchives when appending to a string. 
				// Skip the parsing code completely.
				SafeWrite::WriteNop(__CKPE_OFFSET(0), 6);

				return true;
			}
		}
	}
}