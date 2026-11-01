// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESModelTextureSwap.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSOpenCloseForm.h>
#include <EditorAPI/B/BGSKeywordForm.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x138, vtable RVA 0x30FAB88
				class TESObjectACTI :
					public TESBoundObject,				// 00
					public TESFullName,					// 50
					public TESModelTextureSwap,			// 68
					public BGSDestructibleObjectForm,	// B0
					public BGSOpenCloseForm,			// C0
					public BGSKeywordForm				// C8
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftActivator;

					virtual ~TESObjectACTI() = default;

					/* 070 */ void InitializeData() noexcept(true) override;
					/* 078 */ void ClearData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 248 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 250 */ [[nodiscard]] bool CanCheckIn(const void* Forms) const noexcept(true) override;
					/* 258 */ void BuildCheckInDependencyMessage(const void* Forms, void* Message) const noexcept(true) override;
					/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					std::uint8_t padE0[0x8];	// E0 - zeroed by InitializeData
					void* unkE8;				// E8 - zeroed by InitializeData
					void* unkF0;				// F0 - zeroed by InitializeData
					std::uint16_t unkF8;		// F8 - zeroed by InitializeData
					std::uint8_t padFA[6];		// FA
					void* unk100;				// 100 - zeroed by InitializeData
					std::uint8_t unk108[0x30];	// 108
				};
				static_assert(offsetof(TESObjectACTI, unkE8) == 0xE8);
				static_assert(sizeof(TESObjectACTI) == 0x138);
			}
		}
	}
}