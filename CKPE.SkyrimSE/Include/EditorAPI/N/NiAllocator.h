#pragma once

#include <EditorAPI/IDs_RTTI.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				enum class NiMemEventType
				{
					kUnknown = 0,
					kOperNew,
					kOperNewArray,
					kOperDelete,
					kOperDeleteArray,
					kMalloc,
					kRealloc,
					kAlignedMalloc,
					kAlignedRealloc,
					kFree,
					kAlignedFree,
					kExternalAlloc,
					kExternalFree
				};

				class NiAllocator
				{
				public:
					inline static constexpr auto RTTI = IDs::RTTI::NiAllocator;

					virtual ~NiAllocator() = default;

					// add
					virtual void* Allocate(std::size_t& a_sizeInBytes, std::size_t& a_alignment, NiMemEventType a_eventType, 
						bool a_provideAccurateSizeOnDeallocate, const char* a_file, std::int32_t a_line, const char* a_function) = 0;
					virtual void  Deallocate(void* a_memory, NiMemEventType a_eventType, std::size_t a_sizeInBytes) = 0;
					virtual void* Reallocate(void* a_memory, size_t& a_sizeInBytes, std::size_t& a_alignment, NiMemEventType a_eventType, 
						bool a_provideAccurateSizeOnDeallocate, std::size_t a_sizeCurrent, const char* a_file, std::int32_t a_line, const char* a_function) = 0;
					virtual bool  TrackAllocate(const void* const a_memory, std::size_t a_sizeInBytes, NiMemEventType a_eventType,
						const char* a_file, std::int32_t a_line, const char* a_function) = 0;
					virtual bool  TrackDeallocate(const void* const a_memory, NiMemEventType a_eventType) = 0;
					virtual void  Unk_06(void) = 0;
					virtual void  Initialize() = 0;
					virtual void  Shutdown() = 0;
					virtual bool  VerifyAddress(const void* a_memory) = 0;
				};
				static_assert(sizeof(NiAllocator) == 0x8);
			}
		}
	}
}
