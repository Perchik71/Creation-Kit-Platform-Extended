// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <windows.h>

namespace CKPE::Fallout4::EditorAPI
{
	// The nonpolymorphic state stored in the TREELIST_PTR window property.
	struct TreeListControl
	{
		/* 0x000 */ HINSTANCE Instance;
		/* 0x008 */ HWND ParentWindow;
		/* 0x010 */ HWND TreeWindow;
		/* 0x018 */ HWND HeaderWindow;
		/* 0x020 */ std::uint8_t pad020[0x08];
		/* 0x028 */ HFONT TreeFont;
		/* 0x030 */ HFONT HeaderFont;
		/* 0x038 */ HFONT AlternateFont;
		/* 0x040 */ std::uint8_t pad040[0x108];
		/* 0x148 */ WNDPROC OriginalTreeProc;
		/* 0x150 */ WNDPROC OriginalParentProc;
		/* 0x158 */ std::uint8_t pad158[0x260];
	};
	static_assert(sizeof(TreeListControl) == 0x3B8);
	static_assert(offsetof(TreeListControl, TreeWindow) == 0x10);
	static_assert(offsetof(TreeListControl, HeaderWindow) == 0x18);
	static_assert(offsetof(TreeListControl, OriginalTreeProc) == 0x148);
}
