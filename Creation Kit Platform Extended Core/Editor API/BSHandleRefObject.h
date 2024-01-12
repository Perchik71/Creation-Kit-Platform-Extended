// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/CKSSE/BSHandleRefObject_CK.h

#include "NiRefObject.h"

#pragma once

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		template<typename _Ty, typename HandleRef, uint32_t RefCountBits>
		class BSHandleRefObjectT : public NiRefObject
		{
		public:
			//
			// 31             +1       RefCountBits    0
			// |--------------|--------|---------------|
			// | Handle Index | Active |   Ref Count   |
			// |--------------|--------|---------------|
			//
			constexpr static _Ty ACTIVE_BIT_INDEX = (_Ty)RefCountBits;
			constexpr static _Ty HANDLE_BIT_INDEX = (_Ty)ACTIVE_BIT_INDEX + 1;
			constexpr static _Ty REF_COUNT_MASK = (1ull << ACTIVE_BIT_INDEX) - 1ull;

		public:
			BSHandleRefObjectT()
			{
				ClearHandleEntryIndex();
			}

			virtual ~BSHandleRefObjectT()
			{
				ClearHandleEntryIndex();
			}

			_Ty IncRefCount()
			{
				AssertMsg(GetRefCount() < REF_COUNT_MASK,
					"BSHandleRefObject - IncRefCount is about to cause refcount wraparound to 0.");

				return ((HandleRef*)this)->IncRefCount();
			}

			_Ty DecRefCount()
			{
				AssertMsg(GetRefCount() != 0,
					"BSHandleRefObject - DecRefCount called with refcount already 0.");

				return ((HandleRef*)this)->DecRefCount();
			}

			_Ty GetRefCount() const
			{
				return ((_Ty)m_uiRefCount) & REF_COUNT_MASK;
			}

			void SetHandleEntryIndex(_Ty HandleIndex)
			{
				m_uiRefCount = (_Ty)((HandleIndex << HANDLE_BIT_INDEX) | (1ull << ACTIVE_BIT_INDEX) | GetRefCount());
			}

			_Ty GetHandleEntryIndex() const
			{
				return ((_Ty)m_uiRefCount) >> HANDLE_BIT_INDEX;
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
		static_assert(sizeof(BSHandleRefObjectT<uint32_t, NiRefObject, 10>) == 0x10);
		static_assert(sizeof(BSHandleRefObjectT<uint64_t, NiRefObject_64, 10>) == 0x10);

		typedef BSHandleRefObjectT<uint32_t, NiRefObject, 10> BSHandleRefObject_Original;
		typedef BSHandleRefObjectT<uint32_t, NiRefObject, 8> BSHandleRefObject_Extremly;
		typedef BSHandleRefObjectT<uint64_t, NiRefObject_64, 10> BSHandleRefObject_64_Extremly;
	}
}