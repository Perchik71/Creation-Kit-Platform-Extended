#pragma once

#include <EditorAPI/I/IMemoryStore.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			struct HeapStats
			{
			public:
				// members
				const char*		heapName;              // 00
				std::size_t		memHeapSize;           // 08
				std::size_t		memHeapCommitted;      // 10
				std::size_t		memAllocatedToBlocks;  // 18
				std::int32_t	numBlocks;             // 20
				std::int32_t	numFreeBlocks;         // 24
				std::size_t		memFreeInBlocks;       // 28
				std::size_t		memUsedInBlocks;       // 30
				std::size_t		smallestFreeBlock;     // 38
				std::size_t		largestFreeBlock;      // 40
				std::size_t		heapOverhead;          // 48
				std::size_t		freeListOverhead;      // 50
				std::size_t		blockOverhead;         // 58
				std::size_t		totalFree;             // 60
			};
			static_assert(sizeof(HeapStats) == 0x68);

			enum class MEM_CONTEXT;

			class __declspec(novtable) IMemoryHeap :
				public IMemoryStore  // 00
			{
			public:
				virtual ~IMemoryHeap() = default;

				// override (IMemoryStoreBase)
				virtual bool			ContainsBlockImpl(const void* a_block) const override { return BlockInHeap(a_block); }								// 0D
				// override (IMemoryStore)
				virtual void*			AllocateAlignImpl(std::size_t a_size, std::uint32_t a_alignment) override { return Allocate(a_size, a_alignment); }	// 0E
				virtual void			DeallocateAlignImpl(void*& a_block) override { Deallocate(a_block, 0); }											// 0F

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
