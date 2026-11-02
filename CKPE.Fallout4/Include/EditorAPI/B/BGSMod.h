// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BaseFormComponent.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace BGSMod
			{
				class Container
				{
				private:
					char pad0[0x8];
				};
				static_assert(sizeof(Container) == 0x8);

				namespace Template
				{
					class Items : public BaseFormComponent
					{
					public:
						constexpr static Common::ID RTTI = IDs::RTTI::BGSMod__Template__Items;
						constexpr static Common::ID VTABLE = IDs::VTABLE::BGSMod__Template__Items[0];

						/* 0x008 */ BSTArray<void*> items;
					};
					static_assert(sizeof(Items) == 0x20);
					static_assert(offsetof(Items, items) == 0x8);
				}
			}
		}
	}
}
