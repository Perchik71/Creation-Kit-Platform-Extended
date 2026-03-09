// Copyright © 2024-2026 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>

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
				virtual ~IMemoryStoreBase() = default;											// 00
				virtual std::size_t Size(const void* a_mem) const = 0;							// 01
				virtual void GetMemoryStats(MemoryStats* a_stats) = 0;							// 02
				virtual void SetAllOptionDbg(const void* a_block, bool a_value) = 0;			// 03
				virtual void SetStoreContextDbg(const void* a_block, bool a_value) = 0;			// 04
				virtual bool IsStoreContextDbg(const void* a_block) = 0;						// 05
			private:
				virtual void Nullsub_06() = 0;													// 06
			public:
				virtual void SetCheckPointDbg(const void* a_block, bool a_value) = 0;			// 07
				virtual bool IsCheckPointDbg(const void* a_block) const = 0;					// 08
			private:
				virtual void Nullsub_09() = 0;													// 09
			public:
				virtual void SetStackTraceDbg(const void* a_block, std::uint32_t a_trace) = 0;	// 0A
				virtual bool IsStackTraceDbg(const void* a_block) const = 0;					// 0B
			private:
				virtual void Nullsub_0C() = 0;													// 0C
			public:
				virtual bool ContainsBlockImpl(const void* a_block) const = 0;					// 0D
			};
			static_assert(sizeof(IMemoryStoreBase) == 0x8);
		}
	}
}