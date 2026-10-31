// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/BGSMenuDisplayObject.h>
#include <EditorAPI/B/BGSEquipType.h>
#include <EditorAPI/T/TESDescription.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0xD0, vtable RVA 0x307C0F8, 124 virtual functions
				class TESShout :
					public TESForm,               // 000
					public TESFullName,           // 028
					public BGSMenuDisplayObject,  // 040
					public BGSEquipType,          // 050
					public TESDescription         // 060
				{
				public:
					virtual ~TESShout() = default;

					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 130 */ [[nodiscard]] bool HasKnown() const noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2A8 */ [[nodiscard]] bool CanCheckIn(const CheckInFormList* Forms) const noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftShout;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESShout[0];
				public:
					std::uint8_t variations[0x48]; // 88 - Variation
				};
				static_assert(sizeof(TESShout) == 0xD0);
			}
		}
	}
}
