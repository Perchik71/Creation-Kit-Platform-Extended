// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <EditorAPI/Forms/BGSLayer.h>
#include <EditorAPI/Forms/TESObjectCELL.h>
#include <EditorAPI/Forms/TESObjectREFR.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				std::uint32_t BGSLayer::GetItemsCountInCell(const TESObjectCELL* cell) const noexcept(true)
				{
					std::uint32_t dwSize = ItemsCount;

					if (!cell)
						return dwSize;

					auto lpData = ItemsArrayConst;
					std::uint32_t dwResult = 0;

					for (std::uint32_t i = 0; i < dwSize; i++)
					{
						if (lpData[i]->ParentCell == cell)
							dwResult++;
					}

					return dwResult;
				}
			}
		}
	}
}