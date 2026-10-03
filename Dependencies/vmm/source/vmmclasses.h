// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "vmmgeometry.h"
#include <array>

namespace voltek
{
	namespace memory_manager
	{
		inline constexpr auto pool_limits = [] {
			std::array<size_t, 48> limits{};
			size_t index = 0;
			for (size_t size = 16; size <= 128; size += 16)
				limits[index++] = size;
			for (size_t base = 128; base < 131072; base *= 2)
				for (size_t step = 5; step <= 8; ++step)
					limits[index++] = base * step / 4;
			return limits;
		}();
		inline constexpr size_t pool_count = pool_limits.size();
		inline constexpr size_t pool_limit_maximum = pool_limits.back();
		static_assert(pool_count <= UINT8_MAX);

		inline constexpr auto class_geometries = [] {
			std::array<class_geometry, pool_count> geometries{};
			for (size_t index = 0; index < pool_count; ++index)
			{
				const auto limit = pool_limits[index];
				const auto stride = sizeof(block_base) + limit;
				const auto count = page_body_target_bytes / stride < minimum_blocks_per_page ?
					minimum_blocks_per_page : page_body_target_bytes / stride;
				geometries[index] = { limit, stride, count, count * stride, 1 };
				if (limit <= 4096)
				{
					const auto cap = 16 * 1024 / stride < 64 ? 16 * 1024 / stride : 64;
					geometries[index].cache_cap = cap;
					geometries[index].cache_batch = cap / 2 > 0 ? cap / 2 : 1;
				}
			}
			return geometries;
		}();

		inline constexpr size_t cached_pool_count = [] {
			size_t count = 0;
			for (const auto& geometry : class_geometries)
				count += geometry.cache_cap != 0;
			return count;
		}();

		static_assert([] {
			for (size_t index = 0; index < pool_count; ++index)
			{
				const auto& geometry = class_geometries[index];
				if (geometry.stride != 16 + geometry.limit || geometry.stride % 16 ||
					geometry.count > UINT32_MAX || geometry.cache_cap > 64 ||
					(index < cached_pool_count) != (geometry.cache_cap != 0))
					return false;
			}
			return true;
		}());
		static_assert([] {
			for (size_t index = 0; index < pool_count; ++index)
				if ((index && pool_limits[index] <= pool_limits[index - 1]) ||
					pool_limits[index] % (pool_limits[index] <= 1024 ? 16 : 256))
					return false;
			return true;
		}());

		template<size_t Step, size_t Maximum>
		constexpr auto make_pool_lookup()
		{
			std::array<uint8_t, Maximum / Step + 1> lookup{};
			size_t id = 0;
			for (size_t index = 0; index < lookup.size(); ++index)
			{
				while (pool_limits[id] < index * Step)
					++id;
				lookup[index] = static_cast<uint8_t>(id);
			}
			return lookup;
		}

		inline constexpr auto small_pool_lookup = make_pool_lookup<16, 1024>();
		inline constexpr auto large_pool_lookup = make_pool_lookup<256, pool_limit_maximum>();

		// Callers handle zero-size and large blocks before dispatch.
		[[nodiscard]] constexpr size_t pool_class_of(size_t size) noexcept
		{
			return size <= 1024 ? small_pool_lookup[(size + 15) >> 4] :
				large_pool_lookup[(size + 255) >> 8];
		}

		inline constexpr size_t all_page_bodies_bytes = [] {
			size_t bytes = 0;
			for (const auto& geometry : class_geometries)
				bytes += geometry.body_bytes;
			return bytes;
		}();
	}
}
