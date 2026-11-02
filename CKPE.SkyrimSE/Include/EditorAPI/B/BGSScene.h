// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x2E8, vtable RVA 0x30A4EC0
				class BGSScene :
					public TESForm	// 00
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftScene;

					virtual ~BGSScene() = default;

					/* 078 */ void ClearData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 0E0 */ void InitItemImpl() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 230 */ bool CanCheckOutChildren() noexcept(true) override;
					/* 240 */ bool MarkChildFormsAsDeleted(bool Value, void* UserFile, void* VersionFile) noexcept(true) override;
					/* 248 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 250 */ [[nodiscard]] bool CanCheckIn(const void* Forms) const noexcept(true) override;
					/* 258 */ void BuildCheckInDependencyMessage(const void* Forms, void* Message) const noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					std::uint8_t pad28[0x80];	// 28
					std::uint32_t fnam;	// A8 - FNAM
					std::uint8_t padAC[0x14];	// AC
					std::uint8_t vnam[0x10];	// C0 - VNAM
					std::uint32_t inam;	// D0 - INAM
					std::uint8_t padD4[0x214];	// D4
				};
				static_assert(offsetof(BGSScene, pad28) == 0x28);
				static_assert(offsetof(BGSScene, fnam) == 0xA8);
				static_assert(offsetof(BGSScene, vnam) == 0xC0);
				static_assert(offsetof(BGSScene, inam) == 0xD0);
				static_assert(sizeof(BGSScene) == 0x2E8);
			}
		}
	}
}