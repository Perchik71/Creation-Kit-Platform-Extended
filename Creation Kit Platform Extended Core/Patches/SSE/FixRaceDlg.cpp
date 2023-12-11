// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixRaceDlg.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixRaceDlgPatch::FixRaceDlgPatch() : Module(GlobalEnginePtr)
			{}

			bool FixRaceDlgPatch::HasOption() const
			{
				return false;
			}

			bool FixRaceDlgPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixRaceDlgPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixRaceDlgPatch::GetName() const
			{
				return "Fix Race Dlg";
			}

			bool FixRaceDlgPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixRaceDlgPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for the "Race" dialog not saving "Specials" if the list box was empty and a new spell was added. 
					// Inverted comparison logic.
					//
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0x74 });

					return true;
				}

				return false;
			}

			bool FixRaceDlgPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}