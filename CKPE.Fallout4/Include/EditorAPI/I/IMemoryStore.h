// Copyright © 2024-2026 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include "IMemoryStoreBase.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class __declspec(novtable) IMemoryStore :
				public IMemoryStoreBase  // 0
			{
			public:
				virtual ~IMemoryStore() = default;																									// 00

				// add
				virtual void* AllocateAlignImpl(std::size_t a_size, std::uint32_t a_alignment) = 0;													// 0E
				virtual void  DeallocateAlignImpl(void*& a_block) = 0;																				// 0F
				virtual void* TryAllocateImpl([[maybe_unused]] std::size_t a_size, [[maybe_unused]] std::uint32_t a_alignment) { return nullptr; }	// 10
			};
			static_assert(sizeof(IMemoryStore) == 0x8);
		}
	}
}
