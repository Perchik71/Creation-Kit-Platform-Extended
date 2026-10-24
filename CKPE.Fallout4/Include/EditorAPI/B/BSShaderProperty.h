// Copyright © 2024 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/N/NiProperty.h>
#include <EditorAPI/N/NiPointer.h>
#include <EditorAPI/N/NiFlags.h>
#include <EditorAPI/B/BSFadeNode.h>
#include <EditorAPI/B/BSShaderMaterial.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/N/NiColor.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSRenderPass;
			class BSEffectShaderData;
			class BSShaderAccumulator;
			class BSGeometry;

			// 70
			class BSShaderProperty : public NiAPI::NiShadeProperty
			{
			public:
				enum class EShaderPropertyFlag : std::uint64_t
				{
					Specular = 1ULL << 0,
					Skinned = 1ULL << 1,
					TempRefraction = 1ULL << 2,
					VertexAlpha = 1ULL << 3,
					GrayscaleToPaletteColor = 1ULL << 4,
					GrayscaleToPaletteAlpha = 1ULL << 5,
					Falloff = 1ULL << 6,
					EnvMap = 1ULL << 7,
					RGBFalloff = 1ULL << 8,
					CastShadows = 1ULL << 9,
					Face = 1ULL << 10,
					UIMaskRects = 1ULL << 11,
					ModelSpaceNormals = 1ULL << 12,
					NonProjectiveShadows = 1ULL << 13,
					Landscape = 1ULL << 14,
					Refraction = 1ULL << 15,
					RefractionFalloff = 1ULL << 16,
					FireRefraction = RefractionFalloff,
					EyeEnvironmentMapping = 1ULL << 17,
					HairTint = 1ULL << 18,
					ScreendoorAlphaFade = 1ULL << 19,
					HideSecret = 1ULL << 20,
					FaceGenRGBTint = 1ULL << 21,
					OwnEmit = 1ULL << 22,
					ProjectedUV = 1ULL << 23,
					MultipleTextures = 1ULL << 24,
					Tessellate = 1ULL << 25,
					Decal = 1ULL << 26,
					DynamicDecal = 1ULL << 27,
					CharacterLighting = 1ULL << 28,
					ExternalEmittance = 1ULL << 29,
					SoftEffect = 1ULL << 30,
					ZBufferTest = 1ULL << 31,
					ZBufferWrite = 1ULL << 32,
					LODLandscape = 1ULL << 33,
					LODObjects = 1ULL << 34,
					NoFade = 1ULL << 35,
					TwoSided = 1ULL << 36,
					VertexColors = 1ULL << 37,
					GlowMap = 1ULL << 38,
					TransformChanged = 1ULL << 39,
					DismembermentMeatCuff = 1ULL << 40,
					Tint = 1ULL << 41,
					GrassVertexLighting = 1ULL << 42,
					GrassUniformScale = 1ULL << 43,
					GrassFitSlope = 1ULL << 44,
					GrassBillboard = 1ULL << 45,
					NoLODLandBlend = 1ULL << 46,
					Dismemberment = 1ULL << 47,
					Wireframe = 1ULL << 48,
					WeaponBlood = 1ULL << 49,
					HideOnLocalMap = 1ULL << 50,
					PremultAlpha = 1ULL << 51,
					VATSTarget = 1ULL << 52,
					AnisotropicLighting = 1ULL << 53,
					SkewSpecularAlpha = 1ULL << 54,
					MenuScreen = 1ULL << 55,
					MultiLayerParallax = 1ULL << 56,
					AlphaTest = 1ULL << 57,
					GradientRemap = 1ULL << 58,
					VATSTargetDrawAll = 1ULL << 59,
					PipboyScreen = 1ULL << 60,
					TreeAnim = 1ULL << 61,
					EffectLighting = 1ULL << 62,
					RefractionWritesDepth = 1ULL << 63
				};

				struct RenderPassArray
				{
					BSRenderPass* PassList;							// 00
				};
				static_assert(sizeof(RenderPassArray) == 0x8);
				class ForEachVisitor;

				inline std::uint64_t GetFlags() const noexcept(true) { return _Flags.Flags; }
				inline void SetFlags(std::uint64_t v) noexcept(true)
				{
					if (_Flags.Flags != v)
						_LastRenderPassState = 0x7FFFFFFF;
					_Flags.Flags = v;
				}
				inline void SetFlag(EShaderPropertyFlag Flag, bool Enabled) noexcept(true)
				{
					const auto Mask = static_cast<std::uint64_t>(Flag);
					SetFlags(Enabled ? GetFlags() | Mask : GetFlags() & ~Mask);
				}
				inline NiAPI::NiPointer<BSFadeNode> GetFadeNode() const noexcept(true)
				{ return NiAPI::NiPointer<BSFadeNode>(_FadeNode); }
				inline BSShaderMaterial* GetShaderMaterial() const noexcept(true) { return _ShaderMaterial; }
				inline float GetAlpha() const noexcept(true) { return _Alpha; }
				inline void SetMaterialAlpha(float Alpha) noexcept(true)
				{ CKPE::vtbl_call<void>(0x1B0, this, Alpha); }
				inline float GetMaterialAlpha() const noexcept(true)
				{ return CKPE::vtbl_call<float>(0x1B8, this); }
				inline RenderPassArray* GetRenderPasses(BSGeometry* Geometry,
					std::uint32_t RenderMode, BSShaderAccumulator* Accumulator) noexcept(true)
				{ return CKPE::vtbl_call<RenderPassArray*>(0x168, this, Geometry, RenderMode, Accumulator); }
				inline bool InitializeGeometry(BSGeometry* Geometry) noexcept(true)
				{ return CKPE::vtbl_call<bool>(0x160, this, Geometry); }
				inline bool CanMerge(const BSShaderProperty* Property) const noexcept(true)
				{ return CKPE::vtbl_call<bool>(0x198, this, Property); }
				inline std::uint64_t GetDifferenceMask(const BSShaderProperty* Property) const noexcept(true)
				{ return CKPE::vtbl_call<std::uint64_t>(0x1A0, this, Property); }
				inline bool DescribeDifferences(const BSShaderProperty* Property,
					std::uint64_t Differences, char* Buffer, std::uint32_t BufferSize) const noexcept(true)
				{ return CKPE::vtbl_call<bool>(0x1A8, this, Property, Differences, Buffer, BufferSize); }
				inline const BSFixedString& GetRootName() const noexcept(true)
				{ return CKPE::vtbl_call<const BSFixedString&>(0x1C0, this); }
				inline std::int32_t ForEachTexture(ForEachVisitor& Visitor) noexcept(true)
				{ return CKPE::vtbl_call<std::int32_t>(0x1C8, this, &Visitor); }
				inline std::int32_t GetShaderType() const noexcept(true)
				{ return CKPE::vtbl_call<std::int32_t>(0x1D0, this); }
				inline NiAPI::NiTexture* GetBaseTexture() const noexcept(true)
				{ return CKPE::vtbl_call<NiAPI::NiTexture*>(0x1E8, this); }
				inline void ClearRenderPasses() noexcept(true)
				{ CKPE::vtbl_call<void>(0x218, this); }
				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{
					return offsetof(BSShaderProperty, _Flags) == 0x30 &&
						offsetof(BSShaderProperty, _RenderPassList) == 0x38 &&
						offsetof(BSShaderProperty, _FadeNode) == 0x48 &&
						offsetof(BSShaderProperty, _ShaderMaterial) == 0x58 &&
						offsetof(BSShaderProperty, _ClearRenderPassesLock) == 0x68;
				}
			private:
				/* 0x168 */ virtual RenderPassArray* GetRenderPassesImpl(
					BSGeometry* Geometry, std::uint32_t RenderMode,
					BSShaderAccumulator* Accumulator);
				// Likely shadow passes; the base returns no passes.
				/* 0x170 */ virtual void sub_170();
				// Likely local-map passes; the base returns no passes.
				/* 0x178 */ virtual void sub_178();
				// Likely VATS mask passes; the base returns no passes.
				/* 0x180 */ virtual void sub_180();
				/* 0x188 */ virtual void sub_188();
				// Likely depth passes; the base returns no passes.
				/* 0x190 */ virtual void sub_190();
				/* 0x198 */ virtual bool CanMergeImpl(
					const BSShaderProperty* Property) const;
				/* 0x1A0 */ virtual std::uint64_t GetDifferenceMaskImpl(
					const BSShaderProperty* Property) const;
				/* 0x1A8 */ virtual bool DescribeDifferencesImpl(
					const BSShaderProperty* Property, std::uint64_t Differences,
					char* Buffer, std::uint32_t BufferSize) const;
				/* 0x1B0 */ virtual void SetMaterialAlphaImpl(float Alpha);
				/* 0x1B8 */ virtual float GetMaterialAlphaImpl() const;
				/* 0x1C0 */ virtual const BSFixedString& GetRootNameImpl() const;
				/* 0x1C8 */ virtual std::int32_t ForEachTextureImpl(
					ForEachVisitor& Visitor);
				/* 0x1D0 */ virtual std::int32_t GetShaderTypeImpl() const;
				/* 0x1D8 */ virtual void sub_1D8();
				/* 0x1E0 */ virtual void sub_1E0();
				/* 0x1E8 */ virtual NiAPI::NiTexture* GetBaseTextureImpl() const;
				/* 0x1F0 */ virtual void sub_1F0();
				/* 0x1F8 */ virtual void sub_1F8();
				/* 0x200 */ virtual void sub_200();
				/* 0x208 */ virtual void sub_208();
				/* 0x210 */ virtual void sub_210();
				/* 0x218 */ virtual void ClearRenderPassesImpl();

				float												_Alpha;				// 28
				std::int32_t										_LastRenderPassState;	// 2C
				NiAPI::NiTFlags<std::uint64_t, BSShaderProperty>	_Flags;				// 30
				RenderPassArray									_RenderPassList;	// 38
				RenderPassArray									_DebugRenderPassList;	// 40
				BSFadeNode*										_FadeNode;			// 48
				BSEffectShaderData*								_Effects;			// 50
				BSShaderMaterial*									_ShaderMaterial;	// 58
				std::uint32_t										_LastAccumTime;		// 60
				float												_LODFade;			// 64
				BSNonReentrantSpinLock							_ClearRenderPassesLock;	// 68
				std::uint32_t									_Pad6C;				// 6C
			};
			static_assert(sizeof(BSShaderProperty) == 0x70);
			static_assert(BSShaderProperty::VerifyLayout());

			class BSWaterShaderProperty : public BSShaderProperty
			{
			public:
				std::uint32_t WaterFlags;							// 70
				std::uint32_t Unk74;								// 74
				char Unk78[0x30];									// 78
				BSTArray<void*> UnkA8;								// A8
			};
			static_assert(sizeof(BSWaterShaderProperty) == 0xC0);
			static_assert(offsetof(BSWaterShaderProperty, WaterFlags) == 0x70);
			static_assert(offsetof(BSWaterShaderProperty, UnkA8) == 0xA8);

			class BSSkyShaderProperty : public BSShaderProperty
			{
			public:
				NiAPI::NiColorA Unk70;								// 70
				NiAPI::NiPointer<NiAPI::NiTexture> Textures[3];		// 80
				BSFixedString Unk98;								// 98
				std::uint32_t UnkA0;								// A0
				std::uint16_t UnkA4;								// A4
				std::uint16_t PadA6;								// A6
				std::uint32_t UnkA8;								// A8
				std::uint32_t PadAC;								// AC
			};
			static_assert(sizeof(BSSkyShaderProperty) == 0xB0);
			static_assert(offsetof(BSSkyShaderProperty, Textures) == 0x80);
		}
	}
}
