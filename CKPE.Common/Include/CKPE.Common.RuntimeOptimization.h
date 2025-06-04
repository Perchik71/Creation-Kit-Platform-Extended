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
			RuntimeOptimization(const RuntimeOptimization&) = delete;
			RuntimeOptimization& operator=(const RuntimeOptimization&) = delete;

			std::uint64_t RemoveTrampolines(std::uintptr_t target, std::uintptr_t size);
		public:
			constexpr RuntimeOptimization() noexcept(true) = default;

			void Apply();
		};


		struct NullsubPatch
		{
			std::initializer_list<std::uint8_t> Signature;
			std::uint8_t JumpPatch[5];
			std::uint8_t CallPatch[5];
		};

		//void RunOptimizations();

		//std::uint64_t PatchEditAndContinue(SmartPointer<Core::RelocationDatabaseItem> patch);
		/*std::uint64_t PatchMemInit();
		std::uint64_t PatchLinkedList();*/
		//std::uint64_t PatchTemplatedFormIterator(SmartPointer<Core::RelocationDatabaseItem> patch);

		//const NullsubPatch* FindNullsubPatch(std::uintptr_t SourceAddress, std::uintptr_t TargetFunction);
		//bool PatchNullsub(std::uintptr_t SourceAddress, std::uintptr_t TargetFunction, const NullsubPatch* Patch = nullptr);

		/*namespace Starfield
		{
			std::uint64_t PatchHasPointer();
		}*/
	}
}