// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <EditorAPI/NiAPI/NiTypes.h>
#include "BGSSounds.h"

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

				// BGSPreloadable ddd added

				// size 0xC0
				// func 101
				class BGSImpactData : public TESForm
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

					inline const char* GetEffectImpactNifPath() const { return _EffectImpactNifPath; }
					inline float GetEffectDuration() const { return _EffectDuration; }
					inline void SetEffectDuration(float v) { _EffectDuration = v; }
					inline float GetAngleThreshold() const { return _AngleThreshold; }
					inline void SetAngleThreshold(float v) { _AngleThreshold = v; }
					inline float GetPlacementRadius() const { return _PlacementRadius; }
					inline void SetPlacementRadius(float v) { _PlacementRadius = v; }
					inline float GetDecalMinWidth() const { return _DecalMinWidth; }
					inline void SetDecalMinWidth(float v) { _DecalMinWidth = v; }
					inline float GetDecalMaxWidth() const { return _DecalMaxWidth; }
					inline void SetDecalMaxWidth(float v) { _DecalMaxWidth = v; }
					inline float GetDecalMinHeight() const { return _DecalMinHeight; }
					inline void SetDecalMinHeight(float v) { _DecalMinHeight = v; }
					inline float GetDecalMaxHeight() const { return _DecalMaxHeight; }
					inline void SetDecalMaxHeight(float v) { _DecalMaxHeight = v; }
					inline float GetDecalDepth() const { return _DecalDepth; }
					inline void SetDecalDepth(float v) { _DecalDepth = v; }
					inline float GetDecalShininess() const { return _DecalShininess; }
					inline void SetDecalShininess(float v) { _DecalShininess = v; }
					inline float GetDecalParallaxScale() const { return _DecalParallaxScale; }
					inline void SetDecalParallaxScale(float v) { _DecalParallaxScale = v; }
					inline OrientationT GetEffectOrientation() const { return _EffectOrientation; }
					inline void SetEffectOrientation(OrientationT v) { _EffectOrientation = v; }
					inline SoundLevelT GetEffectSoundLevel() const { return _EffectSoundLevel; }
					inline void SetEffectSoundLevel(SoundLevelT v) { _EffectSoundLevel = v; }
					inline ImpactResultT GetEffectImpactResult() const { return _EffectImpactResult; }
					inline void SetEffectImpactResult(ImpactResultT v) { _EffectImpactResult = v; }
					inline std::uint8_t GetDecalParallaxPasses() const { return _DecalParallaxPasses; }
					inline void SetDecalParallaxPasses(std::uint8_t v) { _DecalParallaxPasses = v; }
					inline BGSTextureSet* GetTextureSet() const { return _TextureSet; }
					inline void SetTextureSet(BGSTextureSet* v) { _TextureSet = v; }
					inline void CleanTextureSet() { _TextureSet = nullptr; }
					inline BGSTextureSet* GetSecondaryTextureSet() const { return _SecondaryTextureSet; }
					inline void SetSecondaryTextureSet(BGSTextureSet* v) { _SecondaryTextureSet = v; }
					inline void CleanSecondaryTextureSet() { _SecondaryTextureSet = nullptr; }
					inline BGSSoundDescriptorForm* GetImpactSound1() const { return _ImpactSound1; }
					inline void SetImpactSound1(BGSSoundDescriptorForm* v) { _ImpactSound1 = v; }
					inline void CleanImpactSound1() { _ImpactSound1 = nullptr; }
					inline BGSSoundDescriptorForm* GetImpactSound2() const { return _ImpactSound2; }
					inline void SetImpactSound2(BGSSoundDescriptorForm* v) { _ImpactSound2 = v; }
					inline void CleanImpactSound2() { _ImpactSound2 = nullptr; }
					inline BGSHazard* GetEffectHazard() const { return _EffectHazard; }
					inline void SetEffectHazard(BGSHazard* v) { _EffectHazard = v; }
					inline void CleanEffectHazard() { _EffectHazard = nullptr; }
					inline bool HasOwnDecalData() const { return !_HasOwnDecalData; }
					inline void SetOwnDecalData(bool v) { _HasOwnDecalData = (std::uint8_t)(!v); }
					inline bool HasParallax() const { return (_DecalFlags & dfParallax) == dfParallax; }
					inline bool HasBlending() const { return (_DecalFlags & dfBlending) == dfBlending; }
					inline bool HasTesting() const { return (_DecalFlags & dfTesting) == dfTesting; }
					inline bool HasNo4Subtextures() const { return (_DecalFlags & dfNo4Subtextures) == dfNo4Subtextures; }
					inline void SetParallax(bool v) { (_DecalFlags &= ~dfParallax); if (v) _DecalFlags |= dfParallax; }
					inline void SetBlending(bool v) { (_DecalFlags &= ~dfBlending); if (v) _DecalFlags |= dfBlending; }
					inline void SetTesting(bool v) { (_DecalFlags &= ~dfTesting); if (v) _DecalFlags |= dfTesting; }
					inline void SetNo4Subtextures(bool v) { (_DecalFlags &= ~dfNo4Subtextures); if (v) _DecalFlags |= dfNo4Subtextures; }
					inline NiAPI::NiRGB GetDecalColor() const { return _DecalColor; }
					inline void SetDecalColor(const NiAPI::NiRGB& v) { _DecalColor = v; }
					inline void SetDecalColorRGB(std::uint8_t r, std::uint8_t g, std::uint8_t b) { _DecalColor = { r, g, b }; }
				private:
					char pad28[0x8];
					const char* _EffectImpactNifPath;
					char pad38[0x20];
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
					std::uint8_t _DecalFlags;
					char padB6[0x2];
					NiAPI::NiRGB _DecalColor;
					char padBB[0x5];
				};
				static_assert(sizeof(BGSImpactData) == 0xC0);
			}
		}
	}
}