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
				// 1.7.99.0: size 0x68, vtable RVA 0x309E450
				class BGSMovementType :
					public TESForm	// 00
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftMovementType;

					virtual ~BGSMovementType() = default;

					/* 070 */ void InitializeData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 0E0 */ void InitItemImpl() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					std::uint8_t mnam[0x8];	// 28 - MNAM
					std::uint8_t sped[0x2C];	// 30 - SPED
					std::uint8_t inam[0xC];	// 5C - INAM
				};
				static_assert(offsetof(BGSMovementType, mnam) == 0x28);
				static_assert(offsetof(BGSMovementType, sped) == 0x30);
				static_assert(offsetof(BGSMovementType, inam) == 0x5C);
				static_assert(sizeof(BGSMovementType) == 0x68);
			}
		}
	}
}