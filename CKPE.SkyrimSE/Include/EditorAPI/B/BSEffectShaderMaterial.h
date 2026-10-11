// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/BSShader/Shaders/BSEffectShaderMaterial.h

#pragma once

#include <CKPE.Enum.h>

#include <EditorAPI/N/NiPointer.h>
#include <EditorAPI/N/NiColor.h>
#include <EditorAPI/N/NiSourceTexture.h>
#include <EditorAPI/B/BSShaderMaterial.h>
#include <EditorAPI/T/TextureAddressModes.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			using namespace NiAPI;

			class BSEffectShaderMaterial : public BSShaderMaterial
			{
			public:
				virtual ~BSEffectShaderMaterial() = default;

				[[nodiscard]] Type GetType() const noexcept(true) override { return Type::kEffect; }

				[[nodiscard]] inline float GetFalloffStartAngle() const noexcept(true) { return fFalloffStartAngle; }
				[[nodiscard]] inline float GetFalloffStopAngle() const noexcept(true) { return fFalloffStartAngle; }
				[[nodiscard]] inline float GetFalloffStartOpacity() const noexcept(true) { return fFalloffStartOpacity; }
				[[nodiscard]] inline float GetFalloffStopOpacity() const noexcept(true) { return fFalloffStopOpacity; }
				[[nodiscard]] inline NiColorA GetBaseColor() const noexcept(true) { return kBaseColor; }
				[[nodiscard]] inline NiPointer<NiSourceTexture> GetTexture() const noexcept(true) { return spTexture; }
				[[nodiscard]] inline NiPointer<NiSourceTexture> GetGrayscaleTexture() const noexcept(true) { return spGrayscaleTexture; }
				[[nodiscard]] inline float GetSoftFalloffDepth() const noexcept(true) { return fSoftFalloffDepth; }
				[[nodiscard]] inline float GetBaseColorScale() const noexcept(true) { return fBaseColorScale; }
				[[nodiscard]] inline const char* GetTexturePath() const noexcept(true) { return sourceTexturePath; }
				[[nodiscard]] inline const char* GetGreyscaleTexturePath() const noexcept(true) { return greyscaleTexturePath; }

				inline void SetFalloffStartAngle(float a_value) noexcept(true) { fFalloffStartAngle = a_value; }
				inline void SetFalloffStopAngle(float a_value) noexcept(true) { fFalloffStopAngle = a_value; }
				inline void SetFalloffStartOpacity(float a_value) noexcept(true) { fFalloffStartOpacity = a_value; }
				inline void SetFalloffStopOpacity(float a_value) noexcept(true) { fFalloffStopOpacity = a_value; }
				inline void SetBaseColor(const NiColorA& a_color) noexcept(true) { kBaseColor = a_color; }
				inline void SetBaseColorAlpha(float a_value) noexcept(true) { kBaseColor.a = a_value; }
				inline void SetSoftFalloffDepth(float a_value) noexcept(true) { fSoftFalloffDepth = a_value; }
				inline void SetBaseColorScale(float a_value) noexcept(true) { fBaseColorScale = a_value; }

				[[nodiscard]] inline BSGraphics::TextureAddressMode GetClampMode() const noexcept(true) { return eEffectClampMode.get(); }
				[[nodiscard]] inline void SetClampMode(BSGraphics::TextureAddressMode a_mode) noexcept(true) { eEffectClampMode = a_mode; }

				CKPE_PROPERTY(GetFalloffStartAngle, SetFalloffStartAngle) float FalloffStartAngle;
				CKPE_PROPERTY(GetFalloffStopAngle, SetFalloffStopAngle) float FalloffStopAngle;
				CKPE_PROPERTY(GetFalloffStartOpacity, SetFalloffStartOpacity) float FalloffStartOpacity;
				CKPE_PROPERTY(GetFalloffStopOpacity, SetFalloffStopOpacity) float FalloffStopOpacity;
				CKPE_PROPERTY(GetBaseColor, SetBaseColor) NiColorA BaseColor;
				CKPE_PROPERTY(GetSoftFalloffDepth, SetSoftFalloffDepth) float SoftFalloffDepth;
				CKPE_PROPERTY(GetBaseColorScale, SetBaseColorScale) float BaseColorScale;
			private:
				float fFalloffStartAngle;
				float fFalloffStopAngle;
				float fFalloffStartOpacity;
				float fFalloffStopOpacity;
				NiColorA kBaseColor;
				NiPointer<NiSourceTexture> spTexture;
				NiPointer<NiSourceTexture> spGrayscaleTexture;
				float fSoftFalloffDepth;
				float fBaseColorScale;
				BSFixedString sourceTexturePath;
				BSFixedString greyscaleTexturePath;
				TEnum<BSGraphics::TextureAddressMode, std::uint8_t> eEffectClampMode;
				std::uint8_t eUnknown;
			};
			static_assert(sizeof(BSEffectShaderMaterial) == 0x88);
		}
	}
}