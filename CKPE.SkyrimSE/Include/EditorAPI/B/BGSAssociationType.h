// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x50, vtable RVA 0x30952A8
				class BGSAssociationType :
					public TESForm	// 00
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftAssociationType;

					virtual ~BGSAssociationType() = default;

					/* 070 */ void InitializeData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					std::uint8_t mprt[0x8];	// 28 - MPRT
					std::uint8_t fprt[0x8];	// 30 - FPRT
					std::uint8_t mcht[0x8];	// 38 - MCHT
					std::uint8_t fcht[0x8];	// 40 - FCHT
					std::uint32_t data;	// 48 - DATA
					std::uint8_t pad4C[0x4];	// 4C
				};
				static_assert(offsetof(BGSAssociationType, mprt) == 0x28);
				static_assert(offsetof(BGSAssociationType, fprt) == 0x30);
				static_assert(offsetof(BGSAssociationType, mcht) == 0x38);
				static_assert(offsetof(BGSAssociationType, fcht) == 0x40);
				static_assert(offsetof(BGSAssociationType, data) == 0x48);
				static_assert(sizeof(BGSAssociationType) == 0x50);
			}
		}
	}
}