// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/N/NiTimeController.h>

namespace CKPE::Fallout4::EditorAPI
{
	class bhkBlendController : public NiAPI::NiTimeController {};
	static_assert(sizeof(bhkBlendController) == 0x48);
}
