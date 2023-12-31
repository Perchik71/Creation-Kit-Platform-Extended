// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/CKSSE/BSHandleRefObject_CK.h

#include "NiRefObject.h"

#pragma once

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		template<uint32_t RefCountBits>
		class BSHandleRefObjectT : public NiRefObject
		{
		private:
			//
			// 31             +1       RefCountBits    0
			// |--------------|--------|---------------|
			// | Handle Index | Active |   Ref Count   |
			// |--------------|--------|---------------|
			//
			constexpr static uint32_t ACTIVE_BIT_INDEX = RefCountBits;
			constexpr static uint32_t HANDLE_BIT_INDEX = ACTIVE_BIT_INDEX + 1;
			constexpr static uint32_t REF_COUNT_MASK = (1ull << ACTIVE_BIT_INDEX) - 1ull;

		public:
			BSHandleRefObjectT()
			{
				ClearHandleEntryIndex();
			}

			virtual ~BSHandleRefObjectT()
			{
				ClearHandleEntryIndex();
			}

			uint32_t IncRefCount()
			{
				AssertMsg(GetRefCount() < REF_COUNT_MASK,
					"BSHandleRefObject - IncRefCount is about to cause refcount wraparound to 0.");

				return ((NiRefObject*)this)->IncRefCount();
			}

			uint32_t DecRefCount()
			{
				AssertMsg(GetRefCount() != 0,
					"BSHandleRefObject - DecRefCount called with refcount already 0.");

				return ((NiRefObject*)this)->DecRefCount();
			}

			uint32_t GetRefCount() const
			{
				return ((uint32_t)m_uiRefCount) & REF_COUNT_MASK;
			}

			void SetHandleEntryIndex(uint32_t HandleIndex)
			{
				m_uiRefCount = (ULONG)((HandleIndex << HANDLE_BIT_INDEX) | (1ull << ACTIVE_BIT_INDEX) | GetRefCount());
			}

			uint32_t GetHandleEntryIndex() const
			{
				return ((uint32_t)m_uiRefCount) >> HANDLE_BIT_INDEX;
			}

			void ClearHandleEntryIndex()
			{
				m_uiRefCount &= REF_COUNT_MASK;
			}

			bool IsHandleValid() const
			{
				return (m_uiRefCount & (1ull << ACTIVE_BIT_INDEX)) != 0;
			}
		};
		static_assert(sizeof(BSHandleRefObjectT<10>) == 0x10);

		typedef BSHandleRefObjectT<10> BSHandleRefObject_Original;
		typedef BSHandleRefObjectT<8> BSHandleRefObject_Extremly;
	}
}