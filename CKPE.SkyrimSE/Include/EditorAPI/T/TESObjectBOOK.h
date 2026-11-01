// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESModelTextureSwap.h>
#include <EditorAPI/T/TESIcon.h>
#include <EditorAPI/T/TESValueForm.h>
#include <EditorAPI/T/TESWeightForm.h>
#include <EditorAPI/B/TESDescription.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSMessageIcon.h>
#include <EditorAPI/B/BGSPickupPutdownSounds.h>
#include <EditorAPI/B/BGSKeywordForm.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x1E0, vtable RVA 0x30FDC68 (BOOK)
				class TESObjectBOOK :
					public TESBoundObject,				// 000
					public TESFullName,					// 050
					public TESModelTextureSwap,			// 068
					public TESIcon,						// 0B0
					public TESValueForm,				// 0D8
					public TESWeightForm,				// 0E8
					public TESDescription,				// 0F8
					public BGSDestructibleObjectForm,	// 120
					public BGSMessageIcon,				// 130
					public BGSPickupPutdownSounds,		// 160
					public BGSKeywordForm				// 178
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftBook;

					virtual ~TESObjectBOOK() = default;

					/* 070 */ void InitializeData() noexcept(true) override;
					/* 078 */ void ClearData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					std::uint8_t unk190[0x18];	// 190
					void* unk1A8;				// 1A8 - set by the constructor
					void* unk1B0;				// 1B0 - zeroed by InitializeData
					std::uint8_t unk1B8[0x28];	// 1B8
				};
				static_assert(offsetof(TESObjectBOOK, unk1A8) == 0x1A8);
				static_assert(sizeof(TESObjectBOOK) == 0x1E0);
			}
		}
	}
}