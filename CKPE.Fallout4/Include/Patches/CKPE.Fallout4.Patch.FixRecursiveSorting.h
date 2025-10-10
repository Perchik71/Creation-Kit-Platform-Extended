// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <algorithm>
#include <CKPE.Common.Patch.h>
#include <EditorAPI/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			class FixRecursiveSorting : public Common::Patch
			{
				template<typename T, bool Stable = false>
				static void sub(EditorAPI::BSTArray<T>& Array, std::int32_t(*SortFunction)(void*, void*))
				{
					auto compare = [SortFunction](const T& A, const T& B)
						{
							return SortFunction(A, B) == -1;
						};

					if constexpr (Stable)
						std::stable_sort(&Array[0], &Array[Array.size()], compare);
					else
						std::sort(&Array[0], &Array[Array.size()], compare);
				}

				FixRecursiveSorting(const FixRecursiveSorting&) = delete;
				FixRecursiveSorting& operator=(const FixRecursiveSorting&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				FixRecursiveSorting();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
			};
		}
	}
}