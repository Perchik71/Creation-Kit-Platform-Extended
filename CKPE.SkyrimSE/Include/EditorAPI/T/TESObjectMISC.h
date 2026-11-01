// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESModelTextureSwap.h>
#include <EditorAPI/T/TESIcon.h>
#include <EditorAPI/T/TESValueForm.h>
#include <EditorAPI/T/TESWeightForm.h>
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
				// 1.7.99.0: size 0x178, vtable RVA 0x3100980 (MISC)
				class TESObjectMISC :
					public TESBoundObject,				// 000
					public TESFullName,					// 050
					public TESModelTextureSwap,			// 068
					public TESIcon,						// 0B0
					public TESValueForm,				// 0D8
					public TESWeightForm,				// 0E8
					public BGSDestructibleObjectForm,	// 0F8
					public BGSMessageIcon,				// 108
					public BGSPickupPutdownSounds,		// 138
					public BGSKeywordForm				// 150
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftMisc;

					virtual ~TESObjectMISC() = default;

					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					std::uint8_t unk168;		// 168 - 0 on construction
					std::uint8_t unk169;		// 169 - 1 on construction
					std::uint8_t pad16A[6];		// 16A
					void* unk170;				// 170 - initialised by a helper constructor (type unresolved)
				};
				static_assert(offsetof(TESObjectMISC, unk168) == 0x168);
				static_assert(sizeof(TESObjectMISC) == 0x178);
			}
		}
	}
}