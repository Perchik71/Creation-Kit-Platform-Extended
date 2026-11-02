// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BGSPickupPutdownSounds.h>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESIcon.h>
#include <EditorAPI/T/TESModel.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x100, vtable RVA 0x309EDD0
				class BGSNote :
					public TESBoundObject,	// 00
					public TESModel,	// 50
					public TESFullName,	// 80
					public TESIcon,	// 98
					public BGSPickupPutdownSounds	// C0
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftNote;

					virtual ~BGSNote() = default;

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
					/* 2F8 */ void CollectAdditionalResourceFiles(void* Files) noexcept(true) override;
				public:
					std::uint8_t padD8[0x20];	// D8
					std::uint8_t data;	// F8 - DATA
					std::uint8_t padF9[0x7];	// F9
				};
				static_assert(offsetof(BGSNote, padD8) == 0xD8);
				static_assert(offsetof(BGSNote, data) == 0xF8);
				static_assert(sizeof(BGSNote) == 0x100);
			}
		}
	}
}