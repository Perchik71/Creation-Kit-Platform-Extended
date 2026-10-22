// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

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
				[[nodiscard]] virtual std::uint32_t GetFormComponentType() const noexcept(true);
				virtual void InitializeDataComponent();
				virtual void ClearDataComponent();
				virtual void InitComponent();
				// MSVC places the owner overload first in the vtable.
				virtual void CopyComponent(BaseFormComponent* Copy) noexcept(true);
				virtual void CopyComponent(BaseFormComponent* Copy, Forms::TESForm* Owner) noexcept(true);
				[[nodiscard]] virtual bool IsComponentDifferent(const BaseFormComponent* Other) const noexcept(true);
				virtual void RemoveComponentReferencesToForm(Forms::TESForm* Form) noexcept(true);
				[[nodiscard]] virtual bool CanCheckInComponentWithOwner(Forms::TESForm* Owner, const void* FormSet) const noexcept(true);
				[[nodiscard]] virtual bool CanCheckInComponent(const void* FormSet) const noexcept(true);
				virtual void BuildComponentCheckInDependencyMessageWithOwner(const void* FormSet, BSString& Message,
					Forms::TESForm* Owner) const noexcept(true);
				virtual void BuildComponentCheckInDependencyMessage(const void* FormSet, BSString& Message) const noexcept(true);
				virtual void GetComponentReferencedForms(void* OutForms) const noexcept(true);
				virtual bool Perform(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result);
				[[nodiscard]] virtual bool IsDialogBoxSuitable(HWND hDialogBox) const noexcept(true);
				virtual void UpdateDataInDialogBox(HWND hDialogBox) const noexcept(true);
				virtual void UpdateDataFromDialogBox(HWND hDialogBox) noexcept(true);
				virtual void CleanupComponentDialogBox(HWND hDialogBox) noexcept(true);
				virtual void CollectComponentResourceFiles(BSTArray<BSFixedString>& Files) noexcept(true);
				virtual void RemapCopiedComponent(void* CopyMap, Forms::TESForm* Copy) noexcept(true);
			};
			static_assert(sizeof(BaseFormComponent) == 0x8);
		}
	}
}