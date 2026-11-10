#pragma once

#include <EditorAPI/IDs.h>
#include <EditorAPI/I/IMemoryHeap.h>
#include <CKPE.Utils.h>
#include <CKPE.Common.Relocation.h>
#include <Windows.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			class ScrapHeap : public IMemoryStore
			{
			public:
				constexpr static auto RTTI = IDs::RTTI::ScrapHeap;
				constexpr static auto VTABLE = IDs::VTABLE::ScrapHeap[0];

				struct Block
				{
				public:
					// members
					std::size_t sizeFlags;	// 00
					Block*		prev;		// 08
				};
				static_assert(sizeof(Block) == 0x10);

				struct FreeBlock : public Block
				{
				public:
					// members
					FreeBlock* left;   // 10
					FreeBlock* right;  // 18
				};
				static_assert(sizeof(FreeBlock) == 0x20);

				struct FreeTreeNode : public Block
				{
				public:
					// members
					FreeTreeNode**	root;				// 10
					FreeTreeNode*	leftNode;			// 18
					FreeTreeNode*	rightNode;			// 20
					std::size_t		parentAndBlack;		// 28
				};
				static_assert(sizeof(FreeTreeNode) == 0x30);

				virtual ~ScrapHeap() override { VirtualFree(baseAddress, 0, MEM_RELEASE); }

				// override (IMemoryStoreBase)
				std::size_t Size(void const* a_mem) const override { return *static_cast<const std::size_t*>(a_mem) & ~(std::size_t{ 3 } << 62); }
				void        GetMemoryStats(MemoryStats*) override { return; }
				bool        ContainsBlockImpl(const void* a_block) const override { return baseAddress <= a_block && a_block <= endAddress; }
				// override (IMemoryStore)
				void*		AllocateAlignImpl(std::size_t a_size, std::uint32_t a_alignment) override { return Allocate(a_size, a_alignment); }
				void        DeallocateAlignImpl(void*& a_block) override { Deallocate(a_block), a_block = nullptr; }

				void* Allocate(std::size_t a_size, std::size_t a_alignment) const noexcept(true)
				{
					using func_t = void* (*)(const ScrapHeap*, std::size_t, std::uint32_t);
					static Common::Relocation<func_t> func{ IDs::ScrapHeap::Allocate };
					return func(this, a_size, static_cast<std::uint32_t>(a_alignment));
				}

				void  Deallocate(void* a_mem) const noexcept(true)
				{
					using func_t = void (*)(const ScrapHeap*, void*);
					static Common::Relocation<func_t> func{ IDs::ScrapHeap::Deallocate };
					func(this, a_mem);
				}

				// members
				FreeBlock*		smallBlocks[6]{ nullptr };  // 08
				FreeTreeNode*	freeList{ nullptr };        // 38
				Block*			lastBlock{ nullptr };       // 40
				void*			baseAddress{ nullptr };     // 48
				void*			endAddress{ nullptr };      // 50
				void*			commitEnd{ nullptr };       // 58
				std::size_t		reserveSize{ 1 << 26 };     // 60
				std::size_t		minCommit{ 1 << 17 };       // 68
				std::size_t		totalAllocated{ 0 };        // 70
				std::uint32_t	keepPagesRequest{ 0 };      // 78
				std::uint32_t	totalFreeBlocks{ 0 };       // 7C
				std::uint32_t	freeSmallBlocks{ 0 };       // 80
				std::uint32_t	totalAllocatedBlocks{ 0 };  // 84
				std::uint32_t	pmpBarrier{ 0 };            // 88
			};
			static_assert(sizeof(ScrapHeap) == 0x90);
		}
	}
}
