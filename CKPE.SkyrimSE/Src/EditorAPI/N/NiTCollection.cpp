#include <EditorAPI/N/NiTCollection.h>
#include <EditorAPI/N/NiMemoryManager.h>
#include <CKPE.Asserts.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				void* NiMalloc(std::size_t a_sizeInBytes)
				{
					auto mem = malloc(a_sizeInBytes);
					//auto memManager = NiMemoryManager::GetSingleton();
					//auto mem = memManager->Allocate(a_sizeInBytes, 0, NiMemEventType::kMalloc, false);
					CKPE_ASSERT(mem != nullptr);
					return mem;
				}

				void* NiAlignedMalloc(std::size_t a_sizeInBytes, std::size_t a_alignment)
				{
					auto mem = aligned_alloc(a_alignment, a_sizeInBytes);
					//auto memManager = NiMemoryManager::GetSingleton();
					//auto mem = memManager->Allocate(a_sizeInBytes, a_alignment, NiMemEventType::kAlignedMalloc, false);
					CKPE_ASSERT(mem != nullptr);
					return mem;
				}

				void* NiRealloc(void* a_mem, std::size_t a_sizeInBytes)
				{
					auto mem = realloc(a_mem, a_sizeInBytes);
					/*if ((a_sizeInBytes == 0) && a_mem)
					{
						NiFree(a_mem);
						return nullptr;
					}
					else if (!a_mem)
						return NiMalloc(a_sizeInBytes);

					auto memManager = NiMemoryManager::GetSingleton();
					auto mem = memManager->Reallocate(a_mem, a_sizeInBytes, 4, NiMemEventType::kRealloc, false, static_cast<std::size_t>(-1));*/
					CKPE_ASSERT(mem != nullptr);
					return mem;
				}

				void* NiAlignedRealloc(void* a_mem, std::size_t a_sizeInBytes, std::size_t a_alignment)
				{
					auto mem = aligned_realloc(a_mem, a_alignment, a_sizeInBytes);
					/*if ((a_sizeInBytes == 0) && a_mem)
					{
						NiAlignedFree(a_mem);
						return nullptr;
					}
					else if (!a_mem)
						return NiAlignedMalloc(a_sizeInBytes, a_alignment);

					auto memManager = NiMemoryManager::GetSingleton();
					auto mem = memManager->Reallocate(a_mem, a_sizeInBytes, a_alignment, NiMemEventType::kAlignedRealloc, false, static_cast<std::size_t>(-1));*/
					CKPE_ASSERT(mem != nullptr);
					return mem;
				}

				void NiFree(void* a_mem)
				{
					if (a_mem)
					{
						//auto memManager = NiMemoryManager::GetSingleton();
						//memManager->Deallocate(a_mem, NiMemEventType::kFree);
					}
				}

				void NiAlignedFree(void* a_mem)
				{
					if (a_mem)
					{
					//	auto memManager = NiMemoryManager::GetSingleton();
					//	memManager->Deallocate(a_mem, NiMemEventType::kAlignedFree);
					}
				}
			}
		}
	}
}
