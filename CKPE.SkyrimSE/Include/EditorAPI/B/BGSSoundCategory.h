// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BSISoundCategory.h>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x68, vtable RVA 0x30CD170
				class BGSSoundCategory :
					public TESForm,	// 00
					public TESFullName,	// 28
					public BSISoundCategory	// 40
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftSoundCategory;

					virtual ~BGSSoundCategory() = default;

					/* 070 */ void InitializeData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 0E0 */ void InitItemImpl() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 248 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 250 */ [[nodiscard]] bool CanCheckIn(const void* Forms) const noexcept(true) override;
					/* 258 */ void BuildCheckInDependencyMessage(const void* Forms, void* Message) const noexcept(true) override;
					/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					std::uint32_t fnam;	// 48 - FNAM
					std::uint8_t pad4C[0x10];	// 4C
					std::uint16_t vnam;	// 5C - VNAM
					std::uint16_t unam;	// 5E - UNAM
					std::uint8_t pad60[0x8];	// 60
				};
				static_assert(offsetof(BGSSoundCategory, fnam) == 0x48);
				static_assert(offsetof(BGSSoundCategory, vnam) == 0x5C);
				static_assert(offsetof(BGSSoundCategory, unam) == 0x5E);
				static_assert(sizeof(BGSSoundCategory) == 0x68);
			}
		}
	}
}