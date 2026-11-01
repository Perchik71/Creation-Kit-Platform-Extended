// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/TESDescription.h>
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
				// 1.7.99.0: size 0x90, vtable RVA 0x30C8F88
				class BGSMessage :
					public TESForm,	// 00
					public TESFullName,	// 28
					public TESDescription	// 40
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftMessage;

					virtual ~BGSMessage() = default;

					/* 070 */ void InitializeData() noexcept(true) override;
					/* 078 */ void ClearData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
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
					std::uint8_t pad68[0x8];	// 68
					void* qnam;	// 70 - QNAM
					std::uint8_t pad78[0x10];	// 78
					std::uint32_t dnam;	// 88 - DNAM
					std::uint32_t tnam;	// 8C - TNAM
				};
				static_assert(offsetof(BGSMessage, pad68) == 0x68);
				static_assert(offsetof(BGSMessage, qnam) == 0x70);
				static_assert(offsetof(BGSMessage, dnam) == 0x88);
				static_assert(offsetof(BGSMessage, tnam) == 0x8C);
				static_assert(sizeof(BGSMessage) == 0x90);
			}
		}
	}
}