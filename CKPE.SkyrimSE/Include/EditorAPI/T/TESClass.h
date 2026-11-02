// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/TESDescription.h>
#include <EditorAPI/T/TESTexture.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0xB8, vtable RVA 0x30AA1E8 (CLAS)
				class TESClass :
					public TESForm,			// 00
					public TESFullName,		// 28
					public TESDescription,	// 40
					public TESTexture		// 68
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftClass;

					virtual ~TESClass() = default;

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
					std::uint8_t unk90[0x18];	// 90
					std::uint32_t unkA8;		// A8 - zeroed by InitializeData
					std::uint32_t unkAC;		// AC - zeroed by InitializeData
					std::uint8_t unkB0[0x8];	// B0
				};
				static_assert(offsetof(TESClass, unkA8) == 0xA8);
				static_assert(sizeof(TESClass) == 0xB8);
			}
		}
	}
}