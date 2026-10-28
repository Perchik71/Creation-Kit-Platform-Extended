// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BaseFormComponent.h>
#include <EditorAPI/B/BSTList.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class GROUP_REACTION;

			// 1.11.240.0: size 0x20, vtable RVA 0x3021F00, 21 virtual functions
			class TESReactionForm :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~TESReactionForm() = default;

				constexpr static Common::ID VTABLE = IDs::VTABLE::TESReactionForm[0];
			public:
				BSSimpleList<GROUP_REACTION*> reactionList; // 08
				std::int8_t groupFormType; // 18
				std::uint8_t pad19[0x7]; // 19
			};
			static_assert(sizeof(TESReactionForm) == 0x20);
		}
	}
}
