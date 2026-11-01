// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESFormIDListView.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x88, vtable RVA 0x310E140
				class TESImageSpace :
					public TESFormIDListView	// 00
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftImageSpace;

					virtual ~TESImageSpace() = default;

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
					std::uint8_t pad28[0x10];	// 28
					std::uint8_t hnam[0x24];	// 38 - HNAM
					std::uint8_t cnam[0xC];	// 5C - CNAM
					std::uint8_t tnam[0x10];	// 68 - TNAM
					std::uint8_t dnam[0x10];	// 78 - DNAM
				};
				static_assert(offsetof(TESImageSpace, pad28) == 0x28);
				static_assert(offsetof(TESImageSpace, hnam) == 0x38);
				static_assert(offsetof(TESImageSpace, cnam) == 0x5C);
				static_assert(offsetof(TESImageSpace, tnam) == 0x68);
				static_assert(offsetof(TESImageSpace, dnam) == 0x78);
				static_assert(sizeof(TESImageSpace) == 0x88);
			}
		}
	}
}