// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/EditorUI.h"
#include "Editor API/SSE/TESObjectREFR.h"
#include "CheckTextureInMaterial.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_CheckTextureInMaterial_sub1 = 0;
			uintptr_t pointer_CheckTextureInMaterial_sub2 = 0;

			CheckTextureInMaterialPatch::CheckTextureInMaterialPatch() : Module(GlobalEnginePtr)
			{}

			bool CheckTextureInMaterialPatch::HasOption() const
			{
				return false;
			}

			bool CheckTextureInMaterialPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* CheckTextureInMaterialPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* CheckTextureInMaterialPatch::GetName() const
			{
				return "Check Texture In Material";
			}

			bool CheckTextureInMaterialPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> CheckTextureInMaterialPatch::GetDependencies() const
			{
				return {};
			}

			bool CheckTextureInMaterialPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return (eEditorCurrentVersion == EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_1_5_73) ||
					(eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_1_6_1130);
			}

			bool CheckTextureInMaterialPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				auto verPatch = lpRelocationDatabaseItem->Version();

				if (verPatch == 1)
				{
					pointer_CheckTextureInMaterial_sub1 = lpRelocator->DetourFunctionClass(_RELDATA_RAV(0), (uintptr_t)sub1);
					pointer_CheckTextureInMaterial_sub2 = lpRelocator->DetourFunctionClass(_RELDATA_RAV(1), (uintptr_t)sub2);

					return true;
				}

				return false;
			}

			bool CheckTextureInMaterialPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void CheckTextureInMaterialPatch::sub1(void* pSourceTexture, void* Material) noexcept(true)
			{
				if (pSourceTexture && Material)
					fastCall<void>(pointer_CheckTextureInMaterial_sub1, pSourceTexture, Material);
			}

			void CheckTextureInMaterialPatch::sub2(void* pSourceTexture, void* Material) noexcept(true)
			{
				if (pSourceTexture && Material)
					fastCall<void>(pointer_CheckTextureInMaterial_sub2, pSourceTexture, Material);
			}
		}
	}
}