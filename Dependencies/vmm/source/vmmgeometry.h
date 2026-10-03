// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "vmmblock.h"
#include <stddef.h>
#include <stdint.h>

namespace voltek
{
	namespace memory_manager
	{
		// Smaller pages strand less memory behind a few live blocks; retention absorbs the extra churn.
		inline constexpr size_t page_body_target_bytes = 1024ull * 1024;
		// The largest classes still get several blocks per page.
		inline constexpr size_t minimum_blocks_per_page = 8;
		struct class_geometry
		{
			size_t limit;
			size_t stride;
			size_t count;
			size_t body_bytes;
			size_t retained_pages;
			size_t cache_cap;
			size_t cache_batch;
		};
	}
}
