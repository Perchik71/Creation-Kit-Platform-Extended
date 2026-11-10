#pragma once

#include <EditorAPI/I/IMemoryStoreBase.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			class __declspec(novtable) IMemoryStore :
				public IMemoryStoreBase  // 0
			{
			public:
				virtual ~IMemoryStore() = default;																									// 00

				constexpr static Common::ID RTTI = IDs::RTTI::IMemoryStore;
				constexpr static Common::ID VTABLE = IDs::VTABLE::IMemoryStore[0];

				// add
				virtual void* AllocateAlignImpl(std::size_t a_size, std::uint32_t a_alignment) = 0;													// 0E
				virtual void  DeallocateAlignImpl(void*& a_block) = 0;																				// 0F
				virtual void* TryAllocateImpl([[maybe_unused]] std::size_t a_size, [[maybe_unused]] std::uint32_t a_alignment) { return nullptr; }	// 10
			};
			static_assert(sizeof(IMemoryStore) == 0x8);
		}
	}
}
