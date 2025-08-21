// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Patch.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			class RuntimeOptimization : public Common::Patch
			{
				std::uint64_t PatchLinkedList(std::uintptr_t beg, std::uintptr_t end) noexcept(true);
				std::uint64_t PatchTemplatedFormIterator(std::uintptr_t _beg, std::uintptr_t _end,
					std::uintptr_t base, Common::RelocatorDB::PatchDB* db) noexcept(true);

				RuntimeOptimization(const RuntimeOptimization&) = delete;
				RuntimeOptimization& operator=(const RuntimeOptimization&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				RuntimeOptimization();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
			};
		}
	}
}