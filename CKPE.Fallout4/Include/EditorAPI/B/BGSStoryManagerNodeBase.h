// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BGSStoryManagerTreeForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.246.0: size 0x58, vtable RVA 0x3014048, 135 virtual functions
				class BGSStoryManagerNodeBase :
					public BGSStoryManagerTreeForm   // 000
				{
				public:
					virtual ~BGSStoryManagerNodeBase() = default;

					virtual void SaveNodeData() noexcept(true) { return; } // 128
					virtual void* GetConditions() noexcept(true) { return nullptr; } // 129 - RVA 0x87CFA0
					[[nodiscard]] virtual bool CanCheckOutChildNodes() const noexcept(true) { return true; } // 130
					virtual bool UpdateChildNodeTrackingData() noexcept(true) { return true; } // 131
					// Likely a node lifecycle hook; inspected overrides are no-ops.
					virtual void sub_420() { return; } // 132 - RVA 0x87BFB0
					virtual void UpdateNodeDataInDialogBox(HWND hDialogBox) noexcept(true) { return; } // 133
					virtual bool PerformNodeDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam,
						LPARAM lParam, LRESULT Result) noexcept(true) { return false; } // 134

				public:
					std::uint8_t unk28[0x18]; // 28
					std::uint8_t unk40[0x18]; // 40
				};
				static_assert(sizeof(BGSStoryManagerNodeBase) == 0x58);
			}
		}
	}
}
