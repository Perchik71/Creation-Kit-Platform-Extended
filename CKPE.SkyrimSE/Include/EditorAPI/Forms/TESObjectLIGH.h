// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/NiAPI/NiTypes.h>
#include <EditorAPI/TESFullName.h>
#include <EditorAPI/TESModelTextureSwap.h>
#include <EditorAPI/BGSMessageIcon.h>
#include <EditorAPI/TESWeightForm.h>
#include <EditorAPI/TESValueForm.h>
#include <EditorAPI/BGSDestructibleObjectForm.h>
#include <EditorAPI/BGSEquipType.h>
#include <EditorAPI/Forms/TESBoundAnimObject.h>
#include <EditorAPI/Forms/BGSSounds.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSLensFlare;
				class TESObjectLIGH :
					public TESBoundAnimObject,
					public TESFullName,
					public TESModelTextureSwap,
					public TESIcon,
					public BGSMessageIcon,
					public TESWeightForm,
					public TESValueForm,
					public BGSDestructibleObjectForm,
					public BGSEquipType
				{
				public:
					enum class TES_LIGHT_FLAGS
					{
						kNone				= 0,
						kDynamic			= 1 << 0,
						kCanCarry			= 1 << 1,
						kNegative			= 1 << 2,
						kFlicker			= 1 << 3,
						kDeepCopy			= 1 << 4,
						kOffByDefault		= 1 << 5,
						kFlickerSlow		= 1 << 6,
						kPulse				= 1 << 7,
						kPulseSlow			= 1 << 8,
						kSpotlight			= 1 << 9,
						kSpotShadow			= 1 << 10,
						kHemiShadow			= 1 << 11,
						kOmniShadow			= 1 << 12,
						kPortalStrict		= 1 << 13,

						kType = kSpotlight | kSpotShadow | kHemiShadow | kOmniShadow
					};
				private:
					struct OBJ_LIGH  // DATA
					{
						std::int32_t time;
						std::uint32_t radius;
						NiAPI::NiRGB color;
						CKPE::TEnumSet<TES_LIGHT_FLAGS, std::uint32_t> flags;
						float fallofExponent;
						float fov;
						float nearDistance;
						float flickerPeriodRecip;
						float flickerIntensityAmplitude;
						float flickerMovementAmplitude;
					};
					static_assert(sizeof(OBJ_LIGH) == 0x28);

					char unk148[16];
					OBJ_LIGH data;
					float fade;
					BGSSoundDescriptorForm* sound;
					NiAPI::NiColor emittanceColor;
					BGSLensFlare* lensFlare; // 128
				public:
					constexpr static std::uint8_t TYPE_ID = ftLight;

					virtual ~TESObjectLIGH() = default;

					// override (BGSEquipType)
					BGSEquipSlot* GetEquipSlot() const noexcept(true) override;
					void SetEquipSlot(BGSEquipSlot* a_slot) noexcept(true) override { /* Nope */ }

					[[nodiscard]] constexpr bool CanBeCarried() const noexcept(true) { return data.flags.all(TES_LIGHT_FLAGS::kCanCarry); }

					[[nodiscard]] constexpr bool HasPulse() const noexcept(true) { return data.flags.all(TES_LIGHT_FLAGS::kPulse); }
					[[nodiscard]] constexpr bool HasFlicker() const noexcept(true) { return data.flags.all(TES_LIGHT_FLAGS::kFlicker); }
					[[nodiscard]] constexpr bool HasPortalStrict() const noexcept(true) { return data.flags.all(TES_LIGHT_FLAGS::kPortalStrict); }
					[[nodiscard]] constexpr bool HasSpotShadow() const noexcept(true) { return data.flags.all(TES_LIGHT_FLAGS::kSpotShadow); }
					[[nodiscard]] constexpr bool HasHemiShadow() const noexcept(true) { return data.flags.all(TES_LIGHT_FLAGS::kHemiShadow); }
					[[nodiscard]] constexpr bool HasOmniShadow() const noexcept(true) { return data.flags.all(TES_LIGHT_FLAGS::kOmniShadow); }

					[[nodiscard]] inline BGSSoundDescriptorForm* GetSoundDescriptorForm() const noexcept(true) { return sound; }
					[[nodiscard]] inline BGSLensFlare* GetLensFlare() const noexcept(true) { return lensFlare; }

					[[nodiscard]] inline NiAPI::NiRGB GetColor() const noexcept(true) { return data.color; }
					inline void SetColor(const NiAPI::NiRGB& a_color) noexcept(true) { data.color = a_color; }
					[[nodiscard]] inline std::uint32_t GetRadius() const noexcept(true) { return data.radius; }
					inline void SetRadius(std::uint32_t a_radius) noexcept(true) { data.radius = a_radius; }
					[[nodiscard]] inline std::int32_t GetTime() const noexcept(true) { return data.time; }
					inline void SetTime(std::int32_t a_time) noexcept(true) { data.time = a_time; }
					[[nodiscard]] inline float GetFallofExponent() const noexcept(true) { return data.fallofExponent; }
					inline void SetFallofExponent(float a_fallofExponent) noexcept(true) { data.fallofExponent = a_fallofExponent; }
					[[nodiscard]] inline float GetFov() const noexcept(true) { return data.fov; }
					inline void SetFov(float a_fov) noexcept(true) { data.fov = a_fov; }
					[[nodiscard]] inline float GetNearDistance() const noexcept(true) { return data.nearDistance; }
					inline void SetNearDistance(float a_nearDistance) noexcept(true) { data.nearDistance = a_nearDistance; }
					[[nodiscard]] inline float GetFlickerPeriodRecip() const noexcept(true) { return data.flickerPeriodRecip; }
					inline void SetFlickerPeriodRecip(float a_flickerPeriodRecip) noexcept(true) { data.flickerPeriodRecip = a_flickerPeriodRecip; }
					[[nodiscard]] inline float GetFlickerIntensityAmplitude() const noexcept(true) { return data.flickerIntensityAmplitude; }
					inline void SetFlickerIntensityAmplitude(float a_flickerIntensityAmplitude) noexcept(true) { data.flickerIntensityAmplitude = a_flickerIntensityAmplitude; }
					[[nodiscard]] inline float GetFlickerMovementAmplitude() const noexcept(true) { return data.flickerMovementAmplitude; }
					inline void SetFlickerMovementAmplitude(float a_flickerMovementAmplitude) noexcept(true) { data.flickerMovementAmplitude = a_flickerMovementAmplitude; }
					[[nodiscard]] inline float GetFade() const noexcept(true) { return fade; }
					inline void SetFade(float a_fade) noexcept(true) { fade = a_fade; }

					CKPE_PROPERTY(GetColor, SetColor) NiAPI::NiRGB Color;
					CKPE_PROPERTY(GetRadius, SetRadius) std::uint32_t Radius;
					CKPE_PROPERTY(GetTime, SetTime) std::int32_t Time;
					CKPE_PROPERTY(GetFallofExponent, SetFallofExponent) float FallofExponent;
					CKPE_PROPERTY(GetFov, SetFov) float Fov;
					CKPE_PROPERTY(GetNearDistance, SetNearDistance) float NearDistance;
					CKPE_PROPERTY(GetFlickerPeriodRecip, SetFlickerPeriodRecip) float FlickerPeriodRecip;
					CKPE_PROPERTY(GetFlickerIntensityAmplitude, SetFlickerIntensityAmplitude) float FlickerIntensityAmplitude;
					CKPE_PROPERTY(GetFlickerMovementAmplitude, SetFlickerMovementAmplitude) float FlickerMovementAmplitude;
					CKPE_PROPERTY(GetFade, SetFade) float Fade;
				};
				static_assert(sizeof(TESObjectLIGH) == 0x1A8);
			}
		}
	}
}