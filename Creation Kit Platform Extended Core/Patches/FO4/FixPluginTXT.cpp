// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixPluginTXT.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			FixPluginTXTPatch::FixPluginTXTPatch() : Module(GlobalEnginePtr)
			{}

			bool FixPluginTXTPatch::HasOption() const
			{
				return false;
			}

			bool FixPluginTXTPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixPluginTXTPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixPluginTXTPatch::GetName() const
			{
				return "Fix for crash when plugins.txt is present";
			}

			bool FixPluginTXTPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixPluginTXTPatch::GetDependencies() const
			{
				return {};
			}

			bool FixPluginTXTPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixPluginTXTPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// Fix for crash when plugins.txt is present in the game root folder. 
					// Buffer overflow in ArchiveManager::OpenMasterArchives when appending to a string. 
					// Skip the parsing code completely.
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 6);

					return true;
				}

				return false;
			}

			bool FixPluginTXTPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}