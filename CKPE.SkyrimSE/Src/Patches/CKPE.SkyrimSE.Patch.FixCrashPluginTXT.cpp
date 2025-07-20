// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixCrashPluginTXT.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixCrashPluginTXT::FixCrashPluginTXT() : Common::Patch()
			{
				SetName("Fix Crash PluginTXT");
			}

			bool FixCrashPluginTXT::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixCrashPluginTXT::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixCrashPluginTXT::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixCrashPluginTXT::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixCrashPluginTXT::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixCrashPluginTXT::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for crash when plugins.txt is present in the game root folder. 
				// Buffer overflow in ArchiveManager::OpenMasterArchives when appending to a string.
				// Skip the parsing code completely.
				//
				SafeWrite::WriteNop(__CKPE_OFFSET(0), 6);

				return true;
			}
		}
	}
}