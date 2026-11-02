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
			// 1.7.99.0: size 0x10, vtable RVA 0x3075480. Pointer @08 is set by a helper constructor (layout not resolved).
			class BGSAttackDataForm : public BaseFormComponent
			{
			public:
				virtual ~BGSAttackDataForm() = default;

				void* unk08;	// 08
			};
			static_assert(sizeof(BGSAttackDataForm) == 0x10);
		}
	}
}