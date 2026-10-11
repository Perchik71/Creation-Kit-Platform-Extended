// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <mutex>
#include <vector>
#include <CKPE.Common.Patch.h>
#include <EditorAPI/B/BSShaderData.h>
#include <EditorAPI/B/BSFaceGenUtils.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			// The hair colour of the NPC preview (Character Gen Parts tab) never changed: the remapping index written to the
			// head part materials by SetupHeadPartTexture is discarded when the material files are applied to the same
			// geometry afterwards (BSShaderData::ApplyMaterialData). Re-apply the recorded index after that call.
			class FixPreviewHairColor : public Common::Patch
			{
				struct Entry
				{
					EditorAPI::NiAPI::NiNode* Scene;
					EditorAPI::BSGeometry* Geometry;
					EditorAPI::BSShaderProperty* Property;
					float LookupScale;
				};

				inline static std::mutex _Lock;
				inline static std::vector<Entry> _Entries;

				static void sub_SetupHeadPartTexture(EditorAPI::NiAPI::NiNode* Scene, EditorAPI::Forms::TESForm* HeadPart,
					EditorAPI::Forms::TESNPC* NPC) noexcept(true);
				static void sub_ApplyMaterialData(EditorAPI::BSShaderData* Data, EditorAPI::BSGeometry* Geometry, bool Flag) noexcept(true);

				FixPreviewHairColor(const FixPreviewHairColor&) = delete;
				FixPreviewHairColor& operator=(const FixPreviewHairColor&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				FixPreviewHairColor();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
				[[nodiscard]] bool SupportsAddressLibrary() const noexcept(true) override { return true; }
			};
		}
	}
}
