// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixDialogueBranch.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixDialogueBranchPatch::FixDialogueBranchPatch() : Module(GlobalEnginePtr)
			{}

			bool FixDialogueBranchPatch::HasOption() const
			{
				return false;
			}

			bool FixDialogueBranchPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixDialogueBranchPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixDialogueBranchPatch::GetName() const
			{
				return "Fix Dialogue Branch";
			}

			bool FixDialogueBranchPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixDialogueBranchPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for the "Dialogue Branch" dialog showing corrupted starting topic strings. 
					// The address of a variable is provided instead of a string pointer. 
					// Change LEA to MOV.
					//
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0x4C, 0x8B });

					return true;
				}

				return false;
			}

			bool FixDialogueBranchPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}