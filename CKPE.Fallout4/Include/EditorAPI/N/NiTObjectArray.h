// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiAVObject;
				template <class T> class NiPointer;

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
				static_assert(sizeof(NiTObjectArray<NiPointer<NiAVObject>>) == 0x18);
				static_assert(offsetof(NiTObjectArray<NiPointer<NiAVObject>>, Data) == 0x8);
				static_assert(offsetof(NiTObjectArray<NiPointer<NiAVObject>>, ArrayBufLen) == 0x10);
				static_assert(offsetof(NiTObjectArray<NiPointer<NiAVObject>>, EmptyRunStart) == 0x12);
				static_assert(offsetof(NiTObjectArray<NiPointer<NiAVObject>>, Size) == 0x14);
				static_assert(offsetof(NiTObjectArray<NiPointer<NiAVObject>>, GrowSize) == 0x16);
			}
		}
	}
}