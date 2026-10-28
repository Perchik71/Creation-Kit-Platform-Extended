// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/IDs_VTABLE.h>
#include <EditorAPI/C/CheckInFormList.h>
#include <EditorAPI/B/BSTBTree.h>
#include <EditorAPI/B/BSTHashMap.h>
#include <CKPE.Common.h>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/B/BSTArray.h>
#include <windows.h>
#include <cstdint>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSString;

			namespace Forms
			{
				class TESForm;
			}

			class BaseFormComponent
			{
			public:
				virtual ~BaseFormComponent() = default;

				constexpr static Common::ID VTABLE = IDs::VTABLE::BaseFormComponent[0];

				[[nodiscard]] virtual std::uint32_t GetFormComponentType() const noexcept(true);
				virtual void InitializeDataComponent();
				virtual void ClearDataComponent();
				virtual void InitComponent();
				// MSVC places the owner overload first in the vtable.
				virtual void CopyComponent(BaseFormComponent* Copy) noexcept(true);
				virtual void CopyComponent(BaseFormComponent* Copy, Forms::TESForm* Owner) noexcept(true);
				[[nodiscard]] virtual bool IsComponentDifferent(const BaseFormComponent* Other) const noexcept(true);
				virtual void RemoveComponentReferencesToForm(Forms::TESForm* Form) noexcept(true);
				[[nodiscard]] virtual bool CanCheckInComponentWithOwner(Forms::TESForm* Owner, const Forms::CheckInFormList* FormSet) const noexcept(true);
				[[nodiscard]] virtual bool CanCheckInComponent(const Forms::CheckInFormList* FormSet) const noexcept(true);
				virtual void BuildComponentCheckInDependencyMessageWithOwner(const Forms::CheckInFormList* FormSet, BSString& Message,
					Forms::TESForm* Owner) const noexcept(true);
				virtual void BuildComponentCheckInDependencyMessage(const Forms::CheckInFormList* FormSet, BSString& Message) const noexcept(true);
				virtual void GetComponentReferencedForms(BSTBTree<Forms::TESForm*>* OutForms) const noexcept(true);
				virtual bool Perform(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result);
				[[nodiscard]] virtual bool IsDialogBoxSuitable(HWND hDialogBox) const noexcept(true);
				virtual void UpdateDataInDialogBox(HWND hDialogBox) const noexcept(true);
				virtual void UpdateDataFromDialogBox(HWND hDialogBox) noexcept(true);
				virtual void CleanupComponentDialogBox(HWND hDialogBox) noexcept(true);
				virtual void CollectComponentResourceFiles(BSTArray<BSFixedString>& Files) noexcept(true);
				virtual void RemapCopiedComponent(BSTHashMap<Forms::TESForm*, Forms::TESForm*>* CopyMap, Forms::TESForm* Copy) noexcept(true);
			};
			static_assert(sizeof(BaseFormComponent) == 0x8);
		}
	}
}