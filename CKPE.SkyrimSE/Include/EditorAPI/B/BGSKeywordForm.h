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
			namespace Forms { class BGSKeyword; }

			// 1.7.99.0: size 0x18, vtable RVA 0x3076BA8
			class BGSKeywordForm : public BaseFormComponent
			{
			public:
				virtual ~BGSKeywordForm() = default;

				Forms::BGSKeyword** keywords;			// 08
				std::uint32_t numKeywords;		// 10
				std::uint32_t pad14;			// 14
			};
			static_assert(offsetof(BGSKeywordForm, keywords) == 0x08);
			static_assert(offsetof(BGSKeywordForm, numKeywords) == 0x10);
			static_assert(sizeof(BGSKeywordForm) == 0x18);
		}
	}
}