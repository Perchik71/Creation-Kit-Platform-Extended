// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BGSMaterialSwap.h>
#include <EditorAPI/T/TESModel.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// size = 0x50
				// func = 27
				class BGSModelMaterialSwap :
					public TESModel   // 000
				{
				public:
					virtual ~BGSModelMaterialSwap() = default;
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
					/* A0 */ void RemapCopiedComponent(BSTHashMap<Forms::TESForm*, Forms::TESForm*>* CopyMap, Forms::TESForm* Copy) noexcept(true) override;

					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSModelMaterialSwap[0];
				public:
					[[nodiscard]] inline const char* GetModelPath() const noexcept(true) { return !modelName.empty() ? modelName.c_str() : nullptr; }
					[[nodiscard]] inline BGSMaterialSwap* GetMaterialSwap() const noexcept(true) { return _MaterialSwap; }
					[[nodiscard]] inline float GetColorRemapIndex() const noexcept(true) { return fColorRemapIndex; }
					[[nodiscard]] inline bool HasColorRemapIndex() const noexcept(true)
					{
						// 0x7F7FFFFF - magic hex number. It's not NaN nor infiniti, I don't understand what it is.
						return uColorRemapIndex == 0x7F7FFFFF;
					}
				public:
					CKPE_READ_PROPERTY(GetModelPath) const char* ModelPath;
					CKPE_READ_PROPERTY(GetMaterialSwap) BGSMaterialSwap* MaterialSwap;
					CKPE_READ_PROPERTY(GetColorRemapIndex) float ColorRemapIndex;
				private:
					// TESModel is 0x38 bytes (modelName at 0x08)
					char pad38[0x8];					// 38
					BGSMaterialSwap* _MaterialSwap;			// 0x40
					union
					{
						float fColorRemapIndex;
						std::uint32_t uColorRemapIndex;
					};
					char pad4C[0x4];					// 4C
				};
				static_assert(sizeof(BGSModelMaterialSwap) == 0x50);
			}
		}
	}
}
