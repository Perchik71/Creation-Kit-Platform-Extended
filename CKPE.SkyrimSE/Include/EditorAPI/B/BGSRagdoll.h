// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESModel.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x248, vtable RVA 0x30A1F50
				class BGSRagdoll :
					public TESForm,	// 00
					public TESModel	// 28
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftRagdoll;

					virtual ~BGSRagdoll() = default;

					/* 070 */ void InitializeData() noexcept(true) override;
					/* 078 */ void ClearData() noexcept(true) override;
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
					/* 2C8 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					std::uint8_t pad58[0x84];	// 58
					std::uint8_t rafd[0x12C];	// DC - RAFD
					std::uint8_t raps[0x20];	// 208 - RAPS
					void* tnam;	// 228 - TNAM
					void* xnam;	// 230 - XNAM
					std::uint8_t pad238[0x10];	// 238
				};
				static_assert(offsetof(BGSRagdoll, pad58) == 0x58);
				static_assert(offsetof(BGSRagdoll, rafd) == 0xDC);
				static_assert(offsetof(BGSRagdoll, raps) == 0x208);
				static_assert(offsetof(BGSRagdoll, tnam) == 0x228);
				static_assert(offsetof(BGSRagdoll, xnam) == 0x230);
				static_assert(sizeof(BGSRagdoll) == 0x248);
			}
		}
	}
}