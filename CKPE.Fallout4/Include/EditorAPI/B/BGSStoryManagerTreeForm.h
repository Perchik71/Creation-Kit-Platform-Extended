// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.246.0: size 0x28, vtable RVA 0x3013B70, 128 virtual functions
				class BGSStoryManagerTreeForm :
					public TESForm   // 000
				{
				public:
					virtual ~BGSStoryManagerTreeForm() = default;

					// Likely a tree lifecycle hook; inspected overrides are no-ops.
					virtual void sub_3E0() { return; } // 124 - RVA 0x87BFA0
					virtual void ReadNodeDataFromDialogBox(HWND hDialogBox) noexcept(true) { return; } // 125
					[[nodiscard]] virtual std::uint32_t QChildCount() const noexcept(true) { return 0; } // 126
					[[nodiscard]] virtual BGSStoryManagerTreeForm* GetChild(std::uint32_t Index) const noexcept(true) { return nullptr; } // 127
				};
				static_assert(sizeof(BGSStoryManagerTreeForm) == 0x28);
			}
		}
	}
}
