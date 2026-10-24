// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/N/NiObject.h>
#include <EditorAPI/N/NiPointer.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiObjectNET;
				class NiUpdateData;

				class NiTimeController : public NiObject
				{
				public:
					/* 2A */ virtual void Start(float Time) noexcept(true);
					/* 2B */ virtual void Stop() noexcept(true);
					/* 2C */ virtual void Update(NiUpdateData& Data) noexcept(true) = 0;
					/* 2D */ virtual void SetTarget(NiObjectNET* Target) noexcept(true);
					/* 2E */ virtual bool IsTransformController() const noexcept(true);
					/* 2F */ virtual bool IsVertexController() const noexcept(true);
					/* 30 */ virtual float ComputeScaledTime(float Time) noexcept(true);
					/* 31 */ virtual void OnPreDisplay() noexcept(true);
					/* 32 */ virtual bool IsStreamable() const noexcept(true);
					/* 33 */ virtual bool TargetIsRequiredType() const noexcept(true) = 0;

					[[nodiscard]] inline NiObjectNET* GetTarget() const noexcept(true) { return _Target; }
					[[nodiscard]] inline NiTimeController* GetNext() const noexcept(true) { return _Next; }
				private:
					/* 0x10 */ std::uint16_t _Flags;
					/* 0x12 */ std::uint16_t _pad12;
					/* 0x14 */ float _Frequency;
					/* 0x18 */ float _Phase;
					/* 0x1C */ float _LoKeyTime;
					/* 0x20 */ float _HiKeyTime;
					/* 0x24 */ float _StartTime;
					/* 0x28 */ float _LastTime;
					/* 0x2C */ float _WeightedLastTime;
					/* 0x30 */ float _ScaledTime;
					/* 0x34 */ std::uint32_t _pad34;
					/* 0x38 */ NiObjectNET* _Target;
					/* 0x40 */ NiPointer<NiTimeController> _Next;
				public:
					[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
					{
						return offsetof(NiTimeController, _Flags) == 0x10 &&
							offsetof(NiTimeController, _Frequency) == 0x14 &&
							offsetof(NiTimeController, _Phase) == 0x18 &&
							offsetof(NiTimeController, _LoKeyTime) == 0x1C &&
							offsetof(NiTimeController, _HiKeyTime) == 0x20 &&
							offsetof(NiTimeController, _StartTime) == 0x24 &&
							offsetof(NiTimeController, _LastTime) == 0x28 &&
							offsetof(NiTimeController, _WeightedLastTime) == 0x2C &&
							offsetof(NiTimeController, _ScaledTime) == 0x30 &&
							offsetof(NiTimeController, _Target) == 0x38 &&
							offsetof(NiTimeController, _Next) == 0x40;
					}
				};
				static_assert(sizeof(NiTimeController) == 0x48);
				static_assert(NiTimeController::VerifyLayout());
			}
		}
	}
}
