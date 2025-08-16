// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				// sparse array, can have NULL entries that should be skipped
				// iterate from 0 to m_emptyRunStart - 1
				template <typename T>
				class NiTObjectArray
				{
				public:
					NiTObjectArray() = default;
					virtual ~NiTObjectArray() = default;

					T* Data;							// 08
					std::uint16_t	ArrayBufLen;		// 10 - max elements storable in m_data
					std::uint16_t	EmptyRunStart;		// 12 - index of beginning of empty slot run
					std::uint16_t	Size;				// 14 - number of filled slots
					std::uint16_t	GrowSize;			// 16 - number of slots to grow m_data by
				};
			}
		}
	}
}