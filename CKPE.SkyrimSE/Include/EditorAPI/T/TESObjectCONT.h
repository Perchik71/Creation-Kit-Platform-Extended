// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESModelTextureSwap.h>
#include <EditorAPI/B/TESContainer.h>
#include <EditorAPI/T/TESWeightForm.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSOpenCloseForm.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x118, vtable RVA 0x30FEE78
				class TESObjectCONT :
					public TESBoundObject,				// 00
					public TESContainer,				// 50
					public TESFullName,					// 68
					public TESModelTextureSwap,			// 80
					public TESWeightForm,				// C8
					public BGSDestructibleObjectForm,	// D8
					public BGSOpenCloseForm				// E8
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftContainer;

					virtual ~TESObjectCONT() = default;

					/* 070 */ void InitializeData() noexcept(true) override;
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
					std::uint8_t unkF0;			// F0
					std::uint8_t unkF1;			// F1 - zeroed by InitializeData
					std::uint8_t padF2[6];		// F2
					void* unkF8;				// F8 - zeroed by InitializeData (SNAM / QNAM, order not resolved)
					void* unk100;				// 100 - zeroed by InitializeData
					std::uint8_t unk108[0x10];	// 108
				};
				static_assert(offsetof(TESObjectCONT, unkF8) == 0xF8);
				static_assert(sizeof(TESObjectCONT) == 0x118);
			}
		}
	}
}