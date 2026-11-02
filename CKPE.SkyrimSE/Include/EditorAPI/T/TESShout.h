// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/BGSMenuDisplayObject.h>
#include <EditorAPI/B/BGSEquipType.h>
#include <EditorAPI/B/TESDescription.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0xD0, vtable RVA 0x30E5E70
				class TESShout :
					public TESForm,					// 00
					public TESFullName,				// 28
					public BGSMenuDisplayObject,	// 40
					public BGSEquipType,			// 50
					public TESDescription			// 60
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftShout;

					virtual ~TESShout() = default;

					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 248 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 250 */ [[nodiscard]] bool CanCheckIn(const void* Forms) const noexcept(true) override;
					/* 258 */ void BuildCheckInDependencyMessage(const void* Forms, void* Message) const noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					std::uint8_t unk88[0x48];	// 88 - data (SNAM word entries), layout not resolved
				};
				static_assert(offsetof(TESShout, unk88) == 0x88);
				static_assert(sizeof(TESShout) == 0xD0);
			}
		}
	}
}