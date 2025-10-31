// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "NiAPI/NiTypes.h"
#include "NiAPI/NiPoint.h"
#include "NiAPI/NiPointer.h"
#include "NiAPI/NiTexture.h"
#include "BSTextureSet.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// 0x38
			class BSShaderMaterial
			{
			public:
				enum Types
				{
					Default = 0,
					Envmap,
					Glowmap,
					Parallax,
					Face,
					SkinTint,
					HairTint,
					ParallaxOcc,
					Landscape,
					LODLandscape,
					Snow,
					MultiLayerParallax,
					TreeAnim,
					LODObjects,
					MultiIndexSnow,
					LODObjectsHD,
					Eye,
					Cloud,
					LODLandscapeNoise,
					LODLandscapeBlend,
					Dismemberment
				};

				virtual ~BSShaderMaterial() = default;

				inline BSShaderMaterial* Create() const noexcept(true) { CKPE::vtbl_call<void>(0x8, this); }
				inline std::uint32_t GetType() const noexcept(true) { return CKPE::vtbl_call<std::uint32_t>(0x28, this); }
				inline NiAPI::NiPoint2 GetTextCoordOffset() const noexcept(true) { return _TextCoordOffset[0]; }
				inline NiAPI::NiPoint2 GetTextCoordScale() const noexcept(true) { return _TextCoordScale[0]; }

				inline void SetOffsetUV(float u, float v) noexcept(true)
				{
					_TextCoordOffset[0].x = u;
					_TextCoordOffset[1].x = u;
					_TextCoordOffset[0].y = v;
					_TextCoordOffset[1].y = v;
				}

				inline void GetOffsetUV(float* u, float* v) const noexcept(true)
				{
					*u = _TextCoordOffset[0].x;
					*v = _TextCoordOffset[0].y;
				}

				inline void SetScaleUV(float u, float v) noexcept(true)
				{
					_TextCoordScale[0].x = u;
					_TextCoordScale[1].x = u;
					_TextCoordScale[0].y = v;
					_TextCoordScale[1].y = v;
				}

				inline void GetScaleUV(float* u, float* v) const noexcept(true)
				{
					*u = _TextCoordScale[0].x;
					*v = _TextCoordScale[0].y;
				}
			protected:
				volatile std::uint32_t	_RefCount;				// 08
				NiAPI::NiPoint2			_TextCoordOffset[2];	// 0C
				NiAPI::NiPoint2			_TextCoordScale[2];		// 1C
				char					_Pad1C[0xC];			// 2C
			};
			static_assert(sizeof(BSShaderMaterial) == 0x38);

			// C0
			class BSLightingShaderMaterialBase : public BSShaderMaterial
			{
			public:
				virtual ~BSLightingShaderMaterialBase() = default;

				inline bool IsLightingShaderMaterialEnvmap() const noexcept(true) { CKPE::vtbl_call<void>(0x68, this); }
				inline void ClearTextures() const noexcept(true) { CKPE::vtbl_call<void>(0x70, this); }
				inline void GetTextures(NiAPI::NiTexture** Textures, std::uint32_t MaxSize) const noexcept(true) { CKPE::vtbl_call<void>(0x78, this, Textures, MaxSize); }
				inline void SaveToStream(void* Stream) const noexcept(true) { CKPE::vtbl_call<void>(0x80, this, Stream); }
				inline void LoadFromStream(void* Stream) const noexcept(true) { CKPE::vtbl_call<void>(0x88, this, Stream); }
				inline void PrefetchTextures() const noexcept(true) { CKPE::vtbl_call<void>(0x90, this); }
				inline void LoadTextureSet(BSShaderTextureSet* TextureSet) const noexcept(true) { CKPE::vtbl_call<void>(0xA0, this, TextureSet); }
				inline NiAPI::NiColor& SpecularColor() { return _SpecularColor; }
				inline NiAPI::NiPointer<NiAPI::NiTexture> GetDiffuseTexture() const noexcept(true) { return _DiffuseTexture; }
				inline NiAPI::NiPointer<NiAPI::NiTexture> GetNormalTexture() const noexcept(true) { return _NormalTexture; }
				inline NiAPI::NiPointer<NiAPI::NiTexture> GetRimSoftLightingTexture() const noexcept(true) { return _RimSoftLightingTexture; }
				inline NiAPI::NiPointer<NiAPI::NiTexture> GetSmoothnessSpecMaskTexture() const noexcept(true) { return _SmoothnessSpecMaskTexture; }
				inline NiAPI::NiPointer<NiAPI::NiTexture> GetLookupTexture() const noexcept(true) { return _LookupTexture; }
				inline std::uint64_t& TextureClampMode() noexcept(true) { return _TextureClampMode; }
				inline NiAPI::NiPointer<BSShaderTextureSet> GetTextureSet() const noexcept(true) { return _TextureSet; }
				inline float& MaterialAlpha() { return _MaterialAlpha; }
				inline float& RefractionPower() { return _RefractionPower; }
				inline float& Smoothness() { return _Smoothness; }
				inline float& SpecularColorScale() { return _SpecularColorScale; }
				inline float& FresnelPower() { return _FresnelPower; }
				inline float& Wetness_SpecScale() { return _Wetness_SpecScale; }
				inline float& Wetness_SpecPowerScale() { return _Wetness_SpecPowerScale; }
				inline float& Wetness_SpecMin() { return _Wetness_SpecMin; }
				inline float& Wetness_EnvMapScale() { return _Wetness_EnvMapScale; }
				inline float& Wetness_FresnelPower() { return _Wetness_FresnelPower; }
				inline float& Wetness_Metalness() { return _Wetness_Metalness; }
				inline float& SubsurfaceLightRolloff() { return _SubsurfaceLightRolloff; }
				inline float& RimLightPower() { return _RimLightPower; }
				inline float& BackLightPower() { return _BackLightPower; }
				inline float& LookupScale() { return _LookupScale; }
			protected:
				NiAPI::NiColor							_SpecularColor;				// 38
				char									_Pad44[4];					// 44
				NiAPI::NiPointer<NiAPI::NiTexture>		_DiffuseTexture;			// 48
				NiAPI::NiPointer<NiAPI::NiTexture>		_NormalTexture;				// 50
				NiAPI::NiPointer<NiAPI::NiTexture>		_RimSoftLightingTexture;	// 58
				NiAPI::NiPointer<NiAPI::NiTexture>		_SmoothnessSpecMaskTexture;	// 60
				NiAPI::NiPointer<NiAPI::NiTexture>		_LookupTexture;				// 68
				std::uint64_t							_TextureClampMode;			// 70
				NiAPI::NiPointer<BSShaderTextureSet>	_TextureSet;				// 78
				float									_MaterialAlpha;				// 80
				float									_RefractionPower;			// 84
				float									_Smoothness;				// 88
				float									_SpecularColorScale;		// 8C
				float									_FresnelPower;				// 90
				float									_Wetness_SpecScale;			// 94
				float									_Wetness_SpecPowerScale;	// 98
				float									_Wetness_SpecMin;			// 9C
				float									_Wetness_EnvMapScale;		// A0
				float									_Wetness_FresnelPower;		// A4
				float									_Wetness_Metalness;			// A8
				float									_SubsurfaceLightRolloff;	// AC
				float									_RimLightPower;				// B0
				float									_BackLightPower;			// B4
				float									_LookupScale;				// B8
				std::uint32_t							_Unk;						// BC
			};
			static_assert(sizeof(BSLightingShaderMaterialBase) == 0xC0);

			// C8
			class BSLightingShaderMaterialFace : public BSLightingShaderMaterialBase
			{
			public:
				inline NiAPI::NiPointer<NiAPI::NiTexture> GetBaseDiffuse() const { return _BaseDiffuse; }
			private:
				NiAPI::NiPointer<NiAPI::NiTexture>	_BaseDiffuse;				// C0
			};
			static_assert(sizeof(BSLightingShaderMaterialFace) == 0xC8);
		}
	}
}