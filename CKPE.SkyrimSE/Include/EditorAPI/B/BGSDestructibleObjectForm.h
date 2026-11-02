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
			class BGSDestructibleObjectForm : public BaseFormComponent
			{
				// members
				void* data;
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::BGSDestructibleObjectForm;
				constexpr static Common::ID VTABLE = IDs::VTABLE::BGSDestructibleObjectForm[0];

				virtual ~BGSDestructibleObjectForm() = default;
			};
			static_assert(sizeof(BGSDestructibleObjectForm) == 0x10);
		}
	}
}