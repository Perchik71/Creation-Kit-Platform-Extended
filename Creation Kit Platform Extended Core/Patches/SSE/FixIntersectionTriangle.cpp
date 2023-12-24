// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/SSE/BSShaderResourceManager.h"
#include "FixIntersectionTriangle.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			extern uintptr_t pointer_PickGetRecord_sub;
		}
	}

	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			using namespace EditorAPI::SkyrimSpectialEdition;

			FixIntersectionTrianglePatch::FixIntersectionTrianglePatch() : Module(GlobalEnginePtr)
			{}

			bool FixIntersectionTrianglePatch::HasOption() const
			{
				return false;
			}

			bool FixIntersectionTrianglePatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixIntersectionTrianglePatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixIntersectionTrianglePatch::GetName() const
			{
				return "Fix Intersection Triangle";
			}

			bool FixIntersectionTrianglePatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixIntersectionTrianglePatch::GetDependencies() const
			{
				return {};
			}

			bool FixIntersectionTrianglePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixIntersectionTrianglePatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				auto verPatch = lpRelocationDatabaseItem->Version();

				if ((verPatch >= 1) && (verPatch < 3))
				{
					//
					// Rewrite their ray->triangle intersection function. This fixes 3 things:
					//
					// - Being unable to select certain objects in the render view window.
					// - Selections not scaling correctly depending on distance (ex. LOD terrain) and NiObject scale.
					// - The Object Palette window "Conform to slope" option causing broken object angles on placement. SE changed data
					// layouts and geometry vertex data may not include normals.
					// 
					// modify in 1.6.438:
					//
					pointer_PickGetRecord_sub = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0));

					if (verPatch == 2)
						Detours::X64::DetourClassVTable(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1)),
							&BSShaderResourceManager_CK::FindIntersectionsTriShapeFastPathEx, 34);
					else
						Detours::X64::DetourClassVTable(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1)),
							&BSShaderResourceManager_CK::FindIntersectionsTriShapeFastPath, 34);

					return true;
				}

				return false;
			}

			bool FixIntersectionTrianglePatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}