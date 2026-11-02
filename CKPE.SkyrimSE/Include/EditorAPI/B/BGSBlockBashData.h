// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BaseFormComponent.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			// 1.7.99.0: size 0x18, vtable RVA 0x3075AB8. Two pointers @08/@10 are zeroed on construction (form types not resolved).
			class BGSBlockBashData : public BaseFormComponent
			{
			public:
				virtual ~BGSBlockBashData() = default;

				void* unk08;	// 08
				void* unk10;	// 10
			};
			static_assert(sizeof(BGSBlockBashData) == 0x18);
		}
	}
}