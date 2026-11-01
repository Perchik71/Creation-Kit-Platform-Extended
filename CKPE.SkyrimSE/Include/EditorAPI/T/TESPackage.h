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
				// 1.7.99.0: size 0x108, vtable RVA 0x3058C50
				class TESPackage :
					public TESForm	// 00
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftPackage;

					virtual ~TESPackage() = default;

					/* 070 */ void InitializeData() noexcept(true) override;
					/* 078 */ void ClearData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 220 */ void GetObjectWindowTreePath(void* Path) const noexcept(true) override;
					/* 230 */ bool CanCheckOutChildren() noexcept(true) override;
					/* 238 */ bool CheckOutChildren(void* UserFile, void* VersionFile) noexcept(true) override;
					/* 248 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 250 */ [[nodiscard]] bool CanCheckIn(const void* Forms) const noexcept(true) override;
					/* 258 */ void BuildCheckInDependencyMessage(const void* Forms, void* Message) const noexcept(true) override;
					/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					std::uint8_t pkdt[0x58];	// 28 - PKDT
					void* qnam;	// 80 - QNAM
					std::uint8_t poba[0x20];	// 88 - POBA
					std::uint8_t poea[0x20];	// A8 - POEA
					std::uint8_t poca[0x40];	// C8 - POCA
				};
				static_assert(offsetof(TESPackage, pkdt) == 0x28);
				static_assert(offsetof(TESPackage, qnam) == 0x80);
				static_assert(offsetof(TESPackage, poba) == 0x88);
				static_assert(offsetof(TESPackage, poea) == 0xA8);
				static_assert(offsetof(TESPackage, poca) == 0xC8);
				static_assert(sizeof(TESPackage) == 0x108);
			}
		}
	}
}