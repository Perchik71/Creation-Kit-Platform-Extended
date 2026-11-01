// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/B/TESLeveledList.h>
#include <EditorAPI/T/TESModelTextureSwap.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0xB8, vtable RVA 0x30F66E8
				class TESLevCharacter :
					public TESBoundObject,			// 00
					public TESLeveledList,			// 50
					public TESModelTextureSwap		// 70
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftLeveledCharacter;

					virtual ~TESLevCharacter() = default;

				/* 080 */ bool Load(TESFile* File) noexcept(true) override;
				/* 098 */ void Save() noexcept(true) override;
				/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
				/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
				/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
				};
				static_assert(sizeof(TESLevCharacter) == 0xB8);
			}
		}
	}
}