// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BGSStoryManagerTreeForm.h>
#include <EditorAPI/T/TESFullName.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x218, vtable RVA 0x30B31E0
				class TESQuest :
					public BGSStoryManagerTreeForm,	// 00
					public TESFullName	// 28
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftQuest;

					virtual ~TESQuest() = default;

					/* 070 */ void InitializeData() noexcept(true) override;
					/* 078 */ void ClearData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 220 */ void GetObjectWindowTreePath(void* Path) const noexcept(true) override;
					/* 230 */ bool CanCheckOutChildren() noexcept(true) override;
					/* 240 */ bool MarkChildFormsAsDeleted(bool Value, void* UserFile, void* VersionFile) noexcept(true) override;
					/* 248 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 250 */ [[nodiscard]] bool CanCheckIn(const void* Forms) const noexcept(true) override;
					/* 258 */ void BuildCheckInDependencyMessage(const void* Forms, void* Message) const noexcept(true) override;
					/* 280 */ bool Revert() noexcept(true) override;
					/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C8 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2E8 */ void InitializeListView(HWND hListView) const noexcept(true) override;
				public:
					std::uint8_t unk40[0x1D8];	// 40 - data, layout not resolved
				};
				static_assert(offsetof(TESQuest, unk40) == 0x40);
				static_assert(sizeof(TESQuest) == 0x218);
			}
		}
	}
}