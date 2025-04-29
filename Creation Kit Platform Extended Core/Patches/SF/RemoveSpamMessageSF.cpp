// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "RemoveSpamMessageSF.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
		{
			RemoveSpamMessagePatch::RemoveSpamMessagePatch() : Module(GlobalEnginePtr)
			{}

			bool RemoveSpamMessagePatch::HasOption() const
			{
				return false;
			}

			bool RemoveSpamMessagePatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* RemoveSpamMessagePatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* RemoveSpamMessagePatch::GetName() const
			{
				return "Spam Message";
			}

			bool RemoveSpamMessagePatch::HasDependencies() const
			{
				return false;
			}

			Array<String> RemoveSpamMessagePatch::GetDependencies() const
			{
				return {};
			}

			bool RemoveSpamMessagePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_1_14_78_0;
			}

			bool RemoveSpamMessagePatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					lpRelocator->Patch(_RELDATA_RAV(0), { 0xC3, 0x90, 0x90, 0x90, 0x90 });

					return true;
				}

				return false;
			}

			bool RemoveSpamMessagePatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}