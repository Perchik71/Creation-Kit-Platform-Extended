// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESFormIDListView.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESModel.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x338, vtable RVA 0x3122BA8
				class TESWorldSpace :
					public TESFormIDListView,	// 00
					public TESFullName,	// 28
					public TESModel	// 40
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftWorldSpace;

					virtual ~TESWorldSpace() = default;

					/* 070 */ void InitializeData() noexcept(true) override;
					/* 078 */ void ClearData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 088 */ bool LoadPartial(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 0C0 */ [[nodiscard]] bool IsBeforeGroup(const void* GroupRecord) const noexcept(true) override;
					/* 0C8 */ [[nodiscard]] bool IsBeforeForm(TESForm* Rhs) const noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 1F0 */ [[nodiscard]] bool IsParentForm() const noexcept(true) override;
					/* 200 */ [[nodiscard]] bool IsFormTypeChild(FormType Type) const noexcept(true) override;
					/* 218 */ [[nodiscard]] bool HasAlteredChildren() const noexcept(true) override;
					/* 228 */ bool RemoveChildFormsFromList(void* Forms) noexcept(true) override;
					/* 230 */ bool CanCheckOutChildren() noexcept(true) override;
					/* 238 */ bool CheckOutChildren(void* UserFile, void* VersionFile) noexcept(true) override;
					/* 240 */ bool MarkChildFormsAsDeleted(bool Value, void* UserFile, void* VersionFile) noexcept(true) override;
					/* 248 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 250 */ [[nodiscard]] bool CanCheckIn(const void* Forms) const noexcept(true) override;
					/* 258 */ void BuildCheckInDependencyMessage(const void* Forms, void* Message) const noexcept(true) override;
					/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C8 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					std::uint8_t unk70[0x2C8];	// 70 - data, layout not resolved
				};
				static_assert(offsetof(TESWorldSpace, unk70) == 0x70);
				static_assert(sizeof(TESWorldSpace) == 0x338);
			}
		}
	}
}