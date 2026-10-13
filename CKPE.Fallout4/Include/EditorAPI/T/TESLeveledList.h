// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BaseFormComponent.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESForm;
				class TESGlobal;
			}

			// 0x18 bytes, array element of TESLeveledList::leveledLists (CK 1.11.240.0)
			struct LEVELED_OBJECT
			{
				Forms::TESForm* form;		// 00
				void* itemExtra;			// 08 - ContainerItemExtra*
				std::uint16_t count;		// 10 - verified: the list calculation adds this many
				std::uint16_t level;		// 12 - verified: entry is used when playerLevel >= level
				std::int8_t chanceNone;		// 14 - verified: rolled against rand(0, 100)
				std::uint8_t pad15[3];		// 15
			};
			static_assert(sizeof(LEVELED_OBJECT) == 0x18);


			// 1.11.240.0: size 0x28, vtable RVA 0x30209D8, 31 virtual functions
			class TESLeveledList :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~TESLeveledList() = default;

				virtual std::int8_t GetChanceNone() const noexcept(true) { return 0; } // 21 - RVA 0x8DE590, v1 = *(_QWORD *)(a1 + 8);; if ( v1 ); return (unsigned int)(int)*(float *)(v1 + 40);; else
				virtual bool GetMultCalc() const noexcept(true) { return false; } // 22 - RVA 0x8DE600, return (*(_BYTE *)(a1 + 34) & 2) != 0;
				virtual std::int32_t GetMaxLevelDifference() const noexcept(true) { return 0; } // 23 - RVA 0x8E2D90, return 0;
				virtual const char* GetOverrideName() const noexcept(true) { return nullptr; } // 24 - RVA 0x8E2DA0, return 0;
				virtual void sub_0C8() { return; } // 25 - RVA 0x8E3640
				virtual void sub_0D0() { return; } // 26 - RVA 0x8E07D0
				virtual void sub_0D8() { return; } // 27 - RVA 0x8258190
				virtual void sub_0E0() { return; } // 28 - RVA 0x8E47B0
				virtual void sub_0E8() { return; } // 29 - RVA 0x8E2B50, return 0;
				virtual void sub_0F0() { return; } // 30 - RVA 0x8E2B40, return 0;

			public:
				Forms::TESGlobal* chanceGlobal; // 08
				void* keywordChances; // 10 - BSTArray<BSTTuple<TESForm*, BGSTypedFormValuePair::SharedVal>>*, heap allocated 24 byte array
				LEVELED_OBJECT* leveledLists; // 18
				std::uint8_t baseListCount; // 20 - number of LEVELED_OBJECT in leveledLists (the CK has no scriptAddedLists)
				std::int8_t chanceNone; // 21
				std::int8_t llFlags; // 22
				std::int8_t maxUseAllCount; // 23
				std::uint32_t unk24; // 24
			};
			static_assert(sizeof(TESLeveledList) == 0x28);
		}
	}
}
