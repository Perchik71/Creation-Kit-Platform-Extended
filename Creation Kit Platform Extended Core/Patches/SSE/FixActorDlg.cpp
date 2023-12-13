// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixActorDlg.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixActorDlgPatch::FixActorDlgPatch() : Module(GlobalEnginePtr)
			{}

			bool FixActorDlgPatch::HasOption() const
			{
				return false;
			}

			bool FixActorDlgPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixActorDlgPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixActorDlgPatch::GetName() const
			{
				return "Fix Actor Dlg";
			}

			bool FixActorDlgPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixActorDlgPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for the "Actor Flags" or "Actor Behavior" dialogs not showing their column headers. 
					// wParam was swapped to lParam for an unknown reason in SE only. Undo that change.
					//
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0x80, 0x12, 0x00, 0x00 });

					return true;
				}

				return false;
			}

			bool FixActorDlgPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}