// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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
			using TCheckTextureInMaterialSub = void(void*, void*);
			static std::function<TCheckTextureInMaterialSub> CheckTextureInMaterialSub[2];

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

			bool CheckTextureInMaterial::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool CheckTextureInMaterial::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CheckTextureInMaterial::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				CheckTextureInMaterialSub[0] = reinterpret_cast<TCheckTextureInMaterialSub*>(Relocation(ID(212952)).WriteJump(&sub1));
				CheckTextureInMaterialSub[1] = reinterpret_cast<TCheckTextureInMaterialSub*>(Relocation(ID(659133)).WriteJump(&sub2));

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