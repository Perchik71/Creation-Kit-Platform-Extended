// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/BSShader/BSShaderProperty.h

#pragma once

#include "BSShaderMaterial.h"
#include "NiAPI\NiAlphaProperty.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			using namespace NiAPI;

			class BSRenderPass;

			class BSShaderProperty : public NiAPI::NiShadeProperty
			{
			private:
				static const uint32_t UniqueFlagIndexes[15];
				static const char* UniqueFlagNames[15];
				static const char* FlagNames[64];

			public:
				enum EShaderPropertyFlag
				{
					BSSP_SPECULAR = 0x0,
					BSSP_SKINNED = 0x1,
					BSSP_FLAG_TEMP_REFRACTION = 0x2,
					BSSP_VERTEX_ALPHA = 0x3,
					BSSP_GRAYSCALE_TO_PALETTE_COLOR = 0x4,
					BSSP_GRAYSCALE_TO_PALETTE_ALPHA = 0x5,
					BSSP_FALLOFF = 0x6,
					BSSP_ENVMAP = 0x7,
					BSSP_FLAG_RECEIVE_SHADOWS = 0x8,
					BSSP_FLAG_CAST_SHADOWS = 0x9,
					BSSP_FLAG_FACE = 0xA,
					BSSP_FLAG_PARALLAX = 0xB,
					BSSP_FLAG_MODELSPACENORMALS = 0xC,
					BSSP_FLAG_NON_PROJECTIVE_SHADOWS = 0xD,
					BSSP_FLAG_LANDSCAPE = 0xE,
					BSSP_FLAG_REFRACTION = 0xF,
					BSSP_FLAG_REFRACTION_FIRE = 0x10,
					BSSP_FLAG_EYEREFLECT = 0x11,
					BSSP_FLAG_HAIRTINT = 0x12,
					BSSP_FLAG_SCREENDOOR_ALPHA_FADE = 0x13,
					BSSP_FLAG_LOCALMAP_CLEAR = 0x14,
					BSSP_FLAG_SKIN_TINT = 0x15,
					BSSP_FLAG_OWN_EMIT = 0x16,
					BSSP_FLAG_PROJECTED_UV = 0x17,
					BSSP_FLAG_MULTIPLE_TEXTURES = 0x18,
					BSSP_FLAG_REMAPPABLE_TEXTURES = 0x19,
					BSSP_FLAG_DECAL = 0x1A,
					BSSP_FLAG_DYNAMIC_DECAL = 0x1B,
					BSSP_FLAG_PARALLAX_OCCLUSION = 0x1C,
					BSSP_FLAG_EXTERNAL_EMITTANCE = 0x1D,
					BSSP_FLAG_SOFT_EFFECT = 0x1E,
					BSSP_FLAG_ZBUFFER_TEST = 0x1F,
					BSSP_FLAG_ZBUFFER_WRITE = 0x20,
					BSSP_FLAG_LOD_LANDSCAPE = 0x21,
					BSSP_FLAG_LOD_OBJECTS = 0x22,
					BSSP_FLAG_NOFADE = 0x23,
					BSSP_FLAG_TWO_SIDED = 0x24,
					BSSP_FLAG_VERTEXCOLORS = 0x25,
					BSSP_FLAG_GLOWMAP = 0x26,
					BSSP_FLAG_ASSUME_SHADOWMASK = 0x27,
					BSSP_FLAG_CHARACTER_LIGHTING = 0x28,
					BSSP_FLAG_MULTI_INDEX_SNOW = 0x29,
					BSSP_FLAG_VERTEX_LIGHTING = 0x2A,
					BSSP_FLAG_UNIFORM_SCALE = 0x2B,
					BSSP_FLAG_FIT_SLOPE = 0x2C,
					BSSP_FLAG_BILLBOARD = 0x2D,
					BSSP_FLAG_NO_LOD_LAND_BLEND = 0x2E,
					BSSP_FLAG_ENVMAP_LIGHT_FADE = 0x2F,
					BSSP_FLAG_WIREFRAME = 0x30,
					BSSP_FLAG_WEAPON_BLOOD = 0x31,
					BSSP_FLAG_HIDE_ON_LOCAL_MAP = 0x32,
					BSSP_FLAG_PREMULT_ALPHA = 0x33,
					BSSP_FLAG_CLOUD_LOD = 0x34,
					BSSP_FLAG_ANISOTROPIC_LIGHTING = 0x35,
					BSSP_FLAG_NO_TRANSPARENCY_MULTISAMPLE = 0x36,
					BSSP_FLAG_UNUSED01 = 0x37,
					BSSP_FLAG_MULTI_LAYER_PARALLAX = 0x38,
					BSSP_FLAG_SOFT_LIGHTING = 0x39,
					BSSP_FLAG_RIM_LIGHTING = 0x3A,
					BSSP_FLAG_BACK_LIGHTING = 0x3B,
					BSSP_FLAG_SNOW = 0x3C,
					BSSP_FLAG_TREE_ANIM = 0x3D,
					BSSP_FLAG_EFFECT_LIGHTING = 0x3E,
					BSSP_FLAG_HD_LOD_OBJECTS = 0x3F,
					BSSP_FLAG_COUNT = 0x40,
				};

				struct RenderPassArray
				{
					BSRenderPass* pPassList;
				};

				float fAlpha;
				int iLastRenderPassState;
				uint64_t ulFlags;
				RenderPassArray kRenderPassList;
				char _pad1[0x8];
				RenderPassArray kDebugRenderPassList;
				char _pad2[0x8];
				class BSFadeNode* pFadeNode;
				class BSEffectShaderData* pEffectData;
				class BSShaderPropertyLightData* pLightData;
				BSShaderMaterial* pMaterial;
				char _pad4[0x8];

				float GetAlpha() const;
				class BSFadeNode* QFadeNode() const;
				const RenderPassArray* QRenderPasses() const;
				const RenderPassArray* QDebugRenderPasses() const;

				uint64_t QFlags() const;
				bool GetFlag(uint32_t FlagIndex) const;

				static const char* GetFlagString(uint32_t FlagIndex);
			};
			static_assert(sizeof(BSShaderProperty) == 0x88);
		}
	}
}