// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/B/BGSPreloadable.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x40, vtable RVA 0x307FC28, 124 virtual functions
				class BGSDebris :
					public TESForm,         // 000
					public BGSPreloadable   // 028
				{
				public:
					virtual ~BGSDebris() = default;

					constexpr static std::uint8_t TYPE_ID = ftDebris;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSDebris[0];
				public:
					BSSimpleList<void*> dataList; // 30 - BSSimpleList<BGSDebrisData*>
				};
				static_assert(sizeof(BGSDebris) == 0x40);
			}
		}
	}
}
