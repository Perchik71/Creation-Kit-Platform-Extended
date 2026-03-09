#pragma once

#include <Windows.h>
#include <CKPE.Utils.h>
#include "../I/IMemoryStore.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class __declspec(novtable) ScrapHeap :
				public IMemoryStore		// 00
			{
			public:
				inline static std::uintptr_t RTTI;
				inline static std::uintptr_t VTABLE;

				struct Block
				{
					std::size_t	sizeFlags;	// 00
					Block*		prev;		// 08
				};
				static_assert(sizeof(Block) == 0x10);

				struct FreeBlock :
					public Block		// 00
				{
					FreeBlock* left;	// 10
					FreeBlock* right;	// 18
				};
				static_assert(sizeof(FreeBlock) == 0x20);

				struct FreeTreeNode :
					public FreeBlock	// 00
				{
					FreeTreeNode**	root;			// 20
					FreeTreeNode*	leftNode;		// 28
					FreeTreeNode*	rightNode;		// 30
					std::size_t		parentAndBlack;	// 38
				};
				static_assert(sizeof(FreeTreeNode) == 0x40);

				virtual ~ScrapHeap() override { VirtualFree(baseAddress, 0, MEM_RELEASE); }																	// 00

				// override (IMemoryStoreBase)
				virtual std::size_t	Size(void const* a_mem) const override { return *static_cast<const std::size_t*>(a_mem) & ~(std::size_t{ 3 } << 62); }	// 01
				virtual void		GetMemoryStats(MemoryStats*) override { return; }																		// 02
				virtual bool		ContainsBlockImpl(const void* a_block) const override { return baseAddress <= a_block && a_block <= endAddress; }		// 0D
				// override (IMemoryStore)
				virtual void*		AllocateAlignImpl(std::size_t a_size, std::uint32_t a_alignment) override { return Allocate(a_size, a_alignment); }		// 0E
				virtual void		DeallocateAlignImpl(void*& a_block) override { Deallocate(a_block), a_block = nullptr; }								// 0F

				void* Allocate(std::size_t a_size, std::size_t a_alignment) const noexcept(true)
				{
					return fast_call<void*>(VTABLE + 0x70, this, a_size, a_alignment);
				}

				void Deallocate(void* a_mem) const noexcept(true)
				{
					return fast_call<void>(VTABLE + 0x78, this, a_mem);
				}

			private:
				FreeBlock*		smallBlocks[6]{ nullptr };		// 08
				FreeTreeNode*	freeList{ nullptr };			// 38
				Block*			lastBlock{ nullptr };			// 40
				std::byte*		baseAddress{ nullptr };			// 48
				std::byte*		endAddress{ nullptr };			// 50
				std::byte*		commitEnd{ nullptr };			// 58
				std::size_t		reserveSize;					// 60
				std::size_t		minCommit{ 1u << 17 };			// 68
				std::size_t		totalAllocated{ 0 };			// 70
				std::uint32_t	keepPagesRequest{ 0 };			// 78
				std::uint32_t	totalFreeBlocks{ 0 };			// 7C
				std::uint32_t	freeSmallBlocks{ 0 };			// 80
				std::uint32_t	totalAllocatedBlocks{ 0 };		// 84
				std::uint32_t	pmpBarrier{ 0 };				// 88
			};
			static_assert(sizeof(ScrapHeap) == 0x90);
		}
	}
}
