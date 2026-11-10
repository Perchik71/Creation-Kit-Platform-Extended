// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/N/NiAllocator.h>
#include <CKPE.Common.Relocation.h>
#include <CKPE.Asserts.h>
#include <EditorAPI/IDs.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiMemoryManager 
				{
				public:
					static NiMemoryManager* GetSingleton()
					{
						Common::Relocation<NiMemoryManager**> singleton{ IDs::NiMemoryManager::Singleton };
						return *singleton;
					}

					void* Allocate(std::size_t a_sizeInBytes, std::size_t a_alignment, NiMemEventType a_eventType, bool a_provideAccurateSizeOnDeallocate = false,
						const char* a_sourceFile = nullptr, std::int32_t a_sourceLine = -1, const char* a_function = nullptr)
					{
						CKPE_ASSERT(allocator != nullptr);
						auto mem = allocator->Allocate(a_sizeInBytes, a_alignment, a_eventType, a_provideAccurateSizeOnDeallocate, a_sourceFile, a_sourceLine, a_function);
						CKPE_ASSERT(mem != nullptr);
						return mem;
					}

					void  Deallocate(void* a_mem, NiMemEventType a_eventType, std::size_t a_sizeinBytes = static_cast<std::size_t>(-1))
					{
						CKPE_ASSERT(allocator);
						allocator->Deallocate(a_mem, a_eventType, a_sizeinBytes);
					}

					void* Reallocate(void* a_mem, std::size_t a_sizeInBytes, std::size_t a_alignment, NiMemEventType a_eventType,
						bool a_provideAccurateSizeOnDeallocate = false, std::size_t a_sizeCurrent = static_cast<std::size_t>(-1), const char* a_sourceFile = nullptr,
						std::int32_t a_sourceLine = -1, const char* a_function = nullptr)
					{
						CKPE_ASSERT(allocator != nullptr);
						auto mem = allocator->Reallocate(a_mem, a_sizeInBytes, a_alignment, a_eventType, a_provideAccurateSizeOnDeallocate, a_sizeCurrent, a_sourceFile, a_sourceLine, a_function);
						CKPE_ASSERT(mem != nullptr);
						return mem;
					}

					bool  TrackAllocate(const void* const a_mem, std::size_t a_sizeInBytes, NiMemEventType a_eventType,
						const char* a_sourceFile = nullptr, std::int32_t a_sourceLine = -1, const char* a_function = nullptr)
					{
						CKPE_ASSERT(allocator);
						return allocator->TrackAllocate(a_mem, a_sizeInBytes, a_eventType, a_sourceFile, a_sourceLine, a_function);
					}

					bool  TrackDeallocate(const void* const a_mem, NiMemEventType a_eventType)
					{
						CKPE_ASSERT(allocator);
						return allocator->TrackDeallocate(a_mem, a_eventType);
					}

					// members
					NiAllocator* allocator;  // 00
				};
				static_assert(sizeof(NiMemoryManager) == 0x8);
			}
		}
	}
}