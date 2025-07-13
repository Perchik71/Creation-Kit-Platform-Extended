// Author: Nukem9 
// Link: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/CKSSE/Experimental.h

#pragma once

#include <CKPE.Common.Common.h>

#include <cstdint>
#include <cstdint>
#include <initializer_list>

namespace CKPE
{
	namespace Common
	{
		class CKPE_COMMON_API RuntimeOptimization
		{
			std::uintptr_t _base{ 0 };
		public:
			struct NullsubPatch
			{
				std::initializer_list<std::uint8_t> Signature;
				std::uint8_t JumpPatch[5];
				std::uint8_t CallPatch[5];
			};
		private:
			RuntimeOptimization(const RuntimeOptimization&) = delete;
			RuntimeOptimization& operator=(const RuntimeOptimization&) = delete;

			std::uint64_t RemoveTrampolinesAndNullsubs(std::uintptr_t target, std::uintptr_t size) const;
			std::uint64_t RemoveMemInit(std::uintptr_t target, std::uintptr_t size) const noexcept(true);
		public:
			constexpr RuntimeOptimization() noexcept(true) = default;

			void Apply() noexcept(true);
		};
	}
}