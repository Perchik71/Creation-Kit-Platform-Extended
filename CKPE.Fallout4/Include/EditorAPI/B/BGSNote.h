// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESWeightForm.h>
#include <EditorAPI/T/TESValueForm.h>
#include <EditorAPI/T/TESModel.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESIcon.h>
#include <EditorAPI/B/BGSPickupPutdownSounds.h>
#include <EditorAPI/B/BSFixedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x168, vtable RVA 0x3039EF8, 154 virtual functions
				class BGSNote :
					public TESBoundObject,          // 000
					public TESWeightForm,           // 090
					public TESValueForm,            // 0A0
					public TESModel,                // 0B0
					public TESFullName,             // 0E8
					public TESIcon,                 // 100
					public BGSPickupPutdownSounds   // 128
				{
				public:
					virtual ~BGSNote() = default;

					/* 0A8 */ void InitializeData() override;
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
					/* 338 */ void OnDialogFormSaved(HWND hDialogBox, TESForm* SavedForm) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftNote;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSNote[0];
				public:
					BSFixedString programFile; // 140
					std::uint64_t noteFormID; // 148 - form id or pointer depending on type (the game's union of noteSound and noteFormID)
					std::int8_t type; // 150
					bool hasBeenRead; // 151
					std::uint8_t unk152[0xE]; // 152
					std::uint16_t unk160; // 160
					std::uint8_t unk162[0x6]; // 162
				};
				static_assert(sizeof(BGSNote) == 0x168);
			}
		}
	}
}
