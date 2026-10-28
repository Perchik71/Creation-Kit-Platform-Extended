// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				struct BGSAimModelData
				{
					float aimModelMinConeDegrees; // 00
					float aimModelMaxConeDegrees; // 04
					float aimModelConeIncreasePerShot; // 08
					float aimModelConeDecreasePerSec; // 0C
					std::uint32_t aimModelConeDecreaseDelayMs; // 10
					float aimModelConeSneakMultiplier; // 14
					float aimModelRecoilDiminishSpringForce; // 18
					float aimModelRecoilDiminishSightsMult; // 1C
					float aimModelRecoilMaxDegPerShot; // 20
					float aimModelRecoilMinDegPerShot; // 24
					float aimModelRecoilHipMult; // 28
					std::uint32_t aimModelRecoilShotsForRunaway; // 2C
					float aimModelRecoilArcDeg; // 30
					float aimModelRecoilArcRotateDeg; // 34
					float aimModelConeIronSightsMultiplier; // 38
					float aimModelBaseStability; // 3C
				};
				static_assert(sizeof(BGSAimModelData) == 0x40);

				// 1.11.240.0: size 0x68, vtable RVA 0x2FBC948, 124 virtual functions
				class BGSAimModel :
					public TESForm   // 000
				{
				public:
					virtual ~BGSAimModel() = default;

					constexpr static std::uint8_t TYPE_ID = ftAimModel;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSAimModel[0];
				public:
					BGSAimModelData aimModelData; // 28 - DNAM
				};
				static_assert(sizeof(BGSAimModel) == 0x68);
			}
		}
	}
}
