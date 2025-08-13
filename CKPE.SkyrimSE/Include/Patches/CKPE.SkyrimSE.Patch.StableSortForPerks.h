// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <algorithm>
#include <CKPE.Common.Patch.h>
#include <EditorAPI/BSTArray.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			class StableSortForPerks : public Common::Patch
			{
				StableSortForPerks(const StableSortForPerks&) = delete;
				StableSortForPerks& operator=(const StableSortForPerks&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				StableSortForPerks();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				template<typename T, bool Stable = false>
				static void ArrayQuickSortRecursive(EditorAPI::BSTArray<T>&Array, int(*SortFunction)(const void*, const void*))
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
			};
		}
	}
}