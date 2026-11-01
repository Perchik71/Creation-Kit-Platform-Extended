// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/N/NiTypes.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/N/NiPointer.h>
#include <EditorAPI/N/NiTexture.h>
#include <EditorAPI/B/BSTextureSet.h>
#include <EditorAPI/B/BSLock.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSShaderProperty;

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

				constexpr static Common::ID VTABLE = IDs::VTABLE::BSShaderMaterial[0];

				inline BSShaderMaterial* Create() const noexcept(true) { return CKPE::vtbl_call<BSShaderMaterial*>(0x8, this); }
				inline std::uint32_t GetType() const noexcept(true) { return CKPE::vtbl_call<std::uint32_t>(0x28, this); }
				inline std::uint32_t GetFeature() const noexcept(true) { return GetType(); }
				inline std::uint32_t GetMaterialType() const noexcept(true)
				{ return CKPE::vtbl_call<std::uint32_t>(0x30, this); }
				inline NiAPI::NiPoint2 GetTextCoordOffset() const noexcept(true) { return _TextCoordOffset[0]; }
				inline NiAPI::NiPoint2 GetTextCoordScale() const noexcept(true) { return _TextCoordScale[0]; }
				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{
					return offsetof(BSShaderMaterial, _RefCount) == 0x8 &&
						offsetof(BSShaderMaterial, _TextCoordOffset) == 0xC &&
						offsetof(BSShaderMaterial, _TextCoordScale) == 0x1C;
				}

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
				/* 0x008 */ virtual BSShaderMaterial* CreateImpl() const;
				/* 0x010 */ virtual void CopyMembers(const BSShaderMaterial* Material);
				/* 0x018 */ virtual std::uint32_t ComputeCRC32(
					std::uint32_t Seed, bool Combining) const;
				// Likely default material; returns a shared material pointer.
				/* 0x020 */ virtual void GetDefault(); // returns the static default material; name by CommonLibF4 slot order and the body, signature not recovered
				/* 0x028 */ virtual std::uint32_t GetTypeImpl() const;
				/* 0x030 */ virtual std::uint32_t GetMaterialTypeImpl() const;
				/* 0x038 */ virtual void ReceiveValuesFromRootMaterial(
					BSShaderProperty* Property);
				/* 0x040 */ virtual std::uint64_t GetDifferenceMask(
					const BSShaderMaterial* Material) const;
				/* 0x048 */ virtual bool DescribeDifferences(
					const BSShaderMaterial* Material, std::uint64_t Differences,
					char* Buffer, std::uint32_t BufferSize) const;
				// Likely material viewer; formats UV and reference-count state.
				/* 0x050 */ virtual void GetDebugString(); // builds the "BSShaderMaterial: ... TCOffset ... TCScale" text; signature not recovered
				// Compares the material's values with Other (floats within 0.001); only the effect and water materials do it
				/* 0x058 */ [[nodiscard]] virtual bool Equals(const BSShaderMaterial* Other) const;

				volatile std::uint32_t	_RefCount;				// 08
				NiAPI::NiPoint2			_TextCoordOffset[2];	// 0C
				NiAPI::NiPoint2			_TextCoordScale[2];		// 1C
				std::uint32_t			_Unk2C;					// 2C
				std::uint32_t			_Unk30;					// 30
				char					_Pad34[4];				// 34
			};
			static_assert(sizeof(BSShaderMaterial) == 0x38);
			static_assert(BSShaderMaterial::VerifyLayout());

			// C0
			class BSLightingShaderMaterialBase : public BSShaderMaterial
			{
			public:
				virtual ~BSLightingShaderMaterialBase() = default;

				inline bool IsLightingShaderMaterialEnvmap() const noexcept(true) { return CKPE::vtbl_call<BSLightingShaderMaterialBase*>(0x60, this) != nullptr; }
				inline void ClearTextures() const noexcept(true) { CKPE::vtbl_call<void>(0x70, this); }
				inline std::uint32_t GetTextures(NiAPI::NiTexture** Textures, std::uint32_t MaxSize) const noexcept(true)
				{ return CKPE::vtbl_call<std::uint32_t>(0x78, this, Textures, MaxSize); }
				inline void SaveToStream(void* Stream) const noexcept(true) { CKPE::vtbl_call<void>(0x80, this, Stream); }
				inline void LoadFromStream(void* Stream) const noexcept(true) { CKPE::vtbl_call<void>(0x88, this, Stream); }
				inline void PrefetchTextures(void* OutHandles, const BSTextureSet* TextureSet) const noexcept(true)
				{ CKPE::vtbl_call<void>(0x90, this, OutHandles, TextureSet); }
				inline void LoadTextureSet(BSShaderTextureSet* TextureSet) const noexcept(true) { CKPE::vtbl_call<void>(0xA0, this, TextureSet); }
				inline NiAPI::NiColor& SpecularColor() { return _SpecularColor; }
				inline NiAPI::NiPointer<NiAPI::NiTexture> GetDiffuseTexture() const noexcept(true) { return _DiffuseTexture; }
				inline NiAPI::NiPointer<NiAPI::NiTexture> GetNormalTexture() const noexcept(true) { return _NormalTexture; }
				inline NiAPI::NiPointer<NiAPI::NiTexture> GetRimSoftLightingTexture() const noexcept(true) { return _RimSoftLightingTexture; }
				inline NiAPI::NiPointer<NiAPI::NiTexture> GetSmoothnessSpecMaskTexture() const noexcept(true) { return _SmoothnessSpecMaskTexture; }
				inline NiAPI::NiPointer<NiAPI::NiTexture> GetLookupTexture() const noexcept(true) { return _LookupTexture; }
				inline std::uint32_t& TextureClampMode() noexcept(true) { return _TextureClampMode; }
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
				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{
					return offsetof(BSLightingShaderMaterialBase, _DiffuseTexture) == 0x48 &&
						offsetof(BSLightingShaderMaterialBase, _TextureClampMode) == 0x70 &&
						offsetof(BSLightingShaderMaterialBase, _TextureSet) == 0x78 &&
						offsetof(BSLightingShaderMaterialBase, _MaterialAlpha) == 0x80 &&
						offsetof(BSLightingShaderMaterialBase, _LookupScale) == 0xB8;
				}
			protected:
				/* 0x060 */ virtual BSLightingShaderMaterialBase* AsEnvmap();
				/* 0x068 */ virtual void sub_068();
				/* 0x070 */ virtual void ClearTexturesImpl();
				/* 0x078 */ virtual std::uint32_t GetTexturesImpl(
					NiAPI::NiTexture** Textures, std::uint32_t MaxSize);
				/* 0x080 */ virtual void SaveToStreamImpl(void* Stream);
				/* 0x088 */ virtual void LoadFromStreamImpl(void* Stream);
				/* 0x090 */ virtual void PrefetchTexturesImpl(
					void* OutHandles, const BSTextureSet* TextureSet);
				// Likely texture-set load; also consumes prefetched handles.
				/* 0x098 */ virtual void sub_098();
				/* 0x0A0 */ virtual void LoadTextureSetImpl(BSShaderTextureSet* TextureSet);
				// Likely root-material copy; reads the source material's values.
				/* 0x0A8 */ virtual void sub_0A8();

				NiAPI::NiColor							_SpecularColor;				// 38
				char									_Pad44[4];					// 44
				NiAPI::NiPointer<NiAPI::NiTexture>		_DiffuseTexture;			// 48
				NiAPI::NiPointer<NiAPI::NiTexture>		_NormalTexture;				// 50
				NiAPI::NiPointer<NiAPI::NiTexture>		_RimSoftLightingTexture;	// 58
				NiAPI::NiPointer<NiAPI::NiTexture>		_SmoothnessSpecMaskTexture;	// 60
				NiAPI::NiPointer<NiAPI::NiTexture>		_LookupTexture;				// 68
				std::uint32_t							_TextureClampMode;			// 70
				std::uint32_t							_Pad74;						// 74
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
				BSNonReentrantSpinLock					_LoadTextureSetLock;		// BC
			};
			static_assert(sizeof(BSLightingShaderMaterialBase) == 0xC0);
			static_assert(BSLightingShaderMaterialBase::VerifyLayout());

			// C8
			class BSLightingShaderMaterialFace : public BSLightingShaderMaterialBase
			{
			public:
				inline NiAPI::NiPointer<NiAPI::NiTexture> GetBaseDiffuse() const { return _BaseDiffuse; }
				inline NiAPI::NiPointer<NiAPI::NiTexture> GetWrinkleTexture() const { return _BaseDiffuse; }
				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{ return offsetof(BSLightingShaderMaterialFace, _BaseDiffuse) == 0xC0; }
			private:
				NiAPI::NiPointer<NiAPI::NiTexture>	_BaseDiffuse;				// C0
			};
			static_assert(sizeof(BSLightingShaderMaterialFace) == 0xC8);
			static_assert(BSLightingShaderMaterialFace::VerifyLayout());
		}
	}
}