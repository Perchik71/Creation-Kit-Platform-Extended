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
			// 1.7.99.0: size 0x178, vtable RVA 0x3079E50. Two 0x48 byte model records start @08 (constructed by an array constructor); the rest is not resolved.
			class TESBipedModelForm : public BaseFormComponent
			{
			public:
				virtual ~TESBipedModelForm() = default;

				std::uint8_t unk08[0x170];	// 08
			};
			static_assert(sizeof(TESBipedModelForm) == 0x178);
		}
	}
}