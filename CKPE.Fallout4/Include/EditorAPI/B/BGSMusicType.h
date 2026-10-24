// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/F/FormInterfaces.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x78, vtable RVA 0x3060568, 124 virtual functions
				class BGSMusicType :
					public TESForm,       // 000
					public BSIMusicType   // 028
				{
				public:
					virtual ~BGSMusicType() = default;

				};
				static_assert(sizeof(BGSMusicType) == 0x78);
				static_assert(offsetof(BGSMusicType, flags) == 0x30);
				static_assert(offsetof(BGSMusicType, tracks) == 0x58);
			}
		}
	}
}
