// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x28, vtable RVA 0x306F5E0. TESForm with story manager virtuals; adds no members.
				class BGSStoryManagerTreeForm : public TESForm
				{
				public:
					virtual ~BGSStoryManagerTreeForm() = default;
				};
				static_assert(sizeof(BGSStoryManagerTreeForm) == 0x28);
			}
		}
	}
}