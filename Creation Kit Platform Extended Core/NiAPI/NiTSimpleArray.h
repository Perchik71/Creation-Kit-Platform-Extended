// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace NiAPI
	{
		// Another array, it is similar to BSTArray. 
		// However, it has no offsets and occupies exactly 0x10.
		template<typename T>
		class NiTSimpleArray
		{
		private:
			T* m_pBase;				// Sparse array (nullptr for invalid)
			uint32_t m_usMaxSize;	// Number of elements allocated
			uint32_t m_usSize;		// Last index with a valid element
		public:
			inline uint32_t GetMaxSize() const
			{
				return m_usMaxSize;
			}

			inline uint32_t GetSize() const
			{
				return m_usSize;
			}

			inline T At(size_t Index) const
			{
				return m_pBase[Index];
			}

			inline T& At(size_t Index)
			{
				return m_pBase[Index];
			}

			inline T operator[](size_t Index) const { return At(Index); }
			inline T& operator[](size_t Index) { return At(Index); }
		};
		static_assert(sizeof(NiTSimpleArray<void*>) == 0x10);
	}
}