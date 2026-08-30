// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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

			bool FixCrashPluginTXT::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixCrashPluginTXT::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixCrashPluginTXT::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for crash when plugins.txt is present in the game root folder. 
				// Buffer overflow in ArchiveManager::OpenMasterArchives when appending to a string.
				// Skip the parsing code completely.
				//
				Relocation(ID(319725), Offset{ 0x16A, 0x186 }).WriteFill(NOP, 6);

				return true;
			}
		}
	}
}