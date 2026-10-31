// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/BGSLocalizedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x50, vtable RVA 0x307D338, 124 virtual functions
				class TESWordOfPower :
					public TESForm,      // 000
					public TESFullName   // 028
				{
				public:
					virtual ~TESWordOfPower() = default;

					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftWordOfPower;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESWordOfPower[0];
				public:
					BGSLocalizedString translation; // 40
				};
				static_assert(sizeof(TESWordOfPower) == 0x50);
			}
		}
	}
}
