// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <EditorAPI/BSStringCache.h>

#pragma once

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			std::uint16_t BSStringCache::Entry::acquire() noexcept(true)
			{
				CKPE_ASSERT_MSG(((flags & kState_RefcountMask) < kState_RefcountMask),
					"BSStringCache::Entry string is about to cause refcount wraparound to 0.");
				return InterlockedIncrement16((SHORT*)&flags);
			}
		}
	}
}