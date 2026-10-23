// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSGeometry.h>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/N/NiColor.h>
#include <EditorAPI/N/NiPoint.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// Shader data loaded from a material file (BGSM). CommonLibF4: BSShaderData.
			// Members follow CommonLibF4; the texture set and the controllers stay untyped pointers.
			class BSShaderData
			{
			public:
				// 1.11.246.0: size 0x1E0, the same layout as CommonLibF4's BSShaderData (the constructor 0x142BC5E40 matches the defaults at each offset:
				// specular 08, colours 0C-18, material paths 58/70, alpha test 84-8D, five BSTArrays from C8, controller pointer 140).
				std::uint32_t refCount; // 00 - BSIntrusiveRefCounted
				std::int32_t lightingShader; // 04
				bool specularEnabled; // 08
				std::uint8_t pad09[0x3]; // 09
				NiAPI::NiColor specularColor; // 0C
				float specularMult; // 18
				NiAPI::NiColor emittanceColor; // 1C
				float emittanceColorScale; // 28
				float lightingInfluence; // 2C
				std::int8_t envmapMinLOD; // 30
				std::uint8_t pad31[0x3]; // 31
				float smoothness; // 34
				float fresnelPower_ShaderDataEntry; // 38
				float wetnessControl_SpecScale_ShaderDataEntry; // 3C
				float wetnessControl_SpecPowerScale_ShaderDataEntry; // 40
				float wetnessControl_SpecMin_ShaderDataEntry; // 44
				float wetnessControl_EnvMapScale_ShaderDataEntry; // 48
				float wetnessControl_FresnelPower_ShaderDataEntry; // 4C
				float wetnessControl_Metalness_ShaderDataEntry; // 50
				bool betnessControl_ScreenSpaceReflections; // 54
				std::uint8_t pad55[0x3]; // 55
				BSFixedString rootMaterialPath; // 58
				float alpha; // 60
				float emittanceMult; // 64
				float lookupScale; // 68
				bool emitEnabled; // 6C
				std::uint8_t pad6D[0x3]; // 6D
				BSFixedString materialPath; // 70
				bool alphaBlend; // 78
				std::uint8_t pad79[0xB]; // 79
				bool alphaTest; // 84
				std::uint8_t pad85[0x3]; // 85
				std::int32_t alphaTestFunction; // 88
				std::int8_t alphaTestRef; // 8C
				bool enableEditorAlphaRef; // 8D
				bool vertexColors; // 8E
				bool vertexAlpha; // 8F
				bool modelSpaceNormals; // 90
				bool decal; // 91
				bool externalEmittance; // 92
				bool hideSecret; // 93
				bool noFade; // 94
				bool dismemberment; // 95
				bool facegen; // 96
				bool skinTint; // 97
				bool hair; // 98
				bool twoSided; // 99
				bool tree; // 9A
				bool bloodEnabled; // 9B
				bool uniqueAlphaProperty; // 9C
				bool effectLightingEnabled; // 9D
				bool castShadows; // 9E
				bool receiveShadows; // 9F
				bool dissolveFade; // A0
				bool glowmap; // A1
				bool assumeShadowmask; // A2
				bool screenSpaceReflections; // A3
				bool nonOccluder; // A4
				bool skewSpecularAlpha; // A5
				bool tessellate; // A6
				bool backLighting; // A7
				bool subSurfaceLighting; // A8
				bool rimLighting; // A9
				std::uint8_t padAA[0x2]; // AA
				float subSurfaceRolloff; // AC
				float rimPower; // B0
				float backLightPower; // B4
				bool anisoLighting; // B8
				bool zBufferWrite; // B9
				bool zBufferTest; // BA
				bool refraction; // BB
				float refractionPower; // BC
				bool refractionFalloff; // C0
				std::uint8_t padC1[0x7]; // C1
				BSTArray<void*> lightingFloatControllers; // C8
				BSTArray<void*> lightingUShortControllers; // E0
				BSTArray<void*> lightingColorControllers; // F8
				BSTArray<void*> effectFloatControllers; // 110
				BSTArray<void*> effectColorControllers; // 128
				void* alphaRefTestController; // 140
				bool parallax; // 148
				bool parallaxOcclusion; // 149
				bool multiLayerParallax; // 14A
				std::uint8_t pad14B[0x1]; // 14B
				float parallaxOcclusionHeightScale; // 14C
				float parallaxOcclusionMaxPasses; // 150
				float parallaxLayerThickness; // 154
				float parallaxRefractionScale; // 158
				float parallaxInnerLayerUScale; // 15C
				float parallaxInnerLayerVScale; // 160
				float displacementTextureBias; // 164
				float displacementTextureScale; // 168
				float tessellationPNScale; // 16C
				float tessellationFadeDistance; // 170
				float tessellationBaseFactor; // 174
				bool tessellationNeedsDominantUVs; // 178
				bool tessellationNeedsCrackFreeNormals; // 179
				bool environmentMapping; // 17A
				bool environmentMappingEye; // 17B
				bool environmentMappingWindow; // 17C
				std::uint8_t pad17D[0x3]; // 17D
				float environmentMappingMaskScale; // 180
				std::int32_t skyTYpe; // 184
				bool reflectionsEnabled; // 188
				bool silhouetteReflectionsEnabled; // 189
				bool refractionsEnabled; // 18A
				bool depthEnabled; // 18B
				bool vertexUVEnabled; // 18C
				bool vertexAlphaDepthEnabled; // 18D
				bool foggingEnabled; // 18E
				bool falloffEnabled; // 18F
				bool rgbFalloffEnabled; // 190
				std::uint8_t pad191[0x3]; // 191
				NiAPI::NiColorA falloffData; // 194
				bool softEnabled; // 1A4
				std::uint8_t pad1A5[0x3]; // 1A5
				float softDepth; // 1A8
				NiAPI::NiPoint2 uvOffset; // 1AC
				NiAPI::NiPoint2 uvScale; // 1B4
				bool grayscaleToPaletteColor; // 1BC
				bool grayscaleToPaletteAlpha; // 1BD
				std::uint8_t pad1BE[0x2]; // 1BE
				NiAPI::NiColor hairTintColor; // 1C0
				std::uint8_t pad1CC[0x4]; // 1CC
				void* textureSet; // 1D0
				std::uint8_t pad1D8[0x8]; // 1D8
				// CommonLibF4: BSShaderData::ApplyMaterialData (CK 1.11.240.0 0x142BCC550; address ID 1666424 in 1.10.982.3 - 1.11.240.0, 797024 in 1.10.162.0 = 0x142AB67E0).
				// Rebuilds the BSShaderProperty material of the geometry from the material file, which resets every material member
				// (including BSLightingShaderMaterialBase::lookupScale, the colour remapping index) to the values stored in the file.
				// Set by the patches that detour it to the original function.
				inline static void(*pointer_ApplyMaterialData)(BSShaderData*, BSGeometry*, bool) { nullptr };

				inline void ApplyMaterialData(BSGeometry* Geometry, bool Flag) noexcept(true)
				{
					pointer_ApplyMaterialData(this, Geometry, Flag);
				}
			};
			static_assert(sizeof(BSShaderData) == 0x1E0);
		}
	}
}
