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
			using TCheckTextureInMaterialSub_1 = void(void*, void*);
			using TCheckTextureInMaterialSub_2 = void(std::uint64_t, void*, void*);

			static std::function<TCheckTextureInMaterialSub_1> CheckTextureInMaterialSub_1;
			static std::function<TCheckTextureInMaterialSub_2> CheckTextureInMaterialSub_2;

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

				CheckTextureInMaterialSub_1 = reinterpret_cast<TCheckTextureInMaterialSub_1*>(Relocation(ID(212952)).WriteJump(&sub1));
				CheckTextureInMaterialSub_2 = reinterpret_cast<TCheckTextureInMaterialSub_2*>(Relocation(ID(659133)).WriteJump(&sub2));

				return true;
			}

			void CheckTextureInMaterial::sub1(void* pSourceTexture, void* Material) noexcept(true)
			{
				if (pSourceTexture && Material)
					CheckTextureInMaterialSub_1(pSourceTexture, Material);
			}

			void CheckTextureInMaterial::sub2(std::uint64_t uNums, void* pSourceTexture, void* Material) noexcept(true)
			{
				if (pSourceTexture && Material)
					CheckTextureInMaterialSub_2(uNums, pSourceTexture, Material);
			}
		}
	}
}