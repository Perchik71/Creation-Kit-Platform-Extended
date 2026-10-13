// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/T/TESModel.h>
#include <EditorAPI/B/BGSIdleCollection.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSKeyword;

				// 1.11.240.0: size 0x118, vtable RVA 0x3037160, 154 virtual functions
				class BGSIdleMarker :
					public TESBoundObject,     // 000
					public BGSKeywordForm,     // 090
					public TESModel,           // 0B0
					public BGSIdleCollection   // 0E8
				{
				public:
					virtual ~BGSIdleMarker() = default;

				public:
					BGSKeyword* animArchType; // 108
					BGSKeyword* flavorAnim; // 110
				};
				static_assert(sizeof(BGSIdleMarker) == 0x118);
			}
		}
	}
}
