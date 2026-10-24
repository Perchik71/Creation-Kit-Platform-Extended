// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <windows.h>
#include <commctrl.h>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE::Fallout4::EditorAPI::BGSGenericCategoryLayout
{
	class Manager
	{
	protected:
		// This interface has no virtual destructor.
		/* 0x000 */ virtual void Unk000() = 0;
		/* 0x008 */ virtual void Unk008() = 0;
		/* 0x010 */ virtual void PopulateDefaultCategories() = 0;

	public:
		/* 0x018 */ [[nodiscard]] virtual const char* GetItemColumnTitle() = 0;
		/* 0x020 */ [[nodiscard]] virtual const char* GetLayoutDirectoryName() = 0;
		/* 0x028 */ [[nodiscard]] virtual bool UsesColorControls();
		/* 0x030 */ [[nodiscard]] virtual const char* GetDialogTitle() = 0;

		/* 0x008 */ HWND WindowHandle;
		/* 0x010 */ HIMAGELIST ImageList;
		/* 0x018 */ BSTArray<BSFixedString> ItemNames;
		/* 0x030 */ BSTArray<BSFixedString> CategoryNames;
		/* 0x048 */ std::uint8_t pad048[0x70];
		/* 0x0B8 */ BSFixedString LayoutFileName;
		/* 0x0C0 */ bool Modified;
		/* 0x0C1 */ bool Initialized;
		/* 0x0C2 */ std::uint8_t pad0C2[0x06];
	};
	static_assert(sizeof(Manager) == 0xC8);
	static_assert(offsetof(Manager, WindowHandle) == 0x08);
	static_assert(offsetof(Manager, ItemNames) == 0x18);
	static_assert(offsetof(Manager, CategoryNames) == 0x30);
	static_assert(offsetof(Manager, LayoutFileName) == 0xB8);
	static_assert(offsetof(Manager, Modified) == 0xC0);
}
