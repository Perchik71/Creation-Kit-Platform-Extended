#pragma once

#include <EditorAPI/S/ScrapHeap.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace CompactingStore
			{
				class Store;
			}

			class BSSmallBlockAllocator;
			class IMemoryHeap;

			class MemoryManager
			{
			public:
				class AutoScrapBuffer
				{
				public:
					AutoScrapBuffer() { ctor(); }
					~AutoScrapBuffer() { dtor(); }

					[[nodiscard]] void* GetPtr() const noexcept { return ptr; }

					// members
					void* ptr{ nullptr };  // 0

				private:
					AutoScrapBuffer* ctor()
					{
						using func_t = decltype(&AutoScrapBuffer::ctor);
						static Common::Relocation<func_t> func{ IDs::MemoryManager::AutoScrapBuffer::ctor };
						return func(this);
					}

					void dtor()
					{
						using func_t = decltype(&AutoScrapBuffer::dtor);
						static Common::Relocation<func_t> func{ IDs::MemoryManager::AutoScrapBuffer::dtor };
						return func(this);
					}
				};
				static_assert(sizeof(AutoScrapBuffer) == 0x8);

				class ThreadScrapHeap
				{
				public:
					// members
					ScrapHeap        heap;												// 00
					ThreadScrapHeap* next{ nullptr };									// 90
					std::uint32_t    owningThread{ GetCurrentThreadId() };				// 98
				};
				static_assert(sizeof(ThreadScrapHeap) == 0xA0);

				[[nodiscard]] static MemoryManager& GetSingleton()
				{
					using func_t = decltype(&MemoryManager::GetSingleton);
					const Common::Relocation<func_t> func{ IDs::MemoryManager::Singleton };
					return func();
				}

				[[nodiscard]] void* Allocate(std::size_t a_size, std::uint32_t a_alignment, bool a_alignmentRequired)
				{
					using func_t = decltype(&MemoryManager::Allocate);
					const Common::Relocation<func_t> func{ IDs::MemoryManager::Allocate };
					return func(this, a_size, a_alignment, a_alignmentRequired);
				}

				void Deallocate(void* a_mem, bool a_alignmentRequired)
				{
					using func_t = decltype(&MemoryManager::Deallocate);
					const Common::Relocation<func_t> func{ IDs::MemoryManager::Deallocate };
					return func(this, a_mem, a_alignmentRequired);
				}

				[[nodiscard]] std::size_t Size(void* a_mem)
				{
					using func_t = decltype(&MemoryManager::Size);
					const Common::Relocation<func_t> func{ IDs::MemoryManager::Size };
					return func(this, a_mem);
				}

				[[nodiscard]] ScrapHeap* GetThreadScrapHeap()
				{
					using func_t = decltype(&MemoryManager::GetThreadScrapHeap);
					const Common::Relocation<func_t> func{ IDs::MemoryManager::GetThreadScrapHeap };
					return func(this);
				}

				[[nodiscard]] void* Reallocate(void* a_oldMem, std::size_t a_newSize, std::uint32_t a_alignment, bool a_alignmentRequired)
				{
					using func_t = decltype(&MemoryManager::Reallocate);
					const Common::Relocation<func_t> func{ IDs::MemoryManager::Reallocate };
					return func(this, a_oldMem, a_newSize, a_alignment, a_alignmentRequired);
				}

				void RegisterMemoryManager()
				{
					using func_t = decltype(&MemoryManager::RegisterMemoryManager);
					const Common::Relocation<func_t> func{ IDs::MemoryManager::RegisterMemoryManager };
					return func(this);
				}

				// members
				bool initialized{ false };											// 000
				std::uint16_t numHeaps{ 0 };										// 002
				std::uint16_t numPhysicalHeaps{ 0 };								// 004
				IMemoryHeap** heaps{ nullptr };										// 008
				bool* allowOtherContextAllocs{ nullptr };							// 010
				IMemoryHeap* heapsByContext[127]{ nullptr };						// 018
				ThreadScrapHeap* threadScrapHeap{ nullptr };						// 410
				IMemoryHeap** physicalHeaps{ nullptr };								// 418
				IMemoryHeap* bigAllocHeap{ nullptr };								// 420
				IMemoryHeap* emergencyHeap{ nullptr };								// 428
				BSSmallBlockAllocator* smallBlockAllocator{ nullptr };				// 430
				CompactingStore::Store* compactingStore{ nullptr };					// 438
				IMemoryHeap* externalHavokAllocator{ nullptr };						// 440
				bool specialHeaps{ false };											// 448
				bool allowPoolUse{ true };											// 449
				std::uint32_t sysAllocBytes{ 0 };									// 44C
				std::uint32_t mallocBytes{ 0 };										// 450
				std::uint32_t alignmentForPools{ 4 };								// 454
				std::uint32_t mainThreadMemoryProblemPassSignal{ 0 };				// 458
				std::size_t failedAllocationSize{ 0 };								// 460
				std::uint32_t numMemoryProblemPassesRun{ 0 };						// 468
				std::size_t timeOfLastMemoryProblemPass{ 0 };						// 470
				IMemoryHeap* defaultHeap{ nullptr };								// 478
			};
			static_assert(sizeof(MemoryManager) == 0x480);

			[[nodiscard]] inline void* malloc(std::size_t a_size)
			{
				auto& mem = MemoryManager::GetSingleton();
				return mem.Allocate(a_size, 0, false);
			}

			template <class T>
			[[nodiscard]] T* malloc()
			{
				return static_cast<T*>(malloc(sizeof(T)));
			}

			[[nodiscard]] inline void* aligned_alloc(std::size_t a_alignment, std::size_t a_size)
			{
				auto& mem = MemoryManager::GetSingleton();
				return mem.Allocate(a_size, static_cast<std::uint32_t>(a_alignment), true);
			}

			template <class T>
			[[nodiscard]] T* aligned_alloc()
			{
				return static_cast<T*>(aligned_alloc(alignof(T), sizeof(T)));
			}

			[[nodiscard]] inline void* calloc(std::size_t a_num, std::size_t a_size)
			{
				const auto ret = malloc(a_num * a_size);
				if (ret)
					std::memset(ret, 0, a_num * a_size);
				return ret;
			}

			template <class T>
			[[nodiscard]] T* calloc(std::size_t a_num)
			{
				return static_cast<T*>(calloc(a_num, sizeof(T)));
			}

			[[nodiscard]] inline void* realloc(void* a_ptr, std::size_t a_newSize)
			{
				auto& mem = MemoryManager::GetSingleton();
				return mem.Reallocate(a_ptr, a_newSize, 0, false);
			}

			[[nodiscard]] inline void* aligned_realloc(void* a_ptr, std::size_t a_alignment, std::size_t a_newSize)
			{
				auto& mem = MemoryManager::GetSingleton();
				return mem.Reallocate(a_ptr, a_newSize, static_cast<std::uint32_t>(a_alignment), true);
			}

			inline void free(void* a_ptr)
			{
				auto& mem = MemoryManager::GetSingleton();
				return mem.Deallocate(a_ptr, false);
			}

			inline void aligned_free(void* a_ptr)
			{
				auto& mem = MemoryManager::GetSingleton();
				return mem.Deallocate(a_ptr, true);
			}
		}
	}
}