// Author: Nukem9 
// Link: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/CKSSE/Experimental.h

#pragma once

#include "Core/Engine.h"
#include "Core/Relocator.h"
#include "Core/RelocationDatabase.h"

namespace CreationKitPlatformExtended
{
	namespace Experimental
	{
		struct NullsubPatch
		{
			std::initializer_list<uint8_t> Signature;
			uint8_t JumpPatch[5];
			uint8_t CallPatch[5];
		};

		void RunOptimizations();

		uint64_t PatchEditAndContinue(SmartPointer<Core::RelocationDatabaseItem> patch);
		uint64_t PatchMemInit();
		uint64_t PatchLinkedList();
		uint64_t PatchTemplatedFormIterator(SmartPointer<Core::RelocationDatabaseItem> patch);

		const NullsubPatch* FindNullsubPatch(uintptr_t SourceAddress, uintptr_t TargetFunction);
		bool PatchNullsub(uintptr_t SourceAddress, uintptr_t TargetFunction, const NullsubPatch* Patch = nullptr);

		namespace Starfield
		{
			uint64_t PatchHasPointer();
		}
	}
}