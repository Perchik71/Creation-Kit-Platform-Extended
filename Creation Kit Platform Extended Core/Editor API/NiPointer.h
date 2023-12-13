// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiPointer.h

#pragma once

#include "NiRefObject.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		template<typename T>
		class NiPointer
		{
		protected:
			T* m_pObject;

		public:
			inline NiPointer(T* Object = (T*)nullptr)
			{
				m_pObject = Object;

				if (m_pObject)
					m_pObject->IncRefCount();
			}

			inline NiPointer(const NiPointer<T>& Other)
			{
				m_pObject = Other.m_pObject;

				if (m_pObject)
					m_pObject->IncRefCount();
			}

			inline ~NiPointer()
			{
				if (m_pObject)
					m_pObject->DecRefCount();
			}

			inline operator T* () const
			{
				return m_pObject;
			}

			inline T& operator*() const
			{
				return m_pObject;
			}

			inline T* operator->() const
			{
				return m_pObject;
			}

			inline NiPointer<T>& operator=(T* Other)
			{
				if (m_pObject != Other)
				{
					if (Other)
						Other->IncRefCount();

					if (m_pObject)
						m_pObject->DecRefCount();

					m_pObject = Other;
				}

				return *this;
			}

			inline bool operator!=(const T* Other) const
			{
				return m_pObject != Other;
			}
		};
		static_assert(sizeof(NiPointer<NiRefObject>) == 0x8);
	}
}