// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSPreloadable.h>
#include <EditorAPI/T/TESBoundObject.h>
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
				// 1.7.99.0: size 0x170, vtable RVA 0x30ED718
				class BGSProjectile :
					public TESBoundObject,	// 00
					public TESFullName,	// 50
					public TESModel,	// 68
					public BGSPreloadable,	// 98
					public BGSDestructibleObjectForm	// A0
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftProjectile;

					virtual ~BGSProjectile() = default;

					/* 070 */ void InitializeData() noexcept(true) override;
					/* 078 */ void ClearData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 0E0 */ void InitItemImpl() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 220 */ void GetObjectWindowTreePath(void* Path) const noexcept(true) override;
					/* 248 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 250 */ [[nodiscard]] bool CanCheckIn(const void* Forms) const noexcept(true) override;
					/* 258 */ void BuildCheckInDependencyMessage(const void* Forms, void* Message) const noexcept(true) override;
					/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C8 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					std::uint8_t padB0[0x88];	// B0
					std::uint8_t unk138[0x30];	// 138 - NAM1/NAM2
					std::uint32_t vnam;	// 168 - VNAM
					std::uint8_t pad16C[0x4];	// 16C
				};
				static_assert(offsetof(BGSProjectile, padB0) == 0xB0);
				static_assert(offsetof(BGSProjectile, unk138) == 0x138);
				static_assert(offsetof(BGSProjectile, vnam) == 0x168);
				static_assert(sizeof(BGSProjectile) == 0x170);
			}
		}
	}
}