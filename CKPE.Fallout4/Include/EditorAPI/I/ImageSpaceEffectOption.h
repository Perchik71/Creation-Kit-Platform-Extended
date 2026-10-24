// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/I/ImageSpaceEffect.h>

namespace CKPE::Fallout4::EditorAPI
{
	class ImageSpaceEffectOption : public ImageSpaceEffect
	{
	public:
		/* 0x0B0 */ NiAPI::NiTObjectArray<bool> effectOn;
	};
	static_assert(sizeof(ImageSpaceEffectOption) == 0xC8);
	static_assert(offsetof(ImageSpaceEffectOption, effectOn) == 0xB0);
}
