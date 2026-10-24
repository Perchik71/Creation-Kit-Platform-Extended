// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSStringCache.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			typedef BSStringCache::Ref BSFixedString;
			static_assert(sizeof(BSFixedString) == 0x8);

			class BSAutoFixedString : public BSFixedString
			{
			public:
				BSAutoFixedString() : BSFixedString() { }
				BSAutoFixedString(const char* buf) : BSFixedString(buf) { }
				~BSAutoFixedString() { release(this); }
			};
			static_assert(sizeof(BSAutoFixedString) == 0x8);
		}
	}
}
