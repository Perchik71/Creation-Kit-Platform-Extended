// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BGSGenericCategoryLayout.h>

namespace CKPE::Fallout4::EditorAPI::BGSObjectWindowUtils
{
	class LayoutManager : public BGSGenericCategoryLayout::Manager
	{
	protected:
		/* 0x000 */ void Unk000() override;
		/* 0x008 */ void Unk008() override;
		/* 0x010 */ void PopulateDefaultCategories() override;

	public:
		/* 0x018 */ [[nodiscard]] const char* GetItemColumnTitle() override;
		/* 0x020 */ [[nodiscard]] const char* GetLayoutDirectoryName() override;
		/* 0x028 */ [[nodiscard]] bool UsesColorControls() override;
		/* 0x030 */ [[nodiscard]] const char* GetDialogTitle() override;

	private:
		/* 0x0C8 */ std::uint8_t pad0C8[0x08];
	};
	static_assert(sizeof(LayoutManager) == 0xD0);
}
