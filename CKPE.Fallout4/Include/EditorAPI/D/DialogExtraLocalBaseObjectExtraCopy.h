// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/D/DialogExtraLocalBaseObjectCopy.h>

namespace CKPE::Fallout4::EditorAPI
{
	class DialogExtraLocalBaseObjectExtraCopy : public DialogExtraLocalBaseObjectCopy
	{
	public:
		/* 0x000 */ virtual ~DialogExtraLocalBaseObjectExtraCopy();
	};
	static_assert(sizeof(DialogExtraLocalBaseObjectExtraCopy) == 0x20);
}
