// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// 1.11.246.0: bstbtree.h. Ordered set of T kept in 0x80 byte nodes of six keys; the tree object doubles as the node allocator.
			// Only the header is mapped: the form code passes it around (check-in dependency walks, reference collection, resource file
			// collection) and inserts through native helpers.
			template <class T>
			class BSTBTree
			{
			public:
				struct Node
				{
					std::uint8_t data[0x80]; // keys 00-2F (6 x 8 bytes), link at 30, link at 38, key count at 68 (nodes taken from the free list link at 30)
				};
				static_assert(sizeof(Node) == 0x80);

				[[nodiscard]] std::uint32_t size() const noexcept(true) { return count; }
				[[nodiscard]] bool empty() const noexcept(true) { return count == 0; }

			public:
				std::uint64_t unk00; // 00 - allocator state
				std::uint32_t count; // 08 - number of keys
				std::uint32_t capacity; // 0C - total key slots, grows by 6 per node allocated
				Node* root; // 10
				Node* freeNodes; // 18 - recycled nodes, chained through the link at +30
			};
		}
	}
}
