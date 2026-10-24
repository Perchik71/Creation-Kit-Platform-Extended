// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BSExtraData.h>

namespace CKPE::Fallout4::EditorAPI
{
	class DialogExtraWorkingFile : public BSExtraData
	{
	public:
		enum : std::uint8_t { kDialogExtraType = 0x22 };
		/* 0x000 */ virtual ~DialogExtraWorkingFile();

		/* 0x018 */ char FileName[260];
		/* 0x11C */ std::uint8_t pad11C[0x04];
	};
	static_assert(sizeof(DialogExtraWorkingFile) == 0x120);
	static_assert(offsetof(DialogExtraWorkingFile, FileName) == 0x18);
}
