// Copyright © 2024-2026 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "IMemoryStore.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			enum class MEM_CONTEXT;

			class __declspec(novtable) IMemoryHeap :
				public IMemoryStore  // 00
			{
			public:
				virtual ~IMemoryHeap() = default;

				// override (IMemoryStoreBase)
				virtual bool	ContainsBlockImpl(const void* a_block) const override { return BlockInHeap(a_block); }								// 0D
				// override (IMemoryStore)
				virtual void* AllocateAlignImpl(std::size_t a_size, std::uint32_t a_alignment) override { return Allocate(a_size, a_alignment); }	// 0E
				virtual void	DeallocateAlignImpl(void*& a_block) override { Deallocate(a_block, 0); }											// 0F

				// add
				virtual const char*		GetName() const = 0;													// 11
				virtual void*			Allocate(std::size_t a_size, std::uint32_t a_alignment) = 0;			// 12
				virtual void			Deallocate(void* a_mem, std::uint32_t) = 0;								// 13
				virtual bool			BlockInHeap(const void* a_block) const = 0;								// 14
				virtual std::size_t		TotalSize(const void* a_block) const = 0;								// 15
				virtual void			GetHeapStats(void* a_stats, bool a_fullBlockInfo) = 0;					// 16
			private:
				virtual bool			ReturnSomethingFlag_17() const = 0;										// 17
				virtual void			DumpMaybe_18() const = 0;												// 18
				virtual std::uint32_t	Something_19() const = 0;												// 19
			public:
				virtual bool			ShouldTrySmallBlockPools(std::size_t a_size, bool a_context) = 0;		// 1A
				virtual std::uint32_t	GetPageSize() const = 0;												// 1B
			};
			static_assert(sizeof(IMemoryHeap) == 0x8);
		}
	}
}
