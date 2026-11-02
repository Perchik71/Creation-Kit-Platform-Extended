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
					std::uint8_t padC8[0x1E0];	// C8
					void* vtck;	// 2A8 - VTCK
					void* vtck_2B0;	// 2B0 - VTCK
					std::uint8_t pad2B8[0x8];	// 2B8
					void* dnam;	// 2C0 - DNAM
					void* dnam_2C8;	// 2C8 - DNAM
					std::uint8_t pad2D0[0x10];	// 2D0
					void* nam4;	// 2E0 - NAM4
					void* nam5;	// 2E8 - NAM5
					void* nam7;	// 2F0 - NAM7
					std::uint8_t pad2F8[0x128];	// 2F8
					std::uint32_t vnam;	// 420 - VNAM
					std::uint8_t pad424[0x4];	// 424
					void* unes;	// 428 - UNES
					void* nam8;	// 430 - NAM8
					void* rnam;	// 438 - RNAM
					std::uint8_t unk440[0x420];	// 440 - MTYP/SPED
					std::uint16_t tinl;	// 860 - TINL
					std::uint8_t pad862[0x1E];	// 862
					void* wkmv;	// 880 - WKMV
					void* rnmv;	// 888 - RNMV
					void* swmv;	// 890 - SWMV
					void* flmv;	// 898 - FLMV
					void* snmv;	// 8A0 - SNMV
					void* spmv;	// 8A8 - SPMV
					std::uint8_t pad8B0[0x10];	// 8B0
				};
				static_assert(offsetof(TESRace, padC8) == 0xC8);
				static_assert(offsetof(TESRace, vtck) == 0x2A8);
				static_assert(offsetof(TESRace, vtck_2B0) == 0x2B0);
				static_assert(offsetof(TESRace, dnam) == 0x2C0);
				static_assert(offsetof(TESRace, dnam_2C8) == 0x2C8);
				static_assert(offsetof(TESRace, nam4) == 0x2E0);
				static_assert(offsetof(TESRace, nam5) == 0x2E8);
				static_assert(offsetof(TESRace, nam7) == 0x2F0);
				static_assert(offsetof(TESRace, vnam) == 0x420);
				static_assert(offsetof(TESRace, unes) == 0x428);
				static_assert(offsetof(TESRace, nam8) == 0x430);
				static_assert(offsetof(TESRace, rnam) == 0x438);
				static_assert(offsetof(TESRace, unk440) == 0x440);
				static_assert(offsetof(TESRace, tinl) == 0x860);
				static_assert(offsetof(TESRace, wkmv) == 0x880);
				static_assert(offsetof(TESRace, rnmv) == 0x888);
				static_assert(offsetof(TESRace, swmv) == 0x890);
				static_assert(offsetof(TESRace, flmv) == 0x898);
				static_assert(offsetof(TESRace, snmv) == 0x8A0);
				static_assert(offsetof(TESRace, spmv) == 0x8A8);
				static_assert(sizeof(TESRace) == 0x8C0);
			}
		}
	}
}