// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once
#pragma pack(push, 1)

#include "NiAPI/NiTypes.h"
#include "NiAPI/NiPoint.h"
#include "NiAPI/NiPointer.h"
#include "NiAPI/NiTexture.h"
#include "BSTextureSet.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			class NiStream;

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

				inline BSShaderMaterial* Create() const { thisVirtualCall<void>(0x8, this); }
				inline uint32_t GetType() { return thisVirtualCall<uint32_t>(0x28, this); }
				inline NiAPI::NiPoint2 GetTextCoordOffset() const { return _TextCoordOffset[0]; }
				inline NiAPI::NiPoint2 GetTextCoordScale() const { return _TextCoordScale[0]; }

				inline void SetOffsetUV(float u, float v)
				{
					_TextCoordOffset[0].x = u;
					_TextCoordOffset[1].x = u;
					_TextCoordOffset[0].y = v;
					_TextCoordOffset[1].y = v;
				}

				inline void GetOffsetUV(float* u, float* v)
				{
					*u = _TextCoordOffset[0].x;
					*v = _TextCoordOffset[0].y;
				}

				inline void SetScaleUV(float u, float v)
				{
					_TextCoordScale[0].x = u;
					_TextCoordScale[1].x = u;
					_TextCoordScale[0].y = v;
					_TextCoordScale[1].y = v;
				}

				inline void GetScaleUV(float* u, float* v)
				{
					*u = _TextCoordScale[0].x;
					*v = _TextCoordScale[0].y;
				}
			protected:
				volatile uint32_t	_RefCount;				// 08
				NiAPI::NiPoint2		_TextCoordOffset[2];	// 0C
				NiAPI::NiPoint2		_TextCoordScale[2];		// 1C
				char				_Pad1C[0xC];			// 2C
			};
			static_assert(sizeof(BSShaderMaterial) == 0x38);

			// C0
			class BSLightingShaderMaterialBase : public BSShaderMaterial
			{
			public:
				typedef NiTexture Texture;

				inline bool IsLightingShaderMaterialEnvmap() const { thisVirtualCall<void>(0x68, this); }
				inline void ClearTextures() const { thisVirtualCall<void>(0x70, this); }
				inline void GetTextures(Texture** Textures, uint32_t MaxSize) const { thisVirtualCall<void>(0x78, this, Textures, MaxSize); }
				inline void SaveToStream(NiStream* Stream) const { thisVirtualCall<void>(0x80, this, Stream); }
				inline void LoadFromStream(NiStream* Stream) const { thisVirtualCall<void>(0x88, this, Stream); }
				inline void PrefetchTextures() const { thisVirtualCall<void>(0x90, this); }
				inline void LoadTextureSet(BSShaderTextureSet* TextureSet) const { thisVirtualCall<void>(0xA0, this, TextureSet); }
				inline NiAPI::NiColor& SpecularColor() { return _SpecularColor; }
				inline NiAPI::NiPointer<Texture> GetDiffuseTexture() const { return _DiffuseTexture; }
				inline NiAPI::NiPointer<Texture> GetNormalTexture() const { return _NormalTexture; }
				inline NiAPI::NiPointer<Texture> GetRimSoftLightingTexture() const { return _RimSoftLightingTexture; }
				inline NiAPI::NiPointer<Texture> GetSmoothnessSpecMaskTexture() const { return _SmoothnessSpecMaskTexture; }
				inline NiAPI::NiPointer<Texture> GetLookupTexture() const { return _LookupTexture; }
				inline uint64_t& TextureClampMode() { return _TextureClampMode; }
				inline NiAPI::NiPointer<BSShaderTextureSet> GetTextureSet() const { return _TextureSet; }
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
				NiAPI::NiPointer<Texture>				_DiffuseTexture;			// 48
				NiAPI::NiPointer<Texture>				_NormalTexture;				// 50
				NiAPI::NiPointer<Texture>				_RimSoftLightingTexture;	// 58
				NiAPI::NiPointer<Texture>				_SmoothnessSpecMaskTexture;	// 60
				NiAPI::NiPointer<Texture>				_LookupTexture;				// 68
				uint64_t								_TextureClampMode;			// 70
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
				uint32_t								_Unk;						// BC
			};
			static_assert(sizeof(BSLightingShaderMaterialBase) == 0xC0);

			// C8
			class BSLightingShaderMaterialFace : public BSLightingShaderMaterialBase
			{
			public:
				inline NiAPI::NiPointer<Texture> GetBaseDiffuse() const { return _BaseDiffuse; }
			private:
				NiAPI::NiPointer<Texture>		_BaseDiffuse;				// C0
			};
			static_assert(sizeof(BSLightingShaderMaterialFace) == 0xC8);
		}
	}
}

#pragma pack(pop)