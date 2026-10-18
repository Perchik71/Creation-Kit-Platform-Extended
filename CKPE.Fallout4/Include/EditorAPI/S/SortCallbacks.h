// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <CKPE.Common.Relocation.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// List sort callbacks.
			namespace SortCallbacks
			{
				// ListView compare callback: (item 1, item 2, sort column id).
				// CK 1.11.246.0 0x1406765B0.
				inline std::int32_t CompareItems(const void* Lhs, const void* Rhs, std::size_t Column) noexcept(true)
				{
					using func_t = decltype(&CompareItems);
					static Common::Relocation<func_t> func{ Common::ID{ 643144, 1621905 } };
					return func(Lhs, Rhs, Column);
				}
			}
		}
	}
}
