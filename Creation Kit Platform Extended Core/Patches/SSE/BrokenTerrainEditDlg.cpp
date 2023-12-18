// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "BrokenTerrainEditDlg.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			BrokenTerrainEditDlgPatch::BrokenTerrainEditDlgPatch() : Module(GlobalEnginePtr)
			{}

			bool BrokenTerrainEditDlgPatch::HasOption() const
			{
				return false;
			}

			bool BrokenTerrainEditDlgPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* BrokenTerrainEditDlgPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* BrokenTerrainEditDlgPatch::GetName() const
			{
				return "Broken Terrain Edit Dlg";
			}

			bool BrokenTerrainEditDlgPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool BrokenTerrainEditDlgPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for broken terrain edit dialog undo functionality (Incorrectly removing elements from a linked list,
					// still contains a memory leak)
					//
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 4);
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(1), 4);
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(2), 4);

					return true;
				}
				else if(lpRelocationDatabaseItem->Version() == 2)
				{
					//
					// Fix for broken terrain edit dialog undo functionality (Incorrectly removing elements from a linked list,
					// still contains a memory leak)
					//
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 4);
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(1), 4);

					return true;
				}

				return false;
			}

			bool BrokenTerrainEditDlgPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}