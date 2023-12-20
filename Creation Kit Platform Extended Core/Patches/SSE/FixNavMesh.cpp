// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixNavMesh.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_FixNavMesh_sub = 0;

			FixNavMeshPatch::FixNavMeshPatch() : Module(GlobalEnginePtr)
			{}

			bool FixNavMeshPatch::HasOption() const
			{
				return false;
			}

			bool FixNavMeshPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixNavMeshPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixNavMeshPatch::GetName() const
			{
				return "Fix NavMesh";
			}

			bool FixNavMeshPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				// Fixed in 1.5.73.
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_1_5_3;
			}

			bool FixNavMeshPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for incorrect NavMesh assertion while saving certain ESP files (i.e 3DNPC.esp).
					//
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);
					pointer_FixNavMesh_sub = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));

					return true;
				}

				return false;
			}

			bool FixNavMeshPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void FixNavMeshPatch::sub(__int64 FileHandle, __int64* Value)
			{
				// The pointer is converted to a form id, but the top 32 bits are never cleared correctly (ui32/ui64 union)
				*Value &= 0x00000000FFFFFFFF;

				((void(__fastcall*)(__int64, __int64*))pointer_FixNavMesh_sub)(FileHandle, Value);
			}
		}
	}
}