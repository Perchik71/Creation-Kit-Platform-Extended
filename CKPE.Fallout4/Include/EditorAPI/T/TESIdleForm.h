// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/B/BSFixedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				struct IDLE_DATA
				{
					std::int8_t loopMin; // 00
					std::int8_t loopMax; // 01
					std::int8_t flags; // 02
					std::uint16_t replayDelay; // 04
				};
				static_assert(sizeof(IDLE_DATA) == 0x6);

				// 1.11.246.0: size 0x78, vtable RVA 0x3044AE8, 125 virtual functions
				class TESIdleForm :
					public TESForm   // 000
				{
				public:
					virtual ~TESIdleForm() = default;

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

					constexpr static std::uint8_t TYPE_ID = ftIdle;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESIdleForm[0];

					virtual void OpenIdleManager() noexcept(true) { return; } // 124
				public:
					std::uint8_t unk28[0x18]; // 28
					IDLE_DATA data; // 40
					std::uint16_t unk46; // 46
					std::uint64_t unk48; // 48
					TESIdleForm* parentIdle; // 50
					TESIdleForm* prevIdle; // 58
					BSFixedString behaviorGraphName; // 60
					BSFixedString animEventName; // 68
					BSFixedString animFileName; // 70
				};
				static_assert(sizeof(TESIdleForm) == 0x78);
			}
		}
	}
}
