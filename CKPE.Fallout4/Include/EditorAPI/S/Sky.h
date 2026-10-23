// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/IDs.h>
#include <CKPE.Singleton.h>
#include <CKPE.Common.Relocation.h>
#include <EditorAPI/B/BSSettings.h>
#include <EditorAPI/N/NiNode.h>
#include <EditorAPI/N/NiColor.h>
#include <EditorAPI/N/NiFlags.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESClimate;
				class TESWeather;
				class TESRegion;
			}

			class Clouds
			{
			public:
				virtual ~Clouds();

				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{
					return offsetof(Clouds, _Flags) == 0x512;
				}
			private:
				/* 0x008 */ char pad008[0x50A];
				/* 0x512 */ NiAPI::NiTFlags<std::uint16_t, Clouds> _Flags;
			};
			static_assert(sizeof(Clouds) == 0x518);
			static_assert(Clouds::VerifyLayout());

			class Sky
			{
			public:
				enum class Mode : std::uint32_t
				{
					None = 0,
					Interior = 1,
					SkyDomeOnly = 2,
					Full = 3,
				};

				enum Flags : std::uint32_t
				{
					WeatherChanged = 0x1,
					FastTravel = 0x10,
					HideSky = 0x80,
					ReleaseOverride = 0x200000,
				};

				virtual ~Sky();

				// The editor creates this singleton lazily when loading a worldspace.
				[[nodiscard]] inline static Sky* GetSingleton() noexcept(true)
				{
					static Common::Relocation<Sky**> singleton{ EditorAPI::IDs::Sky::Singleton };
					return *singleton;
				}

				inline void Update(float delta) noexcept(true)
				{
					using func_t = void(*)(Sky*, float);
					static Common::Relocation<func_t> func{ EditorAPI::IDs::Sky::Update };
					func(this, delta);
				}

				inline void UpdateWeather() noexcept(true)
				{
					using func_t = void(*)(Sky*);
					static Common::Relocation<func_t> func{ EditorAPI::IDs::Sky::UpdateWeather };
					func(this);
				}

				inline void SetMode(Mode mode) noexcept(true)
				{
					using func_t = void(*)(Sky*, Mode);
					static Common::Relocation<func_t> func{ EditorAPI::IDs::Sky::SetMode };
					func(this, mode);
				}

				inline void ReleaseWeatherOverride() noexcept(true)
				{
					if (_OverrideWeather)
					{
						_Flags.Set(ReleaseOverride);
						_OverrideWeather = nullptr;
					}
				}

				// Legacy fog/weather helpers manipulate mode bits, not sky flags.
				inline void SetFog() noexcept(true) { _Mode |= 1; }
				inline void SetWeather() noexcept(true) { _Mode |= 2; }
				inline void UnsetFog() noexcept(true) { _Mode &= ~1u; }
				inline void UnsetWeather() noexcept(true) { _Mode &= ~2u; }
				[[nodiscard]] inline bool HasFog() const noexcept(true) { return (_Mode & 1) != 0; }
				[[nodiscard]] inline bool HasWeather() const noexcept(true) { return (_Mode & 2) != 0; }
				[[nodiscard]] inline Clouds* GetClouds() const noexcept(true) { return _Clouds; }
				[[nodiscard]] inline Forms::TESClimate* GetCurrentClimate() const noexcept(true) { return _CurrentClimate; }
				[[nodiscard]] inline Forms::TESWeather* GetCurrentWeather() const noexcept(true) { return _CurrentWeather; }
				[[nodiscard]] inline Forms::TESWeather* GetLastWeather() const noexcept(true) { return _LastWeather; }
				[[nodiscard]] inline Forms::TESWeather* GetOverrideWeather() const noexcept(true) { return _OverrideWeather; }
				[[nodiscard]] inline Mode GetMode() const noexcept(true) { return static_cast<Mode>(_Mode); }
				[[nodiscard]] inline std::uint32_t GetFlags() const noexcept(true) { return _Flags.Flags; }
				[[nodiscard]] inline float GetWeatherTransition() const noexcept(true) { return _CurrentWeatherPercent; }
				[[nodiscard]] inline float GetGameHour() const noexcept(true) { return _CurrentGameHour; }
				[[nodiscard]] inline float GetFogNear() const noexcept(true) { return _FogDistances[0]; }
				[[nodiscard]] inline float GetFogFar() const noexcept(true) { return _FogDistances[1]; }
				[[nodiscard]] inline bool IsSkyHidden() const noexcept(true) { return _Flags.Has(HideSky); }

				CKPE_READ_PROPERTY(GetClouds) Clouds* CloudLayer;
				CKPE_READ_PROPERTY(GetCurrentClimate) Forms::TESClimate* CurrentClimate;
				CKPE_READ_PROPERTY(GetCurrentWeather) Forms::TESWeather* CurrentWeather;
				CKPE_READ_PROPERTY(GetLastWeather) Forms::TESWeather* LastWeather;
				CKPE_READ_PROPERTY(GetOverrideWeather) Forms::TESWeather* OverrideWeather;
				CKPE_READ_PROPERTY(GetMode) Mode SkyMode;
				CKPE_READ_PROPERTY(GetFlags) std::uint32_t SkyFlags;
				CKPE_READ_PROPERTY(GetWeatherTransition) float WeatherTransition;

				inline static ISingleton<Setting> Setting_SkyView;
				inline static ISingleton<Setting> Setting_FogEnabled;

				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{
					return offsetof(Sky, _CurrentClimate) == 0x50 &&
						offsetof(Sky, _CurrentWeather) == 0x58 &&
						offsetof(Sky, _LastWeather) == 0x60 &&
						offsetof(Sky, _OverrideWeather) == 0x70 &&
						offsetof(Sky, _Clouds) == 0x98 &&
						offsetof(Sky, _FogDistances) == 0x338 &&
						offsetof(Sky, _FogHeight) == 0x358 &&
						offsetof(Sky, _CurrentGameHour) == 0x368 &&
						offsetof(Sky, _CurrentWeatherPercent) == 0x370 &&
						offsetof(Sky, _Mode) == 0x37C &&
						offsetof(Sky, _Flags) == 0x3A4 &&
						offsetof(Sky, _WeatherImageSpaceModifiers) == 0x3A8;
				}
			private:
				/* 0x008 */ char pad008[0x8];
				/* 0x010 */ NiAPI::NiNode* _Root;
				/* 0x018 */ NiAPI::NiNode* _MoonsRoot;
				/* 0x020 */ NiAPI::NiNode* _AuroraRoot;
				/* 0x028 */ void* _AuroraModel;
				/* 0x030 */ void* _ExteriorLightingOverride;
				/* 0x038 */ std::uint32_t _CurrentRoom;
				/* 0x03C */ std::uint32_t _PreviousRoom;
				/* 0x040 */ float _LightingTransition;
				/* 0x044 */ float _LightingTransitionTimer;
				/* 0x048 */ void* _AmbientLight;
				/* 0x050 */ Forms::TESClimate* _CurrentClimate;
				/* 0x058 */ Forms::TESWeather* _CurrentWeather;
				/* 0x060 */ Forms::TESWeather* _LastWeather;
				/* 0x068 */ Forms::TESWeather* _DefaultWeather;
				/* 0x070 */ Forms::TESWeather* _OverrideWeather;
				/* 0x078 */ Forms::TESRegion* _CurrentRegion;
				/* 0x080 */ class Atmosphere* _Atmosphere;
				/* 0x088 */ class Stars* _Stars;
				/* 0x090 */ class Sun* _Sun;
				/* 0x098 */ Clouds* _Clouds;
				/* 0x0A0 */ class Moon* _Masser;
				/* 0x0A8 */ class Moon* _Secunda;
				/* 0x0B0 */ class Precipitation* _Precipitation;
				/* 0x0B8 */ NiAPI::NiColor _SkyColors[19];
				/* 0x19C */ char pad19C[0x190];
				/* 0x32C */ float _WindSpeed;
				/* 0x330 */ float _WindAngle;
				/* 0x334 */ float _WindTurbulence;
				/* 0x338 */ float _FogDistances[8];
				/* 0x358 */ float _FogHeight;
				/* 0x35C */ float _FogPower;
				/* 0x360 */ float _FogClamp;
				/* 0x364 */ float _FogHighDensityScale;
				/* 0x368 */ float _CurrentGameHour;
				/* 0x36C */ float _LastWeatherUpdate;
				/* 0x370 */ float _CurrentWeatherPercent;
				/* 0x374 */ float _LastWindDirection;
				/* 0x378 */ float _LastWindDirectionRange;
				/* 0x37C */ std::uint32_t _Mode;
				/* 0x380 */ void* _SkySoundList;
				/* 0x388 */ float _Flash;
				/* 0x38C */ char pad38C[0x4];
				/* 0x390 */ std::uint64_t _FlashTime;
				/* 0x398 */ std::uint32_t _LastMoonPhaseUpdate;
				/* 0x39C */ float _WindowReflectionTimer;
				/* 0x3A0 */ float _AccelerationBeginPercent;
				/* 0x3A4 */ NiAPI::NiTFlags<std::uint32_t, Sky> _Flags;
				/* 0x3A8 */ void* _WeatherImageSpaceModifiers[4];
				/* 0x3C8 */ char pad3C8[0x80];
			};
			static_assert(sizeof(Sky) == 0x448);
			static_assert(Sky::VerifyLayout());
		}
	}
}
