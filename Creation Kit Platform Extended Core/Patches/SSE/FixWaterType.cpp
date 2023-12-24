// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixWaterType.h"
#include "Editor API/SSE/TESWater.h"
#include "Editor API/SSE/BSShaderProperty.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_FixWaterType_sub = 0;

			FixWaterTypePatch::FixWaterTypePatch() : Module(GlobalEnginePtr)
			{}

			bool FixWaterTypePatch::HasOption() const
			{
				return false;
			}

			bool FixWaterTypePatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixWaterTypePatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixWaterTypePatch::GetName() const
			{
				return "Fix Water Type";
			}

			bool FixWaterTypePatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixWaterTypePatch::GetDependencies() const
			{
				return {};
			}

			bool FixWaterTypePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixWaterTypePatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for "Water Type" window options not updating water in the "Render Window" preview
					//
					TESWaterRoot::SingletonFunc = (TESWaterRoot*(*)())(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0)));
					pointer_FixWaterType_sub = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(2), (uintptr_t)&sub);
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(3), 2);
					lpRelocator->Patch(lpRelocationDatabaseItem->At(4), { 0xEB });

					return true;
				}

				return false;
			}

			bool FixWaterTypePatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void FixWaterTypePatch::sub(TESForm* DialogForm, __int64 Unused)
			{
				auto waterRoot = TESWaterRoot::Singleton();

				for (uint32_t i = 0; i < waterRoot->m_WaterObjects.QSize(); i++)
				{
					if (DialogForm->GetFormID() == waterRoot->m_WaterObjects[i]->m_BaseWaterForm->GetFormID())
						((void(__fastcall*)(TESForm*, class BSShaderMaterial*))pointer_FixWaterType_sub)(DialogForm, 
							waterRoot->m_WaterObjects[i]->m_TriShape->QShaderProperty()->pMaterial);
				}
			}
		}
	}
}