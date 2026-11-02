// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BaseFormComponent.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			// 1.7.99.0: size 0x08, vtable RVA 0x30FAB58. Holds no data.
			class BGSOpenCloseForm : public BaseFormComponent
			{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::BGSOpenCloseForm;
				constexpr static Common::ID VTABLE = IDs::VTABLE::BGSOpenCloseForm[0];

				virtual ~BGSOpenCloseForm() = default;
			};
			static_assert(sizeof(BGSOpenCloseForm) == 0x08);
		}
	}
}