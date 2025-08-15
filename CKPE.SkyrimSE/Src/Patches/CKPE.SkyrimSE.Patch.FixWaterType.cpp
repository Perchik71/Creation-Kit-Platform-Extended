// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
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
			std::uintptr_t pointer_FixWaterType_sub = 0;

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

			bool FixWaterType::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixWaterType::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for "Water Type" window options not updating water in the "Render Window" preview
				//
				EditorAPI::TESWaterRoot::SingletonFunc = (EditorAPI::TESWaterRoot* (*)())(__CKPE_OFFSET(0));
				pointer_FixWaterType_sub = __CKPE_OFFSET(1);
				Detours::DetourCall(__CKPE_OFFSET(2), (std::uintptr_t)&sub);
				SafeWrite::WriteNop(__CKPE_OFFSET(3), 2);
				SafeWrite::Write(__CKPE_OFFSET(4), { 0xEB });

				return true;
			}

			void FixWaterType::sub(void* DialogForm, std::int64_t Unused) noexcept(true)
			{
				auto waterRoot = EditorAPI::TESWaterRoot::Singleton();
				for (std::uint32_t i = 0; i < waterRoot->m_WaterObjects.size(); i++)
				{
					auto dlg = (EditorAPI::Forms::TESForm*)DialogForm;
					if (dlg->GetFormID() == waterRoot->m_WaterObjects[i]->m_BaseWaterForm->GetFormID())
						((void(__fastcall*)(EditorAPI::Forms::TESForm*, EditorAPI::BSShaderMaterial*))
							pointer_FixWaterType_sub)(dlg, 
								waterRoot->m_WaterObjects[i]->m_TriShape->QShaderProperty()->pMaterial);
				}
			}
		}
	}
}