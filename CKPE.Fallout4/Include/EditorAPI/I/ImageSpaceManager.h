// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/IDs.h>
#include <CKPE.Common.Relocation.h>
#include <EditorAPI/I/ImageSpaceEffect.h>
#include <EditorAPI/N/NiColor.h>
#include <EditorAPI/N/NiPointer.h>
#include <EditorAPI/N/NiTypes.h>

namespace CKPE::Fallout4::EditorAPI
{
	class BSTriShape;
	namespace Forms { struct ImageSpaceBaseData; }

	class ImageSpaceManager
	{
	public:
		// Indices are 982+; GetEffect translates the older shader indices.
		enum class Effect : std::uint32_t
		{
			Sunbeams = 2,
			HDR = 3,
			HDRCS = 4,
			Refraction = 5,
			DepthOfField = 6,
			DepthOfFieldSplitScreen = 7,
			RadialBlur = 8,
			FullScreenBlur = 9,
			MotionBlur = 10,
			GetHit = 11,
			VatsTarget = 12,
			FullScreenColor = 13,
			GammaCorrectLUT = 15,
			FXAA = 17,
			TemporalAA = 18,
			BokehDepthOfField = 20,
			UpsampleDynamicResolution = 21,
			Map = 22,
			WaterDisplacement = 61,
			Noise = 62,
			RainSplash = 63,
			VLSLight = 64,
			VLS = 65,
			PipboyScreen = 66,
			HUDGlass = 67,
			ModMenu = 68,
			AmbientOcclusion = 69,
			ScalableAmbientObscurance = 70,
			ScalableAmbientObscuranceCS = 71,
			ShaderCopy = 72,
			ShaderCopyScaleBias = 73,
			ShaderCopyVisAlpha = 74,
			ShaderGreyScale = 75,
			ShaderDownsampleDepth = 76,
			ShaderCopyStencil = 77,
			ShaderCopyWaterMask = 78,
			ShaderCopyShadowMapToArray = 79,
			ShaderCopyNormals = 80,
			ShaderRefraction = 81,
			ShaderDoubleVision = 82,
			ShaderTextureMask = 83,
			ShaderMap = 84,
			ShaderDepthOfField = 87,
			ShaderDepthOfFieldFogged = 88,
			ShaderDepthOfFieldSplitScreen = 89,
			ShaderBokehDepthOfFieldPass1 = 90,
			ShaderBokehDepthOfFieldPass2 = 91,
			ShaderBokehDepthOfFieldPass3 = 92,
			ShaderBokehDepthOfFieldPass4 = 93,
			ShaderBokehDepthOfFieldPass4Fogged = 94,
			ShaderDistantBlur = 95,
			ShaderDistantBlurFogged = 96,
			ShaderRadialBlur = 97,
			ShaderRadialBlurMedium = 98,
			ShaderRadialBlurHigh = 99,
			ShaderHDRTonemapBlendCinematic = 100,
			ShaderHDRTonemapBlendCinematicFade = 101,
			ShaderHDRDownSample16 = 102,
			ShaderHDRDownSample4 = 103,
			ShaderHDRDownSample16Lum = 104,
			ShaderHDRDownSample4RGB2Lum = 105,
			ShaderHDRDownSample4LumClamp = 106,
			ShaderHDRDownSample4LightAdapt = 107,
			ShaderHDRDownSample16LumClamp = 108,
			ShaderHDRDownSample16LightAdapt = 109,
			ShaderHDRDownSample4CS = 110,
			ShaderHDRDownSample64RGB2LumCS = 111,
			ShaderHDRDownSample4LumCS = 112,
			ShaderHDRDownSample16LumCS = 113,
			ShaderHDRDownSample2LightAdaptCS = 114,
			ShaderBlur3 = 115,
			ShaderBlur5 = 116,
			ShaderBlur7 = 117,
			ShaderBlur9 = 118,
			ShaderBlur11 = 119,
			ShaderBlur13 = 120,
			ShaderBlur15 = 121,
			ShaderHDRBlurX15_320CS = 122,
			ShaderHDRBlurX15_480CS = 123,
			ShaderHDRBlurX15_1024CS = 124,
			ShaderNonHDRBlur3 = 125,
			ShaderNonHDRBlur5 = 126,
			ShaderNonHDRBlur7 = 127,
			ShaderNonHDRBlur9 = 128,
			ShaderNonHDRBlur11 = 129,
			ShaderNonHDRBlur13 = 130,
			ShaderNonHDRBlur15 = 131,
			ShaderBrightPassBlur3 = 132,
			ShaderBrightPassBlur5 = 133,
			ShaderBrightPassBlur7 = 134,
			ShaderBrightPassBlur9 = 135,
			ShaderBrightPassBlur11 = 136,
			ShaderBrightPassBlur13 = 137,
			ShaderBrightPassBlur15 = 138,
			ShaderBrightPassHDRBlurY15_180CS = 139,
			ShaderBrightPassHDRBlurY15_270CS = 140,
			ShaderBrightPassHDRBlurY15_1024CS = 141,
			ShaderWaterDisplacementClearSimulation = 142,
			ShaderWaterDisplacementTexOffset = 143,
			ShaderWaterDisplacementWadingRipple = 144,
			ShaderWaterDisplacementRainRipple = 145,
			ShaderWaterWadingHeightmap = 146,
			ShaderWaterRainHeightmap = 147,
			ShaderWaterBlendHeightmaps = 148,
			ShaderWaterSmoothHeightmap = 149,
			ShaderWaterDisplacementNormals = 150,
			ShaderNoiseScrollAndBlend = 151,
			ShaderNoiseNormalmap = 152,
			ShaderLocalMap = 153,
			ShaderLocalMapCompanion = 154,
			ShaderAlphaBlend = 155,
			ShaderPipboyScreen = 156,
			ShaderHUDGlass = 157,
			ShaderHUDGlassDropShadow = 158,
			ShaderHUDGlassBlurY = 159,
			ShaderHUDGlassBlurX = 160,
			ShaderHUDGlassMarkers = 161,
			ShaderVatsTargetDebug = 162,
			ShaderVatsTarget = 163,
			ShaderModMenuEffect = 164,
			ShaderModMenuGlowComposite = 165,
			ShaderAmbientOcclusion = 166,
			ShaderAmbientOcclusionBlur = 167,
			ShaderVLSSpotLight = 168,
			ShaderVLSApplication = 169,
			ShaderVLSComposite = 170,
			ShaderVLSSliceCoord = 171,
			ShaderVLSSliceInterp = 172,
			ShaderVLSSliceStencil = 173,
			ShaderVLSSliceScatterRay = 174,
			ShaderVLSSliceScatterInterp = 175,
			ShaderVLSScatterAccum = 176,
			ShaderSAOCameraZ = 177,
			ShaderSAOMinify = 178,
			ShaderSAORawAO = 179,
			ShaderSAOBlurH = 180,
			ShaderSAOBlurV = 181,
			ShaderSAORawAOEditor = 182,
			ShaderSAOCameraZCS = 183,
			ShaderSAOMinifyCS = 184,
			ShaderSAOCameraAndMipsZCS = 185,
			ShaderSAOMipsZCS = 186,
			ShaderSAORawAOCS = 187,
			ShaderSAOBlurHCS = 188,
			ShaderSAOBlurVCS = 189,
			ShaderMotionBlur = 190,
			ShaderTemporalAA = 191,
			ShaderTemporalAAPipboy = 192,
			ShaderTemporalAAPowerArmorPipboy = 193,
			ShaderGammaLinearize = 194,
			ShaderSunbeams = 195,
			ShaderSSLRPrepass = 196,
			ShaderSSLRRaytracing = 197,
			ShaderSSLRBlurH = 198,
			ShaderSSLRBlurV = 199,
			ShaderLensFlare = 200,
			ShaderRainSplash = 201,
			ShaderRainSplashUpdate = 202,
			ShaderRainSplashDraw = 203,
			ShaderLensFlareVisibility = 204,
			ShaderUpsampleDynamicResolution = 205,
			ShaderFullScreenColor = 206,
			ShaderHUDGlassClear = 207,
			ShaderHUDGlassCopy = 208,
			ShaderBlurY3_270CS = 209,
			ShaderBlurY5_270CS = 210,
			ShaderBlurY7_270CS = 211,
			ShaderBlurY9_270CS = 212,
			ShaderBlurY11_270CS = 213,
			ShaderBlurY13_270CS = 214,
			ShaderBlurY15_270CS = 215,
			ShaderBlurX3_480CS = 217,
			ShaderBlurX5_480CS = 218,
			ShaderBlurX7_480CS = 219,
			ShaderBlurX9_480CS = 220,
			ShaderBlurX11_480CS = 221,
			ShaderBlurX13_480CS = 222,
			ShaderBlurX15_480CS = 223,
			ShaderBrightPassBlurY3_270CS = 225,
			ShaderBrightPassBlurY5_270CS = 226,
			ShaderBrightPassBlurY7_270CS = 227,
			ShaderBrightPassBlurY9_270CS = 228,
			ShaderBrightPassBlurY11_270CS = 229,
			ShaderBrightPassBlurY13_270CS = 230,
			ShaderBrightPassBlurY15_270CS = 231,
			Total = 233  // Modern array capacity includes reserved indices.
		};

		ImageSpaceManager() = delete;
		~ImageSpaceManager();

		[[nodiscard]] static ImageSpaceManager* GetSingleton() noexcept(true)
		{
			static Common::Relocation<ImageSpaceManager**> instance{
				EditorAPI::IDs::ImageSpaceManager::Singleton };
			return *instance;
		}

		[[nodiscard]] ImageSpaceEffect* GetEffect(Effect Index) const noexcept(true)
		{
			auto index = static_cast<std::uint32_t>(Index);
			if (VersionLists::GetEditorVersion() ==
				VersionLists::EDITOR_FALLOUT_C4_1_10_162_0 && index >= 80)
			{
				if (Index == Effect::ShaderCopyNormals)
					return nullptr;
				--index;
			}
			return index < effectList.EmptyRunStart ? effectList.Data[index] : nullptr;
		}

		/* 0x000 */ NiAPI::NiRect<std::int32_t> scissorRect;
		/* 0x010 */ NiAPI::NiTObjectArray<ImageSpaceEffect*> effectList;
		/* 0x028 */ NiAPI::NiPointer<BSTriShape> screenTriShape;
		/* 0x030 */ NiAPI::NiPointer<BSTriShape> screenTriShapeColors;
		/* 0x038 */ NiAPI::NiPointer<BSTriShape> dynamicScreenTriShape;
		/* 0x040 */ NiAPI::NiPointer<BSTriShape> partialScreenTriShape;
		/* 0x048 */ bool enablePartialRender;
		/* 0x049 */ std::byte _pad049[3];
		/* 0x04C */ std::int32_t mainTarget;
		/* 0x050 */ NiAPI::NiColorA refractionTint;
		/* 0x060 */ const Forms::ImageSpaceBaseData* baseData;
		/* 0x068 */ const Forms::ImageSpaceBaseData* overrideBaseData;
		/* 0x070 */ const Forms::ImageSpaceBaseData* underwaterBaseData;
		/* 0x078 */ Forms::ImageSpaceBaseData* consoleBaseData;
		/* 0x080 */ std::byte _currentEOFData[0xB0];
		/* 0x130 */ std::byte _lutData[0x70];
		/* 0x1A0 */ void* overrideLUTData;
		/* 0x1A8 */ float motionBlurIntensity;
		/* 0x1AC */ float motionBlurMaxBlur;
		/* 0x1B0 */ std::uint32_t forceNoBokehDepthOfField;
		/* 0x1B4 */ bool isReady;
		/* 0x1B5 */ bool saveTarget;
		/* 0x1B6 */ bool useBokehDepthOfField;
		/* 0x1B7 */ std::byte _pad1B7;
	};
	static_assert(sizeof(ImageSpaceManager) == 0x1B8);
	static_assert(offsetof(ImageSpaceManager, effectList) == 0x10);
	static_assert(offsetof(ImageSpaceManager, baseData) == 0x60);
	static_assert(offsetof(ImageSpaceManager, isReady) == 0x1B4);
}
