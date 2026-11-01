// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESCondition.h>
#include <EditorAPI/B/BSTList.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				struct PATH_DATA
				{
					std::int8_t flags; // 00
				};
				static_assert(sizeof(PATH_DATA) == 0x1);

				// 1.11.240.0: size 0x70, vtable RVA 0x3054830, 124 virtual functions
				class BGSCameraPath :
					public TESForm   // 000
				{
				public:
					virtual ~BGSCameraPath() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 100 */ TESForm* CreateDuplicateForm(bool bCreateEditorID, BSTHashMap<TESForm*, TESForm*>* CopyMap) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 248 */ [[nodiscard]] bool IsParentFormTree() const noexcept(true) override;
					/* 280 */ bool RemoveChildFormsFromList(CheckInFormList* Forms) noexcept(true) override;
					/* 288 */ bool CanCheckOutChildren() noexcept(true) override;
					/* 290 */ bool CheckOutChildren(TESBitArrayFile* UserFile, TESBitArrayFile* VersionFile) noexcept(true) override;
					/* 298 */ bool MarkChildFormsAsDeleted(bool bValue, TESBitArrayFile* UserFile, TESBitArrayFile* VersionFile) noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2A8 */ [[nodiscard]] bool CanCheckIn(const CheckInFormList* Forms) const noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 2D8 */ void PrepareForRevert() noexcept(true) override;
					/* 2E0 */ bool Revert() noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 330 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftCameraPath;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSCameraPath[0];
				public:
					// Same member order as CommonLibF4's BGSCameraPath (conditions, shots, data, childPaths, parentPath, prevPath)
					TESCondition conditions; // 28
					BSSimpleList<void*> shots; // 40 - BGSCameraShot*
					PATH_DATA data; // 50
					std::uint8_t pad51[0x7]; // 51
					void* childPaths; // 58 - NiFormArray*
					BGSCameraPath* parentPath; // 60
					BGSCameraPath* prevPath; // 68
				};
				static_assert(sizeof(BGSCameraPath) == 0x70);
				static_assert(offsetof(BGSCameraPath, shots) == 0x40);
				static_assert(offsetof(BGSCameraPath, data) == 0x50);
				static_assert(offsetof(BGSCameraPath, parentPath) == 0x60);
			}
		}
	}
}
