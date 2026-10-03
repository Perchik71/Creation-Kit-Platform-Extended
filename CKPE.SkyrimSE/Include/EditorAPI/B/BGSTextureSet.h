#pragma once

#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/B/BSTextureSet.h>
#include <EditorAPI/T/TESTexture.h>
#include <EditorAPI/D/DecalData.h>

#include <array>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSTextureSet :
					public TESBoundObject,
					public BSTextureSet
				{
					enum class Flag : std::uint32_t
					{
						kNone = 0,
						kNoSpecularMap = 1 << 0,
						kFacegenTextures = 1 << 1,
						kHasModelSpaceNormalMap = 1 << 2
					};

					// members
					std::array<TESTexture, std::to_underlying(BSTextureSet::Texture::kTotal)> textures;
					DecalData* decalData;
					TEnumSet<Flag, std::uint32_t> flags;
				public:
					virtual ~BGSTextureSet() = default;

					TESTexture& GetTextureAt(BSTextureSet::Texture a_type) noexcept(true) { return textures[std::to_underlying(a_type)]; }

					[[nodiscard]] inline bool HasNoSpecularMap() const noexcept(true) { return flags.all(Flag::kNoSpecularMap); }
					[[nodiscard]] inline bool HasFacegenTextures() const noexcept(true) { return flags.all(Flag::kFacegenTextures); }
					[[nodiscard]] inline bool HasModelSpaceNormalMap() const noexcept(true) { return flags.all(Flag::kHasModelSpaceNormalMap); }

					inline void SetAsNoSpecularMap() noexcept(true) { flags.set(Flag::kNoSpecularMap); }
					inline void SetAsFacegenTextures() noexcept(true) { flags.set(Flag::kFacegenTextures); }
					inline void SetAsModelSpaceNormalMap() noexcept(true) { flags.set(Flag::kHasModelSpaceNormalMap); }

					[[nodiscard]] inline DecalData* GetDecalData() const noexcept(true) { return decalData; }
				};
				static_assert(sizeof(BGSTextureSet) == 0x1B0);
			}
		}
	}
}