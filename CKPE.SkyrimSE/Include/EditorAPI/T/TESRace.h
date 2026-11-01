// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/TESDescription.h>
#include <EditorAPI/B/TESSpellList.h>
#include <EditorAPI/B/BGSSkinForm.h>
#include <EditorAPI/B/BGSBipedObjectForm.h>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/B/BGSAttackDataForm.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x8C0, vtable RVA 0x30B7058
				class TESRace :
					public TESForm,					// 000
					public TESFullName,				// 028
					public TESDescription,			// 040
					public TESSpellList,			// 068
					public BGSSkinForm,				// 078
					public BGSBipedObjectForm,		// 090
					public BGSKeywordForm,			// 0A0
					public BGSAttackDataForm		// 0B8
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftRace;

					virtual ~TESRace() = default;

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
					std::uint8_t unkC8[0x7F8];	// C8 - race data, layout not resolved
				};
				static_assert(offsetof(TESRace, unkC8) == 0xC8);
				static_assert(sizeof(TESRace) == 0x8C0);
			}
		}
	}
}