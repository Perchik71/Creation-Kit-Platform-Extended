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
			class TESContainer;

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


			// 1.11.246.0: size 0x28, vtable RVA 0x30219D8, 31 virtual functions
			class TESLeveledList :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~TESLeveledList() = default;

				virtual std::int8_t GetChanceNone() const noexcept(true) { return 0; } // 21 - RVA 0x8DE600
				virtual bool GetMultCalc() const noexcept(true) { return false; } // 22 - RVA 0x8DE670
				virtual std::int32_t GetMaxLevelDifference() const noexcept(true) { return 0; } // 23 - RVA 0x8E2E00
				virtual const char* GetOverrideName() const noexcept(true) { return nullptr; } // 24 - RVA 0x8E2E10
				virtual void CollectInventoryItems(TESContainer* Inventory, std::uint16_t Level) noexcept(true) { return; } // 25
				virtual void CheckForCircularReferences(Forms::TESForm* Owner) const noexcept(true) { return; } // 26
				[[nodiscard]] virtual bool GetCanContainFormsOfType(std::uint32_t FormType) const noexcept(true) { return false; } // 27
				virtual void PopulateListEntryComboBox(HWND hComboBox, const char* Filter) const noexcept(true) { return; } // 28
				[[nodiscard]] virtual bool HasMaleActors() const noexcept(true) { return false; } // 29
				[[nodiscard]] virtual bool HasFemaleActors() const noexcept(true) { return false; } // 30

			public:
				Forms::TESGlobal* chanceGlobal; // 08
				void* keywordChances; // 10 - BSTArray<BSTTuple<TESForm*, BGSTypedFormValuePair::SharedVal>>*, heap allocated 24 byte array
				LEVELED_OBJECT* leveledLists; // 18
				std::uint8_t baseListCount; // 20 - number of LEVELED_OBJECT in leveledLists (the CK has no scriptAddedLists)
				std::int8_t chanceNone; // 21
				std::int8_t llFlags; // 22
				std::int8_t maxUseAllCount; // 23
				std::uint32_t pad24; // 24
			};
			static_assert(sizeof(TESLeveledList) == 0x28);
		}
	}
}
