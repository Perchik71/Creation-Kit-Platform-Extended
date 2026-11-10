#pragma once

#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			struct MemoryStats
			{
			public:
				// members
				const char*		name;			// 00
				std::size_t		usedSize;		// 08
				std::size_t		committedSize;	// 10
				std::size_t		reservedSize;	// 18
				std::uint32_t	overhead;		// 20
				std::uint32_t	pad24;          // 24
				std::size_t		freeSize;       // 28
			};
			static_assert(sizeof(MemoryStats) == 0x30);

			class IMemoryStoreBase
			{
			public:
				constexpr static auto RTTI = IDs::RTTI::IMemoryStoreBase;
				constexpr static auto VTABLE = IDs::VTABLE::IMemoryStoreBase[0];

				virtual ~IMemoryStoreBase() = default;  // 00

				[[nodiscard]] virtual std::size_t Size(const void* a_mem) const { return 0; };				// 01
				virtual void GetMemoryStats(MemoryStats* a_stats) {};										// 02
				virtual void SetAllOptionDbg(const void* a_block, bool a_value) {};							// 03
				virtual void SetStoreContextDbg(const void* a_block, bool a_value) {};						// 04
				[[nodiscard]] virtual bool IsStoreContextDbg(const void* a_block) { return false; }			// 05
			private:
				virtual void Nullsub_06() {};																// 06
			public:
				virtual void SetCheckPointDbg(const void* a_block, bool a_value) {};						// 07
				[[nodiscard]] virtual bool IsCheckPointDbg(const void* a_block) const { return false; };	// 08
			private:
				virtual void Nullsub_09() {};																// 09
			public:
				virtual void SetStackTraceDbg(const void* a_block, std::uint32_t a_trace) {}				// 0A
				[[nodiscard]] virtual bool IsStackTraceDbg(const void* a_block) const { return false; }		// 0B
			private:
				virtual void Nullsub_0C() {};																// 0C
			public:
				[[nodiscard]] virtual bool ContainsBlockImpl(const void* a_block) const { return false; }	// 0D
			};
			static_assert(sizeof(IMemoryStoreBase) == 0x8);
		}
	}
}
