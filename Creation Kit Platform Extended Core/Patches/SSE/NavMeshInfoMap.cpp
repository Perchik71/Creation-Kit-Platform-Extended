// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "NavMeshInfoMap.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			NavMeshInfoMapPatch::NavMeshInfoMapPatch() : Module(GlobalEnginePtr)
			{}

			bool NavMeshInfoMapPatch::HasOption() const
			{
				return false;
			}

			bool NavMeshInfoMapPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* NavMeshInfoMapPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* NavMeshInfoMapPatch::GetName() const
			{
				return "NavMesh Info Map";
			}

			bool NavMeshInfoMapPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> NavMeshInfoMapPatch::GetDependencies() const
			{
				return {};
			}

			bool NavMeshInfoMapPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool NavMeshInfoMapPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix use-after-free with a NavMeshInfoMap inserted in the altered forms list during a virtual destructor call. 
					// NavMeshInfoMap::Clear.
					//
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 6);

					return true;
				}

				return false;
			}

			bool NavMeshInfoMapPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}