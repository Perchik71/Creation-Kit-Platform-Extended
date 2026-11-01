// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/B/BSString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x80, vtable RVA 0x3083310, 124 virtual functions
				class BGSPackIn :
					public TESForm   // 000
				{
				public:
					virtual ~BGSPackIn() = default;

					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 108 */ void FixupDuplicateForm(BSTHashMap<TESForm*, TESForm*>* CopyMap) noexcept(true) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 198 */ void MarkAsDeleted(bool bValue) noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 278 */ void GetObjectWindowTreePath(BSString& Path) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 2B8 */ void GetReferencedForms(BSTBTree<TESForm*>* OutForms) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftPackIn;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSPackIn[0];
				public:
					TESForm* storageCell; // 28 - CNAM; holds the form id after Load until InitItem resolves it ("Could not find storage cell")
					BSString filter; // 30 - FLTR chunk (editor filter string)
					std::uint32_t unk40; // 40
					std::uint8_t unk44[0x10]; // 44
					std::uint8_t unk54[0x8]; // 54
					std::uint32_t unk5C; // 5C
					std::uint64_t unk60; // 60
					std::uint32_t unk68; // 68
					std::uint8_t unk6C[0xC]; // 6C
					std::uint64_t unk78; // 78
				};
				static_assert(sizeof(BGSPackIn) == 0x80);
				static_assert(offsetof(BGSPackIn, storageCell) == 0x28);
				static_assert(offsetof(BGSPackIn, filter) == 0x30);
				static_assert(offsetof(BGSPackIn, unk40) == 0x40);
			}
		}
	}
}
