// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BaseFormComponent.h>
#include <EditorAPI/B/BSTList.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESPackage;
			}

			// CK uses byte-valued traits and three dword aggression radii.
			struct AIDATA_GAME
			{
				/* 0x00 */ std::uint8_t aggression;
				/* 0x01 */ std::uint8_t confidence;
				/* 0x02 */ std::uint8_t energy;
				/* 0x03 */ std::uint8_t morality;
				/* 0x04 */ std::uint8_t unused;
				/* 0x05 */ std::uint8_t assistance;
				/* 0x06 */ bool useAggroRadius;
				/* 0x07 */ std::uint8_t pad07;
				/* 0x08 */ std::uint32_t aggroRadius[3];
				/* 0x14 */ bool noSlowApproach;
				/* 0x15 */ std::uint8_t pad15[3];
			};
			static_assert(sizeof(AIDATA_GAME) == 0x18);
			static_assert(offsetof(AIDATA_GAME, aggroRadius) == 0x8);
			static_assert(offsetof(AIDATA_GAME, noSlowApproach) == 0x14);


			// 1.11.240.0: size 0x30, vtable RVA 0x301C2C0, 21 virtual functions
			class TESAIForm :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~TESAIForm() = default;
				/* 10 */ void InitializeDataComponent() override;
				/* 18 */ void ClearDataComponent() override;
				/* 20 */ void InitComponent() override;
				/* 38 */ [[nodiscard]] bool IsComponentDifferent(const BaseFormComponent* Other) const noexcept(true) override;
				/* 40 */ void RemoveComponentReferencesToForm(Forms::TESForm* Form) noexcept(true) override;
				/* 60 */ void BuildComponentCheckInDependencyMessage(const Forms::CheckInFormList* FormSet, BSString& Message) const noexcept(true) override;
				/* 68 */ void GetComponentReferencedForms(BSTBTree<Forms::TESForm*>* OutForms) const noexcept(true) override;
				/* 70 */ bool Perform(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) override;
				/* 78 */ [[nodiscard]] bool IsDialogBoxSuitable(HWND hDialogBox) const noexcept(true) override;
				/* 80 */ void UpdateDataInDialogBox(HWND hDialogBox) const noexcept(true) override;
				/* 88 */ void UpdateDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
				/* 90 */ void CleanupComponentDialogBox(HWND hDialogBox) noexcept(true) override;
				/* 98 */ void CollectComponentResourceFiles(BSTArray<BSFixedString>& Files) noexcept(true) override;

				constexpr static Common::ID VTABLE = IDs::VTABLE::TESAIForm[0];
			public:
				AIDATA_GAME aiData; // 08
				BSSimpleList<Forms::TESPackage*> aiPackList; // 20 - CommonLibF4 PackageList (at 18 there)
			};
			static_assert(sizeof(TESAIForm) == 0x30);
			static_assert(offsetof(TESAIForm, aiData) == 0x8);
			static_assert(offsetof(TESAIForm, aiPackList) == 0x20);
		}
	}
}
