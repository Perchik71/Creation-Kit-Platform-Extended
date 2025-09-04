// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				// Another array, it is similar to BSTArray. 
				// However, it has no offsets and occupies exactly 0x10.
				template<typename T>
				class NiTSimpleArray
				{
				private:
					T* m_pBase;					// Sparse array (nullptr for invalid)
					std::uint32_t m_usMaxSize;	// Number of elements allocated
					std::uint32_t m_usSize;		// Last index with a valid element
				public:
					inline std::uint32_t GetMaxSize() const
					{
						return m_usMaxSize;
					}

					inline std::uint32_t GetSize() const
					{
						return m_usSize;
					}

					inline T At(std::size_t Index) const
					{
						return m_pBase[Index];
					}

					inline T& At(std::size_t Index)
					{
						return m_pBase[Index];
					}

					inline T operator[](std::size_t Index) const { return At(Index); }
					inline T& operator[](std::size_t Index) { return At(Index); }
				};
				static_assert(sizeof(NiTSimpleArray<void*>) == 0x10);

				// Another array, it is similar to BSTArray. 
				// However, it has no offsets and occupies exactly 0x10.
				template<typename T>
				class NiTSimpleBigArray
				{
				private:
					T* m_pBase;					// Sparse array (nullptr for invalid)
					std::uint64_t m_usMaxSize;	// Number of elements allocated
					std::uint64_t m_usSize;		// Last index with a valid element
				public:
					inline std::uint32_t GetMaxSize() const
					{
						return m_usMaxSize;
					}

					inline std::uint32_t GetSize() const
					{
						return m_usSize;
					}

					inline T At(std::size_t Index) const
					{
						return m_pBase[Index];
					}

					inline T& At(std::size_t Index)
					{
						return m_pBase[Index];
					}

					inline T operator[](std::size_t Index) const { return At(Index); }
					inline T& operator[](std::size_t Index) { return At(Index); }
				};
				static_assert(sizeof(NiTSimpleBigArray<void*>) == 0x18);
			}
		}
	}
}