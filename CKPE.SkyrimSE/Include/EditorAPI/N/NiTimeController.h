// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.EnumSet.h>
#include <EditorAPI/N/NiObject.h>
#include <EditorAPI/N/NiPointer.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiObjectNET;
				class NiTimeController :
					public NiObject
				{
					enum class CycleType
					{
						kLoop,
						kReverse,
						kClamp,

						kTotal
					};

					enum class Flag
					{
						kAnimType_AppTime = 0 << 0,
						kAnimType_AppInit = 1 << 0,
						kAnimType_Mask = 1,

						kCycleType_Loop = 0 << 1,
						kCycleType_Reverse = 1 << 1,
						kCycleType_Clamp = 2 << 1,
						kCycleType_Mask = 6,

						kActive = 1 << 3,
						kPlayBackwards = 1 << 4,
						kManagerControlled = 1 << 5,
						kComputeScaledTime = 1 << 6,
						kForceUpdate = 1 << 7
					};
				private:
					// members
					TEnumSet<Flag, std::uint16_t> flags;
					std::uint16_t pad12;
					float frequency;
					float phase;
					float loKeyTime;
					float hiKeyTime;
					float startTime;
					float lastTime;
					float weightedLastTime;
					float scaledTime;
					NiObjectNET* target;
					NiPointer<NiTimeController> next;
				public:
					virtual ~NiTimeController() = default;

					// add
					virtual void Start(float a_time) noexcept(true);
					virtual void Stop() noexcept(true);
					virtual void Update(float a_time) noexcept(true) = 0;
					virtual void SetTarget(NiObjectNET* a_target) noexcept(true);
					[[nodiscard]] virtual bool IsTransformController() const noexcept(true) { return false; }
					[[nodiscard]] virtual bool IsVertexController() const noexcept(true) { return false; }
					virtual float ComputeScaledTime(float a_time) noexcept(true);
					virtual void OnPreDisplay() noexcept(true) { return; }
					[[nodiscard]] virtual bool IsStreamable() const noexcept(true) { return true; }
					[[nodiscard]] virtual bool TargetIsRequiredType() const noexcept(true) = 0;

					[[nodiscard]] constexpr bool HasActive() const noexcept(true) { return flags.all(Flag::kActive); }
					[[nodiscard]] constexpr bool HasPlayBackwards() const noexcept(true) { return flags.all(Flag::kPlayBackwards); }
					[[nodiscard]] constexpr bool HasManagerControlled() const noexcept(true) { return flags.all(Flag::kManagerControlled); }
					[[nodiscard]] constexpr bool HasComputeScaledTime() const noexcept(true) { return flags.all(Flag::kComputeScaledTime); }
					[[nodiscard]] constexpr bool HasForceUpdate() const noexcept(true) { return flags.all(Flag::kForceUpdate); }

					[[nodiscard]] inline float GetFrequency() const noexcept(true) { return frequency; }
					[[nodiscard]] inline float GetPhase() const noexcept(true) { return phase; }
					[[nodiscard]] inline float GetLoKeyTime() const noexcept(true) { return loKeyTime; }
					[[nodiscard]] inline float GetHiKeyTime() const noexcept(true) { return hiKeyTime; }
					[[nodiscard]] inline float GetStartTime() const noexcept(true) { return startTime; }
					[[nodiscard]] inline float GetLastTime() const noexcept(true) { return lastTime; }
					[[nodiscard]] inline float GetWeightedLastTime() const noexcept(true) { return weightedLastTime; }
					[[nodiscard]] inline float GetScaledTime() const noexcept(true) { return scaledTime; }
					[[nodiscard]] inline NiObjectNET* GetTarget() const noexcept(true) { return target; }
				};
				static_assert(sizeof(NiTimeController) == 0x48);
			}
		}
	}
}

