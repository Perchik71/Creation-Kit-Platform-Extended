// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESReactionForm.h>
#include <EditorAPI/T/TESObjectREFR.h>
#include <EditorAPI/B/BSTList.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				struct RANK_DATA;
				class TESCondition;

				class BGSListForm;

				class BGSOutfit;

				class PackageLocation;

				struct FACTION_DATA
				{
					std::uint32_t flags; // 00
				};
				static_assert(sizeof(FACTION_DATA) == 0x4);

				struct FACTION_CRIME_DATA_VALUES
				{
					bool arrest; // 00
					bool attackOnSight; // 01
					std::uint16_t murderCrimeGold; // 02
					std::uint16_t assaultCrimeGold; // 04
					std::uint16_t trespassCrimeGold; // 06
					std::uint16_t pickpocketCrimeGold; // 08
					float stealCrimeGoldMult; // 0C
					std::uint16_t escapeCrimeGold; // 10
					std::uint16_t unused; // 12
				};
				static_assert(sizeof(FACTION_CRIME_DATA_VALUES) == 0x14);

				struct FACTION_VENDOR_DATA_VALUES
				{
					std::uint16_t startHour; // 00
					std::uint16_t endHour; // 02
					std::uint32_t locationRadius; // 04
					bool buysStolen; // 08
					bool notBuySell; // 09
					bool buysNonStolen; // 0A
				};
				static_assert(sizeof(FACTION_VENDOR_DATA_VALUES) == 0xC);

				// 1.11.240.0: size 0xF8, vtable RVA 0x3042CB0, 124 virtual functions
				class TESFaction :
					public TESForm,          // 000
					public TESFullName,      // 028
					public TESReactionForm   // 040
				{
				public:
					virtual ~TESFaction() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B0 */ void ClearData() override;
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
					/* 330 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftFaction;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESFaction[0];
				public:
					FACTION_DATA data; // 60
					std::uint32_t pad64; // 64
					TESObjectREFR* factionJailMarker; // 68
					TESObjectREFR* factionWaitMarker; // 70
					TESObjectREFR* factionStolenContainer; // 78
					TESObjectREFR* factionPlayerInventoryContainer; // 80
					BGSListForm* crimeGroup; // 88
					BGSOutfit* jailOutfit; // 90
					FACTION_CRIME_DATA_VALUES crimeValues; // 98 - CRVA
					std::uint32_t padAC; // AC
					FACTION_VENDOR_DATA_VALUES vendorValues; // B0 - VENV
					std::uint32_t padBC; // BC
					PackageLocation* vendorLocation; // C0
					TESCondition* vendorConditions; // C8
					BGSListForm* vendorSellBuyList; // D0
					TESObjectREFR* merchantContainer; // D8
					BSSimpleList<RANK_DATA*> rankDataList; // E0 - BSSimpleList<RANK_DATA*>
					std::uint64_t unkF0; // F0
				};
				static_assert(sizeof(TESFaction) == 0xF8);
			}
		}
	}
}
