// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/B/BGSIdleCollection.h>
#include <EditorAPI/T/TESPackageData.h>
#include <EditorAPI/T/TESCondition.h>
#include <EditorAPI/P/PackageLocation.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESPackageData;
				class PackageLocation;
				class PackageTarget;

				class TESCombatStyle;
				class TESQuest;

				struct PACKAGE_DATA
				{
					std::uint32_t packFlags; // 00
					std::int8_t packType; // 04
					std::int8_t interruptOverrideType; // 05
					std::int8_t maxSpeed; // 06
					std::uint16_t foBehaviorFlags; // 08
					std::uint16_t packageSpecificFlags; // 0A
				};
				static_assert(sizeof(PACKAGE_DATA) == 0xC);

				struct PACK_SCHED_DATA
				{
					std::int8_t month; // 00
					std::int8_t dayOfWeek; // 01
					std::int8_t date; // 02
					std::int8_t hour; // 03
					std::int8_t min; // 04
					std::int32_t duration; // 08
				};
				static_assert(sizeof(PACK_SCHED_DATA) == 0xC);

				struct PackageSchedule
				{
					PACK_SCHED_DATA psData; // 00
				};
				static_assert(sizeof(PackageSchedule) == 0xC);

				// 1.11.240.0: size 0x118, vtable RVA 0x3006220, 124 virtual functions
				class TESPackage :
					public TESForm   // 000
				{
				public:
					virtual ~TESPackage() = default;

				public:
					PACKAGE_DATA data; // 28
					std::uint32_t unk34; // 34
					TESPackageData* packData; // 38
					PackageLocation* packLoc; // 40
					PackageTarget* packTarg; // 48
					BGSIdleCollection* idleCollection; // 50
					PackageSchedule packSched; // 58
					std::uint8_t unk64[0x4]; // 64
					TESCondition packConditions; // 68
					TESCombatStyle* combatStyle; // 80
					TESQuest* ownerQuest; // 88
					std::uint8_t onBegin[0x20]; // 90 - PackageEventAction (0x20 in the CK)
					std::uint8_t onEnd[0x20]; // B0 - PackageEventAction
					std::uint8_t onChange[0x20]; // D0 - PackageEventAction
					std::uint64_t unkF0; // F0
					std::uint64_t unkF8; // F8
					std::uint64_t unk100; // 100
					std::uint64_t unk108; // 108
					std::uint64_t unk110; // 110
				};
				static_assert(sizeof(TESPackage) == 0x118);
				static_assert(offsetof(TESPackage, packData) == 0x38);
				static_assert(offsetof(TESPackage, packSched) == 0x58);
				static_assert(offsetof(TESPackage, packConditions) == 0x68);
				static_assert(offsetof(TESPackage, onBegin) == 0x90);
			}
		}
	}
}
