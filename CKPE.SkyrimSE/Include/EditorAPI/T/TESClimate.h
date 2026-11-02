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
				// 1.7.99.0: size 0xF0, vtable RVA 0x310D0E0
				class TESClimate :
					public TESForm	// 00
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftClimate;

					virtual ~TESClimate() = default;

					/* 070 */ void InitializeData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C8 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					std::uint8_t unk28[0x30];	// 28 - MODL/MODT
					std::uint8_t wlst[0x80];	// 58 - WLST
					std::uint8_t tnam[0x18];	// D8 - TNAM
				};
				static_assert(offsetof(TESClimate, unk28) == 0x28);
				static_assert(offsetof(TESClimate, wlst) == 0x58);
				static_assert(offsetof(TESClimate, tnam) == 0xD8);
				static_assert(sizeof(TESClimate) == 0xF0);
			}
		}
	}
}