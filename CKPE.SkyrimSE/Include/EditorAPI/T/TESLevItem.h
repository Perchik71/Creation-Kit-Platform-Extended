// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/B/TESLeveledList.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x70, vtable RVA 0x30F7128
				class TESLevItem :
					public TESBoundObject,	// 00
					public TESLeveledList	// 50
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftLeveledItem;

					virtual ~TESLevItem() = default;

				/* 080 */ bool Load(TESFile* File) noexcept(true) override;
				/* 098 */ void Save() noexcept(true) override;
				/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
				/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
				/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
				};
				static_assert(sizeof(TESLevItem) == 0x70);
			}
		}
	}
}