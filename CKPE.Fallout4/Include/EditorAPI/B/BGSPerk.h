// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESDescription.h>
#include <EditorAPI/T/TESIcon.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/B/BGSPerkEntry.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSPerkEntry;
				class BGSSoundDescriptorForm;

				struct PerkData
				{
					bool trait; // 00
					std::int8_t level; // 01
					std::int8_t numRanks; // 02
					bool playable; // 03
					bool hidden; // 04
				};
				static_assert(sizeof(PerkData) == 0x5);

				// 1.11.240.0: size 0x138, vtable RVA 0x303B298, 124 virtual functions
				class BGSPerk :
					public TESForm,         // 000
					public TESFullName,     // 028
					public TESDescription,  // 040
					public TESIcon          // 068
				{
				public:
					virtual ~BGSPerk() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2A8 */ [[nodiscard]] bool CanCheckIn(const CheckInFormList* Forms) const noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 330 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 338 */ void OnDialogFormSaved(HWND hDialogBox, TESForm* SavedForm) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftPerk;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSPerk[0];
				public:
					PerkData data; // 90
					std::uint8_t pad95[0x3]; // 95
					std::uint8_t perkConditions[0x18]; // 98 - TESCondition (0x18 in the CK) - CTDA
					BSTArray<BGSPerkEntry*> perkEntries; // B0 - polymorphic entries, destroyed through their virtual destructor by ClearData
					BGSPerk* nextPerk; // C8 - zeroed by InitializeData
					BGSSoundDescriptorForm* sound; // D0 - zeroed by InitializeData
					BSFixedString swfFile; // D8 - BSFixedStringCS, emptied by ClearData
					std::uint64_t unkE0; // E0 - 0 on construction
					std::uint64_t unkE8; // E8 - 0 on construction
					std::uint8_t unkF0; // F0 - 0 on construction
					std::uint8_t padF1[0x7]; // F1
					// A hash set in the CK's 0x38 byte table layout (capacity 104, good 10C, modification count 118, entries 128, end of chain marker
					// 110): 16 byte entries {key, next}, trivially destructible; the key is a 16 bit value (the insert routine hashes and compares a uint16 at entry +0, CopyFrom copies the set entry by entry).
					std::uint8_t unkF8[0x38]; // F8
					std::uint16_t unk130; // 130 - 0 on construction
					std::uint8_t pad132[0x6]; // 132
				};
				static_assert(offsetof(BGSPerk, swfFile) == 0xD8);
				static_assert(offsetof(BGSPerk, perkEntries) == 0xB0);
				static_assert(offsetof(BGSPerk, unkF8) == 0xF8);
				static_assert(sizeof(BGSPerk) == 0x138);
			}
		}
	}
}
