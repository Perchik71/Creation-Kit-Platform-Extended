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
				struct CombatStyleGeneralData
				{
					float offensiveMult; // 00
					float defensiveMult; // 04
					float groupOffensiveMult; // 08
					float meleeScoreMult; // 0C
					float magicScoreMult; // 10
					float rangedScoreMult; // 14
					float shoutScoreMult; // 18
					float unarmedScoreMult; // 1C
					float staffScoreMult; // 20
					float avoidThreatChance; // 24
					float dodgeThreatChance; // 28
					float evadeThreatChance; // 2C
				};
				static_assert(sizeof(CombatStyleGeneralData) == 0x30);

				struct CombatStyleMeleeData
				{
					float attackIncapacitatedMult; // 00
					float powerAttackIncapacitatedMult; // 04
					float powerAttackBlockingMult; // 08
					float bashMult; // 0C
					float bashRecoiledMult; // 10
					float bashAttackMult; // 14
					float bashPowerAttackMult; // 18
					float specialAttackMult; // 1C
					float blockWhenIncapacitatedMult; // 20
					float attackWhenIncapacitatedMult; // 24
				};
				static_assert(sizeof(CombatStyleMeleeData) == 0x28);

				struct CombatStyleRangedData
				{
					float accuracyMult; // 00
				};
				static_assert(sizeof(CombatStyleRangedData) == 0x4);

				struct CombatStyleCloseRangeData
				{
					float circleMult; // 00
					float fallbackMult; // 04
					float flankDistanceMult; // 08
					float stalkTimeMult; // 0C
					float chargeDistanceMult; // 10
					float flipThrowProbability; // 14
					float sprintChargeProbability; // 18
					float sideswipeProbability; // 1C
					float disengageProbability; // 20
					std::uint32_t throwMaxTargets; // 24
					float flankVarianceMult; // 28
				};
				static_assert(sizeof(CombatStyleCloseRangeData) == 0x2C);

				struct CombatStyleLongRangeData
				{
					float strafeMult; // 00
					float adjustRangeMult; // 04
					float crouchMult; // 08
					float waitMult; // 0C
					float rangeMult; // 10
				};
				static_assert(sizeof(CombatStyleLongRangeData) == 0x14);

				struct CombatStyleCoverData
				{
					float coverSearchDistanceMult; // 00
				};
				static_assert(sizeof(CombatStyleCoverData) == 0x4);

				struct CombatStyleFlightData
				{
					float hoverChance; // 00
					float diveBombChance; // 04
					float groundAttackChance; // 08
					float hoverTimeMult; // 0C
					float groundAttackTimeMult; // 10
					float perchAttackChance; // 14
					float perchAttackTimeMult; // 18
					float flyingAttackChance; // 1C
				};
				static_assert(sizeof(CombatStyleFlightData) == 0x20);

				// 1.11.240.0: size 0xF0, vtable RVA 0x3088410, 124 virtual functions
				class TESCombatStyle :
					public TESForm   // 000
				{
				public:
					virtual ~TESCombatStyle() = default;

				public:
					CombatStyleGeneralData generalData; // 28
					CombatStyleMeleeData meleeData; // 58
					CombatStyleRangedData rangedData; // 80
					CombatStyleCloseRangeData closeRangeData; // 84
					CombatStyleLongRangeData longRangeData; // B0
					CombatStyleCoverData coverData; // C4
					CombatStyleFlightData flightData; // C8
					std::uint32_t unkE8; // E8
					std::uint32_t unkEC; // EC
				};
				static_assert(sizeof(TESCombatStyle) == 0xF0);
			}
		}
	}
}
