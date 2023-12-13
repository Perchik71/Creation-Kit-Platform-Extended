// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiNode.h

#pragma once

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		template<typename T>
		class NiTArray
		{
		public:
			virtual ~NiTArray();

			T* m_pBase;				// Sparse array (nullptr for invalid)
			uint16_t m_usMaxSize;	// Number of elements allocated
			uint16_t m_usSize;		// Last index with a valid element
			uint16_t m_usESize;		// Number of elements in use
			uint16_t m_usGrowBy;

			uint16_t GetMaxSize() const
			{
				return m_usMaxSize;
			}

			uint16_t GetSize() const
			{
				return m_usSize;
			}

			uint16_t GetESize() const
			{
				return m_usESize;
			}

			T At(size_t Index) const
			{
				return m_pBase[Index];
			}
		};
	}
}