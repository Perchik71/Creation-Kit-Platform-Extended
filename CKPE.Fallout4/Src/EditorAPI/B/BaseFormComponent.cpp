// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Utils.h>
#include <EditorAPI/B/BaseFormComponent.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			std::uint32_t BaseFormComponent::GetFormComponentType() const noexcept(true)
			{ 
				return CKPE::vtbl_call<std::uint32_t>(0x8, this);
			}

			void BaseFormComponent::InitializeDataComponent()
			{ 
				CKPE::vtbl_call<void>(0x10, this);
			}

			void BaseFormComponent::ClearDataComponent()
			{ 
				CKPE::vtbl_call<void>(0x18, this);
			}

			void BaseFormComponent::InitComponent()
			{ 
				CKPE::vtbl_call<void>(0x20, this);
			}

			void BaseFormComponent::CopyComponent(BaseFormComponent* Copy, Forms::TESForm* Owner) noexcept(true)
			{ 
				CKPE::vtbl_call<void>(0x28, this, Copy, Owner);
			}

			void BaseFormComponent::CopyComponent(BaseFormComponent* Copy) noexcept(true)
			{ 
				CKPE::vtbl_call<void>(0x30, this, Copy);
			}

			bool BaseFormComponent::IsComponentDifferent(const BaseFormComponent* Other) const noexcept(true)
			{ 
				return CKPE::vtbl_call<bool>(0x38, this, Other);
			}

			void BaseFormComponent::RemoveComponentReferencesToForm(Forms::TESForm* Form) noexcept(true)
			{
				CKPE::vtbl_call<void>(0x40, this, Form);
			}

			bool BaseFormComponent::CanCheckInComponentWithOwner(Forms::TESForm* Owner, const Forms::CheckInFormList* FormSet) const noexcept(true)
			{ 
				return CKPE::vtbl_call<bool>(0x48, this, Owner, FormSet);
			}

			bool BaseFormComponent::CanCheckInComponent(const Forms::CheckInFormList* FormSet) const noexcept(true)
			{
				return CKPE::vtbl_call<bool>(0x50, this, FormSet);
			}

			void BaseFormComponent::BuildComponentCheckInDependencyMessageWithOwner(const Forms::CheckInFormList* FormSet, BSString& Message,
				Forms::TESForm* Owner) const noexcept(true)
			{
				CKPE::vtbl_call<void>(0x58, this, FormSet, &Message, Owner);
			}

			void BaseFormComponent::BuildComponentCheckInDependencyMessage(const Forms::CheckInFormList* FormSet, BSString& Message) const noexcept(true)
			{
				CKPE::vtbl_call<void>(0x60, this, FormSet, &Message);
			}

			void BaseFormComponent::GetComponentReferencedForms(BSTBTree<Forms::TESForm*>* OutForms) const noexcept(true)
			{
				CKPE::vtbl_call<void>(0x68, this, OutForms);
			}

			bool BaseFormComponent::Perform(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result)
			{ 
				return CKPE::vtbl_call<bool>(0x70, this, hDialogBox, uMsg, wParam, lParam, Result);
			}

			bool BaseFormComponent::IsDialogBoxSuitable(HWND hDialogBox) const noexcept(true)
			{ 
				return CKPE::vtbl_call<bool>(0x78, this, hDialogBox);
			}

			void BaseFormComponent::UpdateDataInDialogBox(HWND hDialogBox) const noexcept(true)
			{ 
				CKPE::vtbl_call<void>(0x80, this, hDialogBox);
			}

			void BaseFormComponent::UpdateDataFromDialogBox(HWND hDialogBox) noexcept(true)
			{ 
				CKPE::vtbl_call<void>(0x88, this, hDialogBox);
			}

			void BaseFormComponent::CleanupComponentDialogBox(HWND hDialogBox) noexcept(true)
			{ 
				CKPE::vtbl_call<void>(0x90, this, hDialogBox);
			}

			void BaseFormComponent::CollectComponentResourceFiles(BSTArray<BSFixedString>& Files) noexcept(true)
			{
				CKPE::vtbl_call<void>(0x98, this, &Files);
			}

			void BaseFormComponent::RemapCopiedComponent(BSTHashMap<Forms::TESForm*, Forms::TESForm*>* CopyMap, Forms::TESForm* Copy) noexcept(true)
			{
				CKPE::vtbl_call<void>(0xA0, this, CopyMap, Copy);
			}
		}
	}
}