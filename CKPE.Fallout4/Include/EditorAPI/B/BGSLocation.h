// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESFaction;
				class BGSMusicType;
				class BGSEncounterZone;

				// 1.11.240.0: size 0x1E0, vtable RVA 0x30A3F38, 124 virtual functions
				class BGSLocation :
					public TESForm,         // 000
					public TESFullName,     // 028
					public BGSKeywordForm   // 040
				{
				public:
					virtual ~BGSLocation() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0C0 */ void LoadPartial(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 278 */ void GetObjectWindowTreePath(BSString& Path) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 2B8 */ void GetReferencedForms(BSTBTree<TESForm*>* OutForms) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftLocation;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSLocation[0];
				public:
					BGSLocation* parentLoc; // 60
					TESFaction* unreportedCrimeFaction; // 68
					BGSMusicType* musicType; // 70
					BGSEncounterZone* zone; // 78
					std::uint64_t unk80; // 80
					std::uint32_t worldLocMarker; // 88 - MNAM - ObjectRefHandle
					float worldLocRadius; // 8C - RNAM
					float actorFadeMult; // 90 - ANAM
					std::uint32_t horseLocMarker; // 94 - NAM0 - ObjectRefHandle
					BSTArray<void*> specialRefs; // 98 - BSTArray<SpecialRefData>
					BSTArray<void*> uniqueNPCs; // B0 - BSTArray<UniqueNPCData>
					BSTArray<void*> unkC8; // C8 - element type not identified yet
					// Three BSTHashMap-like tables in the CK's 0x38 byte layout (capacity +0C, free +10, good +14, end of chain marker +18, entries +30); element types not identified: Load, Save, CopyFrom and Compare never touch 0xC8 onwards; the constructor and its factory copy only clear them and install the table end-of-chain markers (E0 uses a different marker object than 148 and 180).
					std::uint8_t hashMapE0[0x38]; // E0 - CK 0x38 byte layout, the entries pointer at +30 is 110
					BSTArray<void*> unk118; // 118 - element type not identified yet
					BSTArray<void*> unk130; // 130 - element type not identified yet
					std::uint8_t hashMap148[0x38]; // 148 - CK 0x38 byte layout, the entries pointer at +30 is 178
					std::uint8_t hashMap180[0x38]; // 180 - CK 0x38 byte layout, the entries pointer at +30 is 1B0
					std::uint64_t unk1B8; // 1B8
					BSTArray<void*> unk1C0; // 1C0 - element type not identified yet
					std::uint32_t unk1D8; // 1D8
					std::uint32_t unk1DC; // 1DC
				};
				static_assert(offsetof(BGSLocation, hashMapE0) == 0xE0);
				static_assert(offsetof(BGSLocation, hashMap148) == 0x148);
				static_assert(offsetof(BGSLocation, hashMap180) == 0x180);
				static_assert(sizeof(BGSLocation) == 0x1E0);
			}
		}
	}
}
