// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiPointer.h

#pragma once

#include <EditorAPI/NiAPI/NiRefObject.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				template<typename T>
				class NiPointer
				{
				protected:
					T* m_pObject;

				public:
					inline NiPointer(T* Object = (T*)nullptr) noexcept(true)
					{
						m_pObject = Object;

						if (m_pObject)
							m_pObject->IncRefCount();
					}

					inline NiPointer(const NiPointer<T>& Other) noexcept(true)
					{
						m_pObject = Other.m_pObject;

						if (m_pObject)
							m_pObject->IncRefCount();
					}

					inline ~NiPointer() noexcept(true)
					{
						if (m_pObject)
							m_pObject->DecRefCount();
					}

					inline operator T* () const noexcept(true) { return m_pObject; }
					inline T& operator*() const noexcept(true) { return m_pObject; }
					inline T* operator->() const noexcept(true) { return m_pObject; }
					inline bool operator!=(const T* Other) const noexcept(true) { return m_pObject != Other; }

					inline NiPointer<T>& operator=(T* Other) noexcept(true)
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

					inline bool Empty() const noexcept(true) { return m_pObject == nullptr; }

					template<typename T>
					inline T* GetPtr() const noexcept(true) { return (T*)m_pObject; }

					template<typename T>
					inline void Reset(T* p) noexcept(true) { m_pObject = p; }
				};
				static_assert(sizeof(NiPointer<NiRefObject>) == 0x8);
			}
		}
	}
}