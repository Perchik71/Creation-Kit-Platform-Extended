// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/BGSLocalizedString.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x50, vtable RVA 0x30E7188 (WOOP)
				class TESWordOfPower :
					public TESForm,		// 00
					public TESFullName	// 28
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftWordOfPower;

					virtual ~TESWordOfPower() = default;

					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					BGSLocalizedString translation;	// 40 - TNAM, loaded by the same reader as the TESFullName name
				};
				static_assert(offsetof(TESWordOfPower, translation) == 0x40);
				static_assert(sizeof(TESWordOfPower) == 0x50);
			}
		}
	}
}