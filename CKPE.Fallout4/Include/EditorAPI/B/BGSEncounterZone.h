// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSLocation;
				class TESForm;

				struct ENCOUNTER_ZONE_DATA
				{
					TESForm* zoneOwner; // 00
					BGSLocation* location; // 08
					std::int8_t ownerRank; // 10
					std::int8_t minLevel; // 11
					std::uint8_t flags; // 12 - REX::TEnumSet<FLAG, std::uint8_t>
					std::int8_t maxLevel; // 13
				};
				static_assert(sizeof(ENCOUNTER_ZONE_DATA) == 0x18);

				// 1.11.240.0: size 0x48, vtable RVA 0x30A2A98, 124 virtual functions
				class BGSEncounterZone :
					public TESForm   // 000
				{
				public:
					virtual ~BGSEncounterZone() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 2B8 */ void GetReferencedForms(BSTBTree<TESForm*>* OutForms) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftEncounterZone;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSEncounterZone[0];
				public:
					ENCOUNTER_ZONE_DATA data; // 28
					std::uint64_t unk40; // 40
				};
				static_assert(sizeof(BGSEncounterZone) == 0x48);
			}
		}
	}
}
