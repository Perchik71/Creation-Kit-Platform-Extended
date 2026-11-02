// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESModelTextureSwap.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x150, vtable RVA 0x309BA30
				class BGSHeadPart :
					public TESForm,	// 00
					public TESFullName,	// 28
					public TESModelTextureSwap	// 40
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftHeadPart;

					virtual ~BGSHeadPart() = default;

					/* 070 */ void InitializeData() noexcept(true) override;
					/* 078 */ void ClearData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 0E0 */ void InitItemImpl() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 220 */ void GetObjectWindowTreePath(void* Path) const noexcept(true) override;
					/* 248 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 250 */ [[nodiscard]] bool CanCheckIn(const void* Forms) const noexcept(true) override;
					/* 258 */ void BuildCheckInDependencyMessage(const void* Forms, void* Message) const noexcept(true) override;
					/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C8 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2F8 */ void CollectAdditionalResourceFiles(void* Files) noexcept(true) override;
				public:
					std::uint8_t data;	// 88 - DATA
					std::uint8_t pad89[0x3];	// 89
					std::uint32_t pnam;	// 8C - PNAM
					std::uint8_t pad90[0x18];	// 90
					void* tnam;	// A8 - TNAM
					std::uint8_t padB0[0x90];	// B0
					void* cnam;	// 140 - CNAM
					void* rnam;	// 148 - RNAM
				};
				static_assert(offsetof(BGSHeadPart, data) == 0x88);
				static_assert(offsetof(BGSHeadPart, pnam) == 0x8C);
				static_assert(offsetof(BGSHeadPart, tnam) == 0xA8);
				static_assert(offsetof(BGSHeadPart, cnam) == 0x140);
				static_assert(offsetof(BGSHeadPart, rnam) == 0x148);
				static_assert(sizeof(BGSHeadPart) == 0x150);
			}
		}
	}
}