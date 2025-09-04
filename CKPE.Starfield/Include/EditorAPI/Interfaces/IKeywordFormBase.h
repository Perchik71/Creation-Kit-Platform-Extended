// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSKeyword;
			}

			namespace Interfaces
			{
				class IKeywordFormBase
				{
				public:
					virtual ~IKeywordFormBase();
					virtual bool HasKeyword(const Forms::BGSKeyword* keyword, const void* data);
					virtual void CollectAllKeywords(void* refOutKeywordA, const void* data);
				};
				static_assert(sizeof(IKeywordFormBase) == 0x8);
			}
		}
	}
}