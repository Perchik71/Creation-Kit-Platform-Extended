// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <algorithm>
#include <EditorAPI/IDs.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/B/BSTriShape.h>
#include <Patches/CKPE.Fallout4.Patch.FixPreviewHairColor.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			FixPreviewHairColor::FixPreviewHairColor() : Common::Patch()
			{
				SetName("Fixed hair colour in the NPC preview");
			}

			bool FixPreviewHairColor::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixPreviewHairColor::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixPreviewHairColor::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixPreviewHairColor::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixPreviewHairColor::DoQuery() const noexcept(true)
			{
				// The functions are byte-identical (BinDiff similarity 1.0) and share their address IDs in 1.10.982.3, 1.11.137.0 and 1.11.240.0.
				// In 1.10.162.0 they were located by structure (IDs 685825 and 797024); every offset used here matches the later versions.
				const auto version = VersionLists::GetEditorVersion();
				return (version != VersionLists::EDITOR_FALLOUT_C4_1_10_943_1) && (version <= VersionLists::EDITOR_FALLOUT_C4_LAST);
			}

			bool FixPreviewHairColor::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				EditorAPI::BSFaceGenUtils::pointer_SetupHeadPartTexture = reinterpret_cast<void(*)(EditorAPI::NiAPI::NiNode*,
					EditorAPI::Forms::TESForm*, EditorAPI::Forms::TESNPC*)>(Relocation(EditorAPI::IDs::Functions::BSFaceGenUtils__SetupHeadPartShaderState).WriteJump(&sub_SetupHeadPartTexture));
				EditorAPI::BSShaderData::pointer_ApplyMaterialData = reinterpret_cast<void(*)(EditorAPI::BSShaderData*,
					EditorAPI::BSGeometry*, bool)>(Relocation(EditorAPI::IDs::BSShaderData::ApplyMaterialData).WriteJump(&sub_ApplyMaterialData));

				return true;
			}

			void FixPreviewHairColor::sub_SetupHeadPartTexture(EditorAPI::NiAPI::NiNode* Scene, EditorAPI::Forms::TESForm* HeadPart,
				EditorAPI::Forms::TESNPC* NPC) noexcept(true)
			{
				EditorAPI::BSFaceGenUtils::SetupHeadPartTexture(Scene, HeadPart, NPC);

				if (!Scene || !HeadPart)
					return;

				// Find the geometry the same way the original function does (vtable +0x50) and remember the remapping index it has just written.
				auto Object = Scene->GetObjectByName(HeadPart->GetEntryEditID());
				EditorAPI::BSGeometry* Geometry = Object ? Object->IsTriShape() : nullptr;
				if (!Geometry)
					return;

				auto Property = Geometry->GetShaderProperty().GetPtr();
				auto Material = Property ? Property->GetShaderMaterial() : nullptr;
				if (!Material)
					return;

				const Entry NewEntry{ Scene, Geometry, Property, static_cast<EditorAPI::BSLightingShaderMaterialBase*>(Material)->LookupScale() };

				std::lock_guard<std::mutex> lock(_Lock);

				// A different head scene means a new preview was built: forget the geometry of the old one.
				if (!_Entries.empty() && (_Entries.front().Scene != Scene))
					_Entries.clear();

				auto It = std::find_if(_Entries.begin(), _Entries.end(), [Geometry](const Entry& e) { return e.Geometry == Geometry; });
				if (It != _Entries.end())
					*It = NewEntry;
				else
					_Entries.push_back(NewEntry);
			}

			void FixPreviewHairColor::sub_ApplyMaterialData(EditorAPI::BSShaderData* Data, EditorAPI::BSGeometry* Geometry, bool Flag) noexcept(true)
			{
				Data->ApplyMaterialData(Geometry, Flag);

				if (!Geometry)
					return;

				std::lock_guard<std::mutex> lock(_Lock);
				if (_Entries.empty())
					return;

				auto Property = Geometry->GetShaderProperty().GetPtr();
				auto Material = Property ? Property->GetShaderMaterial() : nullptr;
				if (!Material)
					return;

				// The material file has just reset the remapping index of the head part: put back the one SetupHeadPartTexture chose.
				for (const auto& Item : _Entries)
				{
					if ((Item.Geometry == Geometry) && (Item.Property == Property))
					{
						static_cast<EditorAPI::BSLightingShaderMaterialBase*>(Material)->LookupScale() = Item.LookupScale;
						break;
					}
				}
			}
		}
	}
}
