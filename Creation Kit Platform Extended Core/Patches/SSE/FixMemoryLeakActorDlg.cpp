// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixMemoryLeakActorDlg.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixMemoryLeakActorDlgPatch::FixMemoryLeakActorDlgPatch() : Module(GlobalEnginePtr)
			{}

			bool FixMemoryLeakActorDlgPatch::HasOption() const
			{
				return false;
			}

			bool FixMemoryLeakActorDlgPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixMemoryLeakActorDlgPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixMemoryLeakActorDlgPatch::GetName() const
			{
				return "Fix Memory Leak ActorDlg";
			}

			bool FixMemoryLeakActorDlgPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixMemoryLeakActorDlgPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for a memory leak in BSShadowLight::ClearShadowMapData after opening "Actor" dialogs (~500kb per instance). 
					// The code loops over a ShadowMapData array and checks if ShadowMapIndex is NOT -1, freeing the data if true. 
					// When opening a dialog this is always -1 and it never gets deallocated. Hacky fix: remove the check.
					//
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 6);
					
					return true;
				}

				return false;
			}

			bool FixMemoryLeakActorDlgPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}