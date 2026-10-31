// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BaseFormComponent.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// CK 1.11.240.0: size 0x8 (only the vtable pointer; the scalar deleting destructor frees 8 bytes),
			// 21 virtual functions. The constructor only stores the vtable, the destructor takes the global spin lock.
			// The dword that follows the component inside TESBoundObject (0x88) belongs to TESBoundObject, not to the component.
			class BGSSoundTagComponent :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~BGSSoundTagComponent() = default;
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

				constexpr static Common::ID VTABLE = IDs::VTABLE::BGSSoundTagComponent[0];
			};
			static_assert(sizeof(BGSSoundTagComponent) == 0x8);
		}
	}
}
