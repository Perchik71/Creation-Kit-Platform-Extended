// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BaseFormComponent.h>
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
				class TESForm;
				class TESBoundObject;
			}

			// 1.11.246.0: size 0x28 (the game's ContainerItemExtra is 0x18). Allocated per entry by the CNTO/COED loader, freed by
			// TESContainer::ClearDataComponent (0x28 bytes); copy constructor 0x1407D3DF0, default constructor 0x1407D3B00.
			struct ContainerItemExtra
			{
				Forms::TESForm* owner;			// 00 - NPC, faction or other owner of the item
				void* conditional;				// 08 - global variable when the owner is an NPC (ftActor, 45), else the faction rank
				float healthMult;				// 10 - 1.0 on construction
				std::uint8_t unk14;				// 14 - 0 on construction
				std::uint8_t pad15[3];			// 15
				std::int32_t unk18;				// 18 - -1 on construction
				std::uint32_t pad1C;			// 1C
				void* unk20;					// 20 - owned polymorphic object (deleted through its virtual destructor, cloned on copy)
			};
			static_assert(sizeof(ContainerItemExtra) == 0x28);

			// verified in the CK: TESContainer::ClearDataComponent frees 0x18 bytes per entry (+16 points to a 0x28 byte extra)
			struct ContainerObject
			{
				std::int32_t count;			// 00
				std::int32_t pad04;			// 04
				Forms::TESBoundObject* obj;	// 08
				ContainerItemExtra* itemExtra;	// 10
			};
			static_assert(sizeof(ContainerObject) == 0x18);


			// 1.11.246.0: size 0x78, vtable RVA 0x301D938, 22 virtual functions
			class TESContainer :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~TESContainer() = default;
				/* 08 */ [[nodiscard]] std::uint32_t GetFormComponentType() const noexcept(true) override;
				/* 10 */ void InitializeDataComponent() override;
				/* 18 */ void ClearDataComponent() override;
				/* 20 */ void InitComponent() override;
				/* 38 */ [[nodiscard]] bool IsComponentDifferent(const BaseFormComponent* Other) const noexcept(true) override;
				/* 40 */ void RemoveComponentReferencesToForm(Forms::TESForm* Form) noexcept(true) override;
				/* 58 */ void BuildComponentCheckInDependencyMessageWithOwner(const Forms::CheckInFormList* FormSet, BSString& Message, Forms::TESForm* Owner) const noexcept(true) override;
				/* 60 */ void BuildComponentCheckInDependencyMessage(const Forms::CheckInFormList* FormSet, BSString& Message) const noexcept(true) override;
				/* 68 */ void GetComponentReferencedForms(BSTBTree<Forms::TESForm*>* OutForms) const noexcept(true) override;
				/* 70 */ bool Perform(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) override;
				/* 78 */ [[nodiscard]] bool IsDialogBoxSuitable(HWND hDialogBox) const noexcept(true) override;
				/* 80 */ void UpdateDataInDialogBox(HWND hDialogBox) const noexcept(true) override;
				/* 88 */ void UpdateDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
				/* 90 */ void CleanupComponentDialogBox(HWND hDialogBox) noexcept(true) override;
				/* 98 */ void CollectComponentResourceFiles(BSTArray<BSFixedString>& Files) noexcept(true) override;

				constexpr static Common::ID VTABLE = IDs::VTABLE::TESContainer[0];

				virtual void CollectInventoryItems(TESContainer* Inventory, std::uint16_t Level) const noexcept(true) { return; } // 21
			public:
				ContainerObject** containerObjects; // 08
				std::uint32_t numContainerObjects; // 10
				std::uint32_t unk14; // 14 - 0 on construction
				BSTArray<void*> unk18; // 18 - constructed but never referenced by TESContainer's virtual functions
				BSTArray<void*> unk30; // 30 - constructed but never referenced by TESContainer's virtual functions
				BSTArray<void*> unk48; // 48 - constructed but never referenced by TESContainer's virtual functions
				BSTArray<void*> unk60; // 60 - constructed but never referenced by TESContainer's virtual functions
			};
			static_assert(sizeof(TESContainer) == 0x78);
		}
	}
}
