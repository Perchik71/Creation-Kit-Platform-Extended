// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "TESObjectCELL.h"
#include "../NiAPI/NiTSimpleArray.h"

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESCellUseList
				{
				public:
					struct TESCellUse
					{
						TESObjectCELL* FormCell;
						std::uint64_t Unk08;
						std::uint32_t Unk10;
						std::uint32_t Unk14;
					};

					NiAPI::NiTSimpleBigArray<TESCellUse> List;
					std::uint64_t Unk08;
				};
				static_assert(sizeof(TESCellUseList::TESCellUse) == 0x18);
				static_assert(sizeof(TESCellUseList) == 0x20);
			}
		}
	}
}