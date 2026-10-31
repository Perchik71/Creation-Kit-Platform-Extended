// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESLeveledList.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0xB8, vtable RVA 0x308EB50, 154 virtual functions
				class TESLevSpell :
					public TESBoundObject,  // 000
					public TESLeveledList   // 090
				{
				public:
					virtual ~TESLevSpell() = default;

					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftLeveledSpell;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESLevSpell[0];
				};
				static_assert(sizeof(TESLevSpell) == 0xB8);
			}
		}
	}
}
