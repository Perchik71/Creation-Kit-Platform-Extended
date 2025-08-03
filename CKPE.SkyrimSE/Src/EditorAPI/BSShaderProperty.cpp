// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/BSShader/BSShaderProperty.cpp

#include <EditorAPI/BSShaderProperty.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			const std::uint32_t BSShaderProperty::UniqueFlagIndexes[15] =
			{
				0x07, 0x26, 0x0B, 0x0A, 0x15, 0x12, 0x1C,
				0x0E, 0x21, 0x38, 0x3D, 0x22, 0x29, 0x3F,
				0x2E
			};

			const char* BSShaderProperty::UniqueFlagNames[15] =
			{
				"Envmap",
				"Glowmap",
				"Parallax",
				"Facegen",
				"Facegen RGB Tint",
				"Hair Tint",
				"Parallax Occlusion",
				"Multitexture Landscape",
				"LOD Landscape",
				"Multilayer Parallax",
				"Tree Anim",
				"LOB Objects",
				"MultiIndex",
				"LOD Objects HD",
				"Multitexture Landscape (LOD Blend)"
			};

			const char* BSShaderProperty::FlagNames[64] =
			{
				"specular",
				"skinned",
				"temp refraction",
				"vertex alpha",
				"grayscale to palette - color",
				"grayscale to palette - alpha",
				"falloff",
				"environment mapping",
				"receive shadows",
				"cast shadows",
				"facegen",
				"parallax",
				"model-space normals",
				"non-projective shadows",
				"landscape",
				"refraction",
				"fire refraction",
				"eye environment mapping",
				"hair",
				"screendoor alpha fade",
				"localmap hide secret",
				"facegen rgb tint",
				"own emit",
				"projected uv",
				"multiple textures",
				"remappable textures",
				"decal",
				"dynamic decal",
				"parallax occlusion",
				"external emittance",
				"soft effect",
				"zbuffer test",
				"zbuffer write",
				"lod landscape",
				"lod objects",
				"no fade",
				"two-sided geometry",
				"vertex colors",
				"glow map",
				"assume shadowmask",
				"character lighting",
				"multi index snow",
				"vertex lighting",
				"uniform scale",
				"fit slope",
				"billboard",
				"no lod land blend",
				"envmap light fade",
				"wireframe",
				"weapon blood",
				"hide on local map",
				"premult alpha",
				"cloud LOD",
				"anisotropic lighting",
				"no transparency multisampling",
				"unused01",
				"multi layer parallax",
				"soft lighting",
				"rim lighting",
				"back lighting",
				"snow",
				"tree anim",
				"effect lighting",
				"hd lod objects"
			};

			float BSShaderProperty::GetAlpha() const
			{
				return fAlpha;
			}

			class BSFadeNode* BSShaderProperty::QFadeNode() const
			{
				return pFadeNode;
			}

			const BSShaderProperty::RenderPassArray* BSShaderProperty::QRenderPasses() const
			{
				return &kRenderPassList;
			}

			const BSShaderProperty::RenderPassArray* BSShaderProperty::QDebugRenderPasses() const
			{
				return &kDebugRenderPassList;
			}

			uint64_t BSShaderProperty::QFlags() const
			{
				return ulFlags;
			}

			bool BSShaderProperty::GetFlag(std::uint32_t FlagIndex) const
			{
				if (FlagIndex >= BSSP_FLAG_COUNT)
					return false;

				return QFlags() & (1ull << FlagIndex);
			}

			const char* BSShaderProperty::GetFlagString(std::uint32_t FlagIndex)
			{
				if (FlagIndex >= BSSP_FLAG_COUNT)
					return nullptr;

				return FlagNames[FlagIndex];
			}
		}
	}
}
