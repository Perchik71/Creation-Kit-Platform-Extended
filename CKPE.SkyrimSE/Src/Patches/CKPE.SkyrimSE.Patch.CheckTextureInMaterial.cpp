// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.CheckTextureInMaterial.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			typedef void(*TCheckTextureInMaterialSub)(void*, void*);
			
			static TCheckTextureInMaterialSub CheckTextureInMaterialSub[2];

			CheckTextureInMaterial::CheckTextureInMaterial() : Common::Patch()
			{
				SetName("Check Texture In Material");
			}

			bool CheckTextureInMaterial::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* CheckTextureInMaterial::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool CheckTextureInMaterial::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> CheckTextureInMaterial::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool CheckTextureInMaterial::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CheckTextureInMaterial::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				CheckTextureInMaterialSub[0] = (TCheckTextureInMaterialSub)Detours::DetourJump(__CKPE_OFFSET(0), (uintptr_t)&sub1);
				CheckTextureInMaterialSub[1] = (TCheckTextureInMaterialSub)Detours::DetourJump(__CKPE_OFFSET(1), (uintptr_t)&sub2);

				return true;
			}

			void CheckTextureInMaterial::sub1(void* pSourceTexture, void* Material) noexcept(true)
			{
				if (pSourceTexture && Material)
					CheckTextureInMaterialSub[0](pSourceTexture, Material);
			}

			void CheckTextureInMaterial::sub2(void* pSourceTexture, void* Material) noexcept(true)
			{
				if (pSourceTexture && Material)
					CheckTextureInMaterialSub[1](pSourceTexture, Material);
			}
		}
	}
}