// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/BSShaderResourceManager.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixIntersectionTriangle.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			extern std::uintptr_t pointer_PickGetRecord_sub;
		}

		namespace Patch
		{
			FixIntersectionTriangle::FixIntersectionTriangle() : Common::Patch()
			{
				SetName("Fix Intersection Triangle");
			}

			bool FixIntersectionTriangle::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixIntersectionTriangle::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixIntersectionTriangle::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> FixIntersectionTriangle::GetDependencies() const noexcept(true)
			{
				return { "Replace BSPointerHandle And Manager" };
			}

			bool FixIntersectionTriangle::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixIntersectionTriangle::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixIntersectionTriangle::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				// Rewrite their ray->triangle intersection function. This fixes 3 things:
				//
				// - Being unable to select certain objects in the render view window.
				// - Selections not scaling correctly depending on distance (ex. LOD terrain) and NiObject scale.
				// - The Object Palette window "Conform to slope" option causing broken object angles on placement. SE changed data
				// layouts and geometry vertex data may not include normals.
				// 
				// modify in 1.6.438:
				//
				if (VersionLists::GetEditorVersion() >= VersionLists::EDITOR_SKYRIM_SE_1_6_438)
					Relocation(ID(132528)).WriteVFunc(34, &EditorAPI::BSShaderResourceManager::FindIntersectionsTriShapeFastPathEx);
				else
					Relocation(ID(132528)).WriteVFunc(34, &EditorAPI::BSShaderResourceManager::FindIntersectionsTriShapeFastPath);

				EditorAPI::pointer_PickGetRecord_sub = ID{ 551353, 1014796 }.Address();

				return true;
			}
		}
	}
}