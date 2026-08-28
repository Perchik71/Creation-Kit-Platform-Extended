// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "vbits.h"
#include "vmmblock.h"
#include <stddef.h>
#include <stdint.h>
#include <type_traits>

namespace voltek
{
	namespace memory_manager
	{
		// Every size class aims one page body at the same commit target.
		inline constexpr size_t page_body_target_bytes = 4ull * 1024 * 1024;
		// Bitmaps have no tail pass, so a count must be a whole number of 256-bit groups.
		inline constexpr size_t block_count_granularity = 256;
		// bits::find_first_set_bit_avx2 consumes whole 2048-bit chunks and would read past a partial one.
		inline constexpr size_t block_count_scan_granularity = 2048;
		inline constexpr size_t page_map_region_count = 16;
		inline constexpr size_t region_page_map_minimum = 65536;

		// A count the bitmap scans without reading past the page or reporting an index outside it.
		[[nodiscard]] constexpr bool is_valid_block_count(size_t count) noexcept
		{
			return count >= block_count_granularity &&
				(count % block_count_granularity) == 0 &&
				(count < block_count_scan_granularity || (count % block_count_scan_granularity) == 0);
		}

		// Largest valid count not above "count", or 0 when none exists.
		[[nodiscard]] constexpr size_t floor_block_count(size_t count) noexcept
		{
			if (count < block_count_granularity)
				return 0;

			const size_t step = count < block_count_scan_granularity ?
				block_count_granularity : block_count_scan_granularity;
			return (count / step) * step;
		}

		// Smallest valid count not below "count".
		[[nodiscard]] constexpr size_t ceil_block_count(size_t count) noexcept
		{
			if (count <= block_count_granularity)
				return block_count_granularity;

			const size_t step = count <= (block_count_scan_granularity - block_count_granularity) ?
				block_count_granularity : block_count_scan_granularity;
			return ((count + step - 1) / step) * step;
		}

		// Picks the valid count whose page body lands nearest the target.
		[[nodiscard]] constexpr size_t blocks_per_page_for_stride(size_t stride) noexcept
		{
			const size_t ideal = page_body_target_bytes / stride;
			const size_t low = floor_block_count(ideal);
			const size_t high = ceil_block_count(ideal);
			if (!low)
				return high;

			const size_t low_gap = page_body_target_bytes - (low * stride);
			const size_t high_bytes = high * stride;
			const size_t high_gap = high_bytes > page_body_target_bytes ?
				high_bytes - page_body_target_bytes : 0;
			return low_gap <= high_gap ? low : high;
		}

		// Compile-time page geometry for one class of blocks.
		template<typename _type>
		struct page_geometry
		{
			static constexpr size_t stride = sizeof(_type);
			static_assert(stride > 0 && stride <= page_body_target_bytes,
				"block stride must be non-zero and no larger than the page body target");

			static constexpr size_t count = blocks_per_page_for_stride(stride);
			static_assert(is_valid_block_count(count),
				"chosen block count would let the bitmap scan read past the page");
			static_assert(count <= SIZE_MAX / stride,
				"page body size overflows");

			static constexpr size_t body_bytes = count * stride;
		};

		template<typename _type>
		inline constexpr size_t blocks_per_page = page_geometry<_type>::count;

		[[nodiscard]] constexpr bool is_region_page_map_safe(size_t count) noexcept
		{
			return count >= region_page_map_minimum &&
				(count % page_map_region_count) == 0 &&
				is_valid_block_count(count / page_map_region_count);
		}

		template<typename _type>
		inline constexpr bool uses_region_page_map =
			is_region_page_map_safe(blocks_per_page<_type>);

		template<typename _type>
		using page_map = std::conditional_t<
			uses_region_page_map<_type>,
			voltek::core::bits_regions,
			voltek::core::bits>;

		// One page body per size class, the commit the allocator retains once every pool is warm.
		inline constexpr size_t all_page_bodies_bytes =
			page_geometry<block8_t>::body_bytes +
			page_geometry<block16_t>::body_bytes +
			page_geometry<block32_t>::body_bytes +
			page_geometry<block64_t>::body_bytes +
			page_geometry<block128_t>::body_bytes +
			page_geometry<block256_t>::body_bytes +
			page_geometry<block512_t>::body_bytes +
			page_geometry<block1024_t>::body_bytes +
			page_geometry<block4096_t>::body_bytes +
			page_geometry<block8192_t>::body_bytes +
			page_geometry<block16384_t>::body_bytes +
			page_geometry<block32768_t>::body_bytes +
			page_geometry<block65536_t>::body_bytes +
			page_geometry<block131072_t>::body_bytes;

		// 100.5078125 MiB; a different target or block layout moves this.
		static_assert(all_page_bodies_bytes == 105390080,
			"aggregate one-page commit changed");
	}
}
