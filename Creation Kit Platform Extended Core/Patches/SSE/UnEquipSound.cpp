// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "UnEquipSound.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			UnEquipSoundPatch::UnEquipSoundPatch() : Module(GlobalEnginePtr)
			{}

			bool UnEquipSoundPatch::HasOption() const
			{
				return false;
			}

			bool UnEquipSoundPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* UnEquipSoundPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* UnEquipSoundPatch::GetName() const
			{
				return "UnEquip Sound";
			}

			bool UnEquipSoundPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool UnEquipSoundPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Correct the "Push-to-game not supported" error when clicking the "UnEquip Sound" button on the weapon editor
					// dialog or "Add" button on the music track dialog. 3682 is reserved exclusively for the PTG functionality, so
					// the button ids must be changed. Disable PTG code instead.
					//
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 6);

					return true;
				}

				return false;
			}

			bool UnEquipSoundPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}