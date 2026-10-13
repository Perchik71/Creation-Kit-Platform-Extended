// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESModel.h>
#include <EditorAPI/T/TESImageSpaceModifiableForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				struct CAMERA_SHOT_DATA
				{
					std::int32_t cameraAction; // 00 - REX::TEnumSet<CAM_ACTION, std::int32_t>
					std::int32_t location; // 04 - REX::TEnumSet<CAM_OBJECT, std::int32_t>
					std::int32_t target; // 08 - REX::TEnumSet<CAM_OBJECT, std::int32_t>
					std::uint32_t flags; // 0C
					float playerTimeMult; // 10
					float targetTimeMult; // 14
					float globalTimeMult; // 18
					float maxTime; // 1C
					float minTime; // 20
					float targetPercentBetweenActors; // 24
					float nearTargetDistance; // 28
					float locationSpring; // 2C
					float targetSpring; // 30
					float rotationOffsetX; // 34
					float rotationOffsetY; // 38
					float rotationOffsetZ; // 3C
				};
				static_assert(sizeof(CAMERA_SHOT_DATA) == 0x40);

				// 1.11.240.0: size 0xC8, vtable RVA 0x30553E0, 124 virtual functions
				class BGSCameraShot :
					public TESForm,                      // 000
					public TESModel,                     // 028
					public TESImageSpaceModifiableForm   // 060
				{
				public:
					virtual ~BGSCameraShot() = default;

				public:
					CAMERA_SHOT_DATA data; // 70
					std::uint8_t conditions[0x8]; // B0 - TESCondition
					std::uint8_t unkB8[0x10]; // B8
				};
				static_assert(sizeof(BGSCameraShot) == 0xC8);
			}
		}
	}
}
