// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/TESDescription.h>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x80, vtable RVA 0x30BF6D0
				class BGSCollisionLayer :
					public TESForm,	// 00
					public TESDescription	// 28
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftCollisionLayer;

					virtual ~BGSCollisionLayer() = default;

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
				public:
					std::uint32_t bnam;	// 50 - BNAM
					std::uint32_t fnam;	// 54 - FNAM
					std::uint32_t gnam;	// 58 - GNAM
					std::uint8_t pad5C[0x4];	// 5C
					std::uint8_t mnam[0x20];	// 60 - MNAM
				};
				static_assert(offsetof(BGSCollisionLayer, bnam) == 0x50);
				static_assert(offsetof(BGSCollisionLayer, fnam) == 0x54);
				static_assert(offsetof(BGSCollisionLayer, gnam) == 0x58);
				static_assert(offsetof(BGSCollisionLayer, mnam) == 0x60);
				static_assert(sizeof(BGSCollisionLayer) == 0x80);
			}
		}
	}
}