// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/TESReactionForm.h>
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
				// 1.7.99.0: size 0xF8, vtable RVA 0x30AB2F0
				class TESFaction :
					public TESForm,	// 00
					public TESFullName,	// 28
					public TESReactionForm	// 40
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftFaction;

					virtual ~TESFaction() = default;

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
					void* jail;	// 68 - JAIL
					void* wait;	// 70 - WAIT
					void* stol;	// 78 - STOL
					void* plcn;	// 80 - PLCN
					void* crgr;	// 88 - CRGR
					void* jout;	// 90 - JOUT
					std::uint8_t crva[0x18];	// 98 - CRVA
					std::uint8_t venv[0x10];	// B0 - VENV
					void* plvd;	// C0 - PLVD
					std::uint8_t padC8[0x8];	// C8
					void* vend;	// D0 - VEND
					void* venc;	// D8 - VENC
					std::uint8_t padE0[0x18];	// E0
				};
				static_assert(offsetof(TESFaction, jail) == 0x68);
				static_assert(offsetof(TESFaction, wait) == 0x70);
				static_assert(offsetof(TESFaction, stol) == 0x78);
				static_assert(offsetof(TESFaction, plcn) == 0x80);
				static_assert(offsetof(TESFaction, crgr) == 0x88);
				static_assert(offsetof(TESFaction, jout) == 0x90);
				static_assert(offsetof(TESFaction, crva) == 0x98);
				static_assert(offsetof(TESFaction, venv) == 0xB0);
				static_assert(offsetof(TESFaction, plvd) == 0xC0);
				static_assert(offsetof(TESFaction, vend) == 0xD0);
				static_assert(offsetof(TESFaction, venc) == 0xD8);
				static_assert(sizeof(TESFaction) == 0xF8);
			}
		}
	}
}