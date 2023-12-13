#pragma once

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		// 18
		template <typename T>
		class NiArray {
		public:
			NiArray();
			virtual ~NiArray();

			// sparse array, can have NULL entries that should be skipped
			// iterate from 0 to m_emptyRunStart - 1

		//	void	** _vtbl;			// 00
			T* m_data;			// 08
			uint16_t	m_arrayBufLen;		// 10 - max elements storable in m_data
			uint16_t	m_emptyRunStart;	// 12 - index of beginning of empty slot run
			uint16_t	m_size;				// 14 - number of filled slots
			uint16_t	m_growSize;			// 16 - number of slots to grow m_data by
		};
	}
}