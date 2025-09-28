// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/BSTList.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESObjectCELL;

				// 0x10
				class TESCellUseList
				{
				public:
					__declspec(align(8)) struct CellUseInfo 
					{

						TESObjectCELL*	Cell;
						char			pad08[8];
						std::uint32_t	Count;
					};
					typedef BSSimpleList<CellUseInfo> CellUseInfoListT;

					// members
					/*00*/ CellUseInfoListT	cellUses;
				};
				static_assert(sizeof(TESCellUseList) == 0x10);
				static_assert(sizeof(TESCellUseList::CellUseInfo) == 0x18);
			}
		}
	}
}
