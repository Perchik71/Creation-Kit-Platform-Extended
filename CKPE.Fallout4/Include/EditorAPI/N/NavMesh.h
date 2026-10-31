// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESChildCell.h>
#include <EditorAPI/F/FormInterfaces.h>
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
				// 1.11.240.0: size 0x290, vtable RVA 0x30FBD40, 124 virtual functions
				class NavMesh :
					public TESForm,       // 000
					public TESChildCell,  // 028
					public BSNavmesh      // 030
				{
				public:
					virtual ~NavMesh() = default;

					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0C8 */ void LoadEdit(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 0F0 */ [[nodiscard]] bool IsBeforeGroup(const TESGroupHeader* GroupRecord) const noexcept(true) override;
					/* 0F8 */ [[nodiscard]] bool IsBeforeForm(TESForm* Rhs) const noexcept(true) override;
					/* 100 */ TESForm* CreateDuplicateForm(bool bCreateEditorID, BSTHashMap<TESForm*, TESForm*>* CopyMap) override;
					/* 108 */ void FixupDuplicateForm(BSTHashMap<TESForm*, TESForm*>* CopyMap) noexcept(true) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 128 */ void DebugInfo(char* lpBuffer, std::uint32_t nBufferSize) const noexcept(true) override;
					/* 198 */ void MarkAsDeleted(bool bValue) noexcept(true) override;
					/* 1A0 */ void MarkAsChanged(bool bValue) noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 220 */ bool BelongsInGroup(TESGroupHeader* Group, bool bAllowParentGroups, bool bCurrentOnly) override;
					/* 228 */ void CreateGroupData(TESGroupHeader* OutGroup, TESGroupHeader* ParentGroup) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2A8 */ [[nodiscard]] bool CanCheckIn(const CheckInFormList* Forms) const noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 2D0 */ bool UndoCheckOut(TESBitArrayFile* UserFile) noexcept(true) override;
					/* 2E0 */ bool Revert() noexcept(true) override;
					/* 2E8 */ void OnFormIDsRemapped(BSTHashMap<std::uint32_t, std::uint32_t>* FormIDMap) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;

				public:
					std::uint32_t unk258; // 258
					std::uint32_t unk25C; // 25C
					BSTArray<void*> unk260; // 260 - element type not identified yet
					BSTArray<void*> unk278; // 278 - element type not identified yet
				};
				static_assert(sizeof(NavMesh) == 0x290);
			}
		}
	}
}
