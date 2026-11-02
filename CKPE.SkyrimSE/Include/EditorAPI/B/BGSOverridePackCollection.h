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
			// 1.7.99.0: size 0x28, vtable RVA 0x30774E8. Four pointers @08-@20 are zeroed on construction (package list forms; roles not resolved).
			class BGSOverridePackCollection : public BaseFormComponent
			{
			public:
				virtual ~BGSOverridePackCollection() = default;

				void* unk08;	// 08
				void* unk10;	// 10
				void* unk18;	// 18
				void* unk20;	// 20
			};
			static_assert(sizeof(BGSOverridePackCollection) == 0x28);
		}
	}
}