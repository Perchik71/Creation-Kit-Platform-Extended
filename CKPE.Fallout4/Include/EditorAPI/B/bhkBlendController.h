// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/N/NiTimeController.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE::Fallout4::EditorAPI
{
	class bhkBlendController : public NiAPI::NiTimeController
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::bhkBlendController;
		constexpr static Common::ID VTABLE = IDs::VTABLE::bhkBlendController[0];
	};
	static_assert(sizeof(bhkBlendController) == 0x48);
}
