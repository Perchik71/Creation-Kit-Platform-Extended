// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkInfrastructure.h>

namespace CKPE::Fallout4::EditorAPI
{
	class bhkThreadPool : public hkThreadPool
	{
	public:
		/* 0x010 */ std::byte unk010[0x28];
	};
	static_assert(sizeof(bhkThreadPool) == 0x38);
	static_assert(offsetof(bhkThreadPool, unk010) == 0x10);
}
