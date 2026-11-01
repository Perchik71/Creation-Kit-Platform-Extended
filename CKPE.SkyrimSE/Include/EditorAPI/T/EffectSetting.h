// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/B/BGSMenuDisplayObject.h>
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
				// 1.7.99.0: size 0x1C0, vtable RVA 0x31269B0
				class EffectSetting :
					public TESForm,	// 00
					public TESFullName,	// 28
					public BGSMenuDisplayObject,	// 40
					public BGSKeywordForm	// 50
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftEffectSetting;

					virtual ~EffectSetting() = default;

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
					std::uint8_t unk68[0x158];	// 68 - data, layout not resolved
				};
				static_assert(offsetof(EffectSetting, unk68) == 0x68);
				static_assert(sizeof(EffectSetting) == 0x1C0);
			}
		}
	}
}