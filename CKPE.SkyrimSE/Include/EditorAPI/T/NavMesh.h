// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESChildCell.h>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x198, vtable RVA 0x3155C18
				class NavMesh :
					public TESForm,	// 00
					public TESChildCell	// 28
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftNavMesh;

					virtual ~NavMesh() = default;

					/* 078 */ void ClearData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 090 */ bool LoadEdit(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 0D0 */ TESForm* CreateDuplicateForm(bool bCreateEditorID, void* CopyMap) noexcept(true) override;
					/* 0E0 */ void InitItemImpl() noexcept(true) override;
					/* 0C0 */ [[nodiscard]] bool IsBeforeGroup(const void* GroupRecord) const noexcept(true) override;
					/* 0C8 */ [[nodiscard]] bool IsBeforeForm(TESForm* Rhs) const noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 248 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 250 */ [[nodiscard]] bool CanCheckIn(const void* Forms) const noexcept(true) override;
					/* 258 */ void BuildCheckInDependencyMessage(const void* Forms, void* Message) const noexcept(true) override;
					/* 270 */ bool UndoCheckOut(void* UserFile) noexcept(true) override;
					/* 280 */ bool Revert() noexcept(true) override;
					/* 288 */ void OnFormIDsRemapped(void* FormIDMap) noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					std::uint8_t pad30[0x130];	// 30
					std::uint8_t onam[0x18];	// 160 - ONAM
					std::uint8_t pnam[0x18];	// 178 - PNAM
					std::uint8_t nnam[0x8];	// 190 - NNAM
				};
				static_assert(offsetof(NavMesh, pad30) == 0x30);
				static_assert(offsetof(NavMesh, onam) == 0x160);
				static_assert(offsetof(NavMesh, pnam) == 0x178);
				static_assert(offsetof(NavMesh, nnam) == 0x190);
				static_assert(sizeof(NavMesh) == 0x198);
			}
		}
	}
}