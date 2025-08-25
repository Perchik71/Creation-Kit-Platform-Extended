// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <EditorAPI/BSStringCache.h>

#pragma once

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			std::uint32_t BSStringCache::Entry::acquire() noexcept(true)
			{
				return InterlockedIncrement(&refCount);
			}
		}
	}
}