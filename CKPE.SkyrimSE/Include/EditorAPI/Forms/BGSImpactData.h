// Copyright © 2023-2024 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <CKPE.EnumSet.h>
#include <EditorAPI/NiAPI/NiTypes.h>
#include <EditorAPI/Forms/BGSSounds.h>
#include <EditorAPI/TESModel.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSHazard;
				class BGSTextureSet;

				class BGSImpactData :
					public TESForm,
					public TESModel
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftImpactData;

					enum OrientationT : std::uint32_t
					{
						eoSurfaceNormal = 0,
						eoProjectileVector,
						eoProjectileReflection,
					};

					enum SoundLevelT : std::uint32_t
					{
						slLoud = 0,
						slNormal,
						slSilent,
						slVeryLoud,
					};

					enum ImpactResultT : std::uint8_t
					{
						irDefault = 0,
						irDestroy,
						irBounce,
						irImpale,
						irStick,
					};

					enum DecalFlagsT : std::uint8_t
					{
						dfParallax = 1,
						dfBlending = 2,
						dfTesting = 4,
						dfNo4Subtextures = 8,
					};
				public:
					virtual ~BGSImpactData() = default;

					[[nodiscard]] inline float GetEffectDuration() const noexcept(true) { return _EffectDuration; }
					inline void SetEffectDuration(float v) noexcept(true) { _EffectDuration = v; }
					[[nodiscard]] inline float GetAngleThreshold() const noexcept(true) { return _AngleThreshold; }
					inline void SetAngleThreshold(float v) noexcept(true) { _AngleThreshold = v; }
					[[nodiscard]] inline float GetPlacementRadius() const noexcept(true) { return _PlacementRadius; }
					inline void SetPlacementRadius(float v) noexcept(true) { _PlacementRadius = v; }
					[[nodiscard]] inline float GetDecalMinWidth() const noexcept(true) { return _DecalMinWidth; }
					inline void SetDecalMinWidth(float v) noexcept(true) { _DecalMinWidth = v; }
					[[nodiscard]] inline float GetDecalMaxWidth() const noexcept(true) { return _DecalMaxWidth; }
					inline void SetDecalMaxWidth(float v) noexcept(true) { _DecalMaxWidth = v; }
					[[nodiscard]] inline float GetDecalMinHeight() const noexcept(true) { return _DecalMinHeight; }
					inline void SetDecalMinHeight(float v) noexcept(true) { _DecalMinHeight = v; }
					[[nodiscard]] inline float GetDecalMaxHeight() const noexcept(true) { return _DecalMaxHeight; }
					inline void SetDecalMaxHeight(float v) noexcept(true) { _DecalMaxHeight = v; }
					[[nodiscard]] inline float GetDecalDepth() const noexcept(true) { return _DecalDepth; }
					inline void SetDecalDepth(float v) noexcept(true) { _DecalDepth = v; }
					[[nodiscard]] inline float GetDecalShininess() const noexcept(true) { return _DecalShininess; }
					inline void SetDecalShininess(float v) noexcept(true) { _DecalShininess = v; }
					[[nodiscard]] inline float GetDecalParallaxScale() const noexcept(true) { return _DecalParallaxScale; }
					inline void SetDecalParallaxScale(float v) noexcept(true) { _DecalParallaxScale = v; }
					[[nodiscard]] inline OrientationT GetEffectOrientation() const noexcept(true) { return _EffectOrientation; }
					inline void SetEffectOrientation(OrientationT v) noexcept(true) { _EffectOrientation = v; }
					[[nodiscard]] inline SoundLevelT GetEffectSoundLevel() const noexcept(true) { return _EffectSoundLevel; }
					inline void SetEffectSoundLevel(SoundLevelT v) noexcept(true) { _EffectSoundLevel = v; }
					[[nodiscard]] inline ImpactResultT GetEffectImpactResult() const noexcept(true) { return _EffectImpactResult; }
					inline void SetEffectImpactResult(ImpactResultT v) noexcept(true) { _EffectImpactResult = v; }
					[[nodiscard]] inline std::uint8_t GetDecalParallaxPasses() const noexcept(true) { return _DecalParallaxPasses; }
					inline void SetDecalParallaxPasses(std::uint8_t v) noexcept(true) { _DecalParallaxPasses = v; }
					[[nodiscard]] inline BGSTextureSet* GetTextureSet() const noexcept(true) { return _TextureSet; }
					inline void SetTextureSet(BGSTextureSet* v) noexcept(true) { _TextureSet = v; }
					inline void CleanTextureSet() noexcept(true) { _TextureSet = nullptr; }
					[[nodiscard]] inline BGSTextureSet* GetSecondaryTextureSet() const noexcept(true) { return _SecondaryTextureSet; }
					inline void SetSecondaryTextureSet(BGSTextureSet* v) noexcept(true) { _SecondaryTextureSet = v; }
					inline void CleanSecondaryTextureSet() noexcept(true) { _SecondaryTextureSet = nullptr; }
					[[nodiscard]] inline BGSSoundDescriptorForm* GetImpactSound1() const noexcept(true) { return _ImpactSound1; }
					inline void SetImpactSound1(BGSSoundDescriptorForm* v) noexcept(true) { _ImpactSound1 = v; }
					inline void CleanImpactSound1() noexcept(true) { _ImpactSound1 = nullptr; }
					[[nodiscard]] inline BGSSoundDescriptorForm* GetImpactSound2() const noexcept(true) { return _ImpactSound2; }
					inline void SetImpactSound2(BGSSoundDescriptorForm* v) noexcept(true) { _ImpactSound2 = v; }
					inline void CleanImpactSound2() noexcept(true) { _ImpactSound2 = nullptr; }
					[[nodiscard]] inline BGSHazard* GetEffectHazard() const noexcept(true) { return _EffectHazard; }
					inline void SetEffectHazard(BGSHazard* v) noexcept(true) { _EffectHazard = v; }
					inline void CleanEffectHazard() noexcept(true) { _EffectHazard = nullptr; }
					[[nodiscard]] inline bool HasOwnDecalData() const noexcept(true) { return !_HasOwnDecalData; }
					inline void SetOwnDecalData(bool v) noexcept(true) { _HasOwnDecalData = (std::uint8_t)(!v); }
					[[nodiscard]] inline bool HasParallax() const noexcept(true) { return _DecalFlags.all(dfParallax); }
					[[nodiscard]] inline bool HasBlending() const noexcept(true) { return _DecalFlags.all(dfBlending); }
					[[nodiscard]] inline bool HasTesting() const noexcept(true) { return _DecalFlags.all(dfTesting); }
					[[nodiscard]] inline bool HasNo4Subtextures() const noexcept(true) { return _DecalFlags.all(dfNo4Subtextures); }
					inline void SetParallax(bool v) noexcept(true) { if (v) _DecalFlags.set(dfParallax); }
					inline void SetBlending(bool v) noexcept(true) { if (v) _DecalFlags.set(dfBlending); }
					inline void SetTesting(bool v) noexcept(true) { if (v) _DecalFlags.set(dfTesting); }
					inline void SetNo4Subtextures(bool v) noexcept(true) { if (v) _DecalFlags.set(dfNo4Subtextures); }
					[[nodiscard]] inline NiAPI::NiRGB GetDecalColor() const noexcept(true) { return _DecalColor; }
					inline void SetDecalColor(const NiAPI::NiRGB& v) noexcept(true) { _DecalColor = v; }
					inline void SetDecalColorRGB(std::uint8_t r, std::uint8_t g, std::uint8_t b) noexcept(true) { _DecalColor = { r, g, b }; }

					CKPE_PROPERTY(GetEffectDuration, SetEffectDuration) float EffectDuration;
					CKPE_PROPERTY(GetAngleThreshold, SetAngleThreshold) float AngleThreshold;
					CKPE_PROPERTY(GetPlacementRadius, SetPlacementRadius) float PlacementRadius;
					CKPE_PROPERTY(GetDecalMinWidth, SetDecalMinWidth) float DecalMinWidth;
					CKPE_PROPERTY(GetDecalMaxWidth, SetDecalMaxWidth) float DecalMaxWidth;
					CKPE_PROPERTY(GetDecalMinHeight, SetDecalMinHeight) float DecalMinHeight;
					CKPE_PROPERTY(GetDecalMaxHeight, SetDecalMaxHeight) float DecalMaxHeight;
					CKPE_PROPERTY(GetDecalDepth, SetDecalDepth) float DecalDepth;
					CKPE_PROPERTY(GetDecalShininess, SetDecalShininess) float DecalShininess;
					CKPE_PROPERTY(GetDecalParallaxScale, SetDecalParallaxScale) float DecalParallaxScale;
					CKPE_PROPERTY(GetEffectOrientation, SetEffectOrientation) OrientationT EffectOrientation;
					CKPE_PROPERTY(GetEffectSoundLevel, SetEffectSoundLevel) SoundLevelT EffectSoundLevel;
					CKPE_PROPERTY(GetEffectImpactResult, SetEffectImpactResult) ImpactResultT EffectImpactResult;
					CKPE_PROPERTY(GetDecalParallaxPasses, SetDecalParallaxPasses) std::uint8_t DecalParallaxPasses;
					CKPE_PROPERTY(GetTextureSet, SetTextureSet) BGSTextureSet* TextureSet;
				private:
					float _EffectDuration;
					OrientationT _EffectOrientation;
					float _AngleThreshold;
					float _PlacementRadius;
					SoundLevelT _EffectSoundLevel;
					std::uint8_t _HasOwnDecalData;					// 1 - false, 0 - true (◕‿◕)
					ImpactResultT _EffectImpactResult;
					char pad6E[0x2];
					BGSTextureSet* _TextureSet;
					BGSTextureSet* _SecondaryTextureSet;
					BGSSoundDescriptorForm* _ImpactSound1;
					BGSSoundDescriptorForm* _ImpactSound2;
					BGSHazard* _EffectHazard;
					float _DecalMinWidth;
					float _DecalMaxWidth;
					float _DecalMinHeight;
					float _DecalMaxHeight;
					float _DecalDepth;
					float _DecalShininess;
					float _DecalParallaxScale;
					std::uint8_t _DecalParallaxPasses;
					TEnumSet<DecalFlagsT, std::uint8_t> _DecalFlags;
					char padB6[0x2];
					NiAPI::NiRGB _DecalColor;
					char padBB[0x3];
				};
				static_assert(sizeof(BGSImpactData) == 0xC0);
			}
		}
	}
}