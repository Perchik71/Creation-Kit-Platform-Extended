// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixCrashPluginTXT.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixCrashPluginTXTPatch::FixCrashPluginTXTPatch() : Module(GlobalEnginePtr)
			{}

			bool FixCrashPluginTXTPatch::HasOption() const
			{
				return false;
			}

			bool FixCrashPluginTXTPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixCrashPluginTXTPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixCrashPluginTXTPatch::GetName() const
			{
				return "Fix Crash PluginTXT";
			}

			bool FixCrashPluginTXTPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixCrashPluginTXTPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for crash when plugins.txt is present in the game root folder. 
					// Buffer overflow in ArchiveManager::OpenMasterArchives when appending to a string.
					// Skip the parsing code completely.
					//
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 6);

					return true;
				}

				return false;
			}

			bool FixCrashPluginTXTPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}