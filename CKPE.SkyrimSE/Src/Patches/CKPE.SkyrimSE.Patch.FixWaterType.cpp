// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/TESWater.h>
#include <EditorAPI/BSShaderProperty.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixWaterType.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			using TFixWaterType_sub = void(EditorAPI::Forms::TESForm*, EditorAPI::BSShaderMaterial*);

			static std::function<TFixWaterType_sub> FixWaterType_sub;

			FixWaterType::FixWaterType() : Common::Patch()
			{
				SetName("Fix Water Type");
			}

			bool FixWaterType::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixWaterType::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixWaterType::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixWaterType::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixWaterType::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixWaterType::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixWaterType::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for "Water Type" window options not updating water in the "Render Window" preview
				//
				EditorAPI::TESWaterRoot::SingletonFunc = (EditorAPI::TESWaterRoot* (*)())(ID(571218).Address());
				FixWaterType_sub = Relocation<TFixWaterType_sub>(ID(296818)).Get();
				auto target = ID(256456);
				Relocation(target, 0xE3).WriteFill(NOP, 2);
				Relocation(target, 0xF6).WriteCall(&sub);
				Relocation(ID(296818), 0x38).Write(JMP);
				
				return true;
			}

			void FixWaterType::sub(void* DialogForm, [[maybe_unused]] std::int64_t Unused) noexcept(true)
			{
				auto waterRoot = EditorAPI::TESWaterRoot::Singleton();
				for (std::uint32_t i = 0; i < waterRoot->m_WaterObjects.size(); i++)
				{
					auto dlg = (EditorAPI::Forms::TESForm*)DialogForm;
					if (dlg->GetFormID() == waterRoot->m_WaterObjects[i]->m_BaseWaterForm->GetFormID())
						FixWaterType_sub(dlg, waterRoot->m_WaterObjects[i]->m_TriShape->QShaderProperty()->pMaterial);
				}
			}
		}
	}
}