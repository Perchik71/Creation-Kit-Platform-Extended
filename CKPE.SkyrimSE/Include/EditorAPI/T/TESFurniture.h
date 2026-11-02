// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESObjectACTI.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x1E0, vtable RVA 0x30F3FA0
				class TESFurniture :
					public TESObjectACTI
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftFurniture;

					virtual ~TESFurniture() = default;

					/* 070 */ void InitializeData() noexcept(true) override;
					/* 078 */ void ClearData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 0E0 */ void InitItemImpl() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 248 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 250 */ [[nodiscard]] bool CanCheckIn(const void* Forms) const noexcept(true) override;
					/* 258 */ void BuildCheckInDependencyMessage(const void* Forms, void* Message) const noexcept(true) override;
					/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C8 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2F8 */ void CollectAdditionalResourceFiles(void* Files) noexcept(true) override;


				public:
					std::uint8_t pad138[0x48];	// 138
					std::uint8_t unk180[0x18];	// 180 - FNMK/NAM0
					std::uint8_t wbdt[0x4];	// 198 - WBDT
					std::uint32_t mnam;	// 19C - MNAM
					void* nam1;	// 1A0 - NAM1
					std::uint8_t pad1A8[0x38];	// 1A8
				};
				static_assert(offsetof(TESFurniture, pad138) == 0x138);
				static_assert(offsetof(TESFurniture, unk180) == 0x180);
				static_assert(offsetof(TESFurniture, wbdt) == 0x198);
				static_assert(offsetof(TESFurniture, mnam) == 0x19C);
				static_assert(offsetof(TESFurniture, nam1) == 0x1A0);
				static_assert(sizeof(TESFurniture) == 0x1E0);
			}
		}
	}
}