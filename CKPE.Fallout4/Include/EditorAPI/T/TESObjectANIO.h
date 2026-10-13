// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/B/BGSModelMaterialSwap.h>
#include <EditorAPI/B/BSFixedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x80, vtable RVA 0x3093250, 124 virtual functions
				class TESObjectANIO :
					public TESForm,               // 000
					public BGSModelMaterialSwap   // 028
				{
				public:
					virtual ~TESObjectANIO() = default;

				public:
					BSFixedString unloadEventName; // 78
				};
				static_assert(sizeof(TESObjectANIO) == 0x80);
			}
		}
	}
}
