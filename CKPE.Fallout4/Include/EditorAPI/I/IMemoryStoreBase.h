// Copyright � 2024-2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>

#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			struct MemoryStats
			{
				std::uint64_t v[6];
			};

			class __declspec(novtable) IMemoryStoreBase
			{
			public:
				constexpr static Common::ID VTABLE = IDs::VTABLE::IMemoryStoreBase[0];

				virtual ~IMemoryStoreBase() = default;														// 00
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