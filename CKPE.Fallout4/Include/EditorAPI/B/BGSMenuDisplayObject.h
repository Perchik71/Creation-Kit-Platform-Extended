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
				class TESBoundObject;
			}

			// 1.11.240.0: size 0x10, vtable RVA 0x3019CC0, 22 virtual functions
			class BGSMenuDisplayObject :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~BGSMenuDisplayObject() = default;

				constexpr static Common::ID VTABLE = IDs::VTABLE::BGSMenuDisplayObject[0];

				virtual Forms::TESBoundObject* GetMenuDispObject() const noexcept(true) { return nullptr; } // 21 - RVA 0x899710, return (const char *)*((_QWORD *)this + 1);
			public:
				Forms::TESBoundObject* menuDispObject; // 08
			};
			static_assert(sizeof(BGSMenuDisplayObject) == 0x10);
		}
	}
}
