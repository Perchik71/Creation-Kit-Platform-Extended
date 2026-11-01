// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BaseFormComponent.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSTransform;
			}

			class BGSPreviewTransform : public BaseFormComponent
			{
				/* 0x008 */ Forms::BGSTransform* _Transform{ nullptr };
			public:
				enum : std::uint32_t
				{
					kTransformComboBox = 0x41E,
					kPreviewControl = 0x87F
				};

				BGSPreviewTransform() = default;
				BGSPreviewTransform(Forms::BGSTransform& transform) : _Transform(&transform) {}
				BGSPreviewTransform(Forms::BGSTransform* transform) : _Transform(transform) {}
				BGSPreviewTransform(const BGSPreviewTransform& Rhs) : _Transform(Rhs._Transform) {}

				BGSPreviewTransform& operator=(const BGSPreviewTransform& Rhs) noexcept(true) { _Transform = Rhs._Transform; return *this; }

				constexpr bool operator==(const BGSPreviewTransform& Rhs) const noexcept(true) { return _Transform == Rhs._Transform; }
				constexpr bool operator!=(const BGSPreviewTransform& Rhs) const noexcept(true) { return _Transform != Rhs._Transform; }

				virtual ~BGSPreviewTransform() = default;
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

				constexpr static Common::ID VTABLE = IDs::VTABLE::BGSPreviewTransform[0];
			public:
				[[nodiscard]] inline Forms::BGSTransform* GetTransform() const noexcept(true)
				{
					static_assert(offsetof(BGSPreviewTransform, _Transform) == 0x8);
					return _Transform;
				}
				inline void SetTransform(Forms::BGSTransform* transform) noexcept(true) { _Transform = transform; }
			public:
				CKPE_PROPERTY(GetTransform, SetTransform) Forms::BGSTransform* Transform;
			};
			static_assert(sizeof(BGSPreviewTransform) == 0x10);
		}
	}
}