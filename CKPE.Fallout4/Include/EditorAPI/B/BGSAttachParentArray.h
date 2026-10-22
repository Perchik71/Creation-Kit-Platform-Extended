// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BaseFormComponent.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSKeyword;
			}

			// 1.11.240.0: size 0x20, vtable RVA 0x2FA5CE0, 21 virtual functions
			class BGSAttachParentArray :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~BGSAttachParentArray() = default;

			public:
				Forms::BGSKeyword** array; // 08 - array of keyword pointers (8 byte elements; the game stores 16 bit keyword indices)
				std::uint32_t size; // 10
				std::uint32_t pad14; // 14
				std::uint32_t listControlID; // 18 - CK only, dialog list view control ID ("Editor ID" / "ID" / "Group" columns)
				std::uint32_t pad1C; // 1C
			};
			static_assert(sizeof(BGSAttachParentArray) == 0x20);
		}
	}
}
