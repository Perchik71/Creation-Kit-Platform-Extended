// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiPointer.h

#pragma once

#include <algorithm>
#include <CKPE.Asserts.h>
#include <EditorAPI/NiAPI/NiRefObject.h>
#include <EditorAPI/CRC.h>

namespace CKPE
{
	namespace Fallout4
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

					template <class>
					friend class NiPointer;

					void TryAttach()
					{
						if (m_pObject)
							m_pObject->IncRefCount();
					}

					void TryDetach()
					{
						if (m_pObject)
						{
							m_pObject->DecRefCount();
							m_pObject = nullptr;
						}
					}
				public:
					constexpr NiPointer() noexcept(true) = default;
					constexpr NiPointer(std::nullptr_t) noexcept(true) {}

					explicit NiPointer(T* rhs) :
						m_pObject(rhs)
					{
						TryAttach();
					}

					template<class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, std::int32_t> = 0>
					explicit NiPointer(Y* rhs) :
						m_pObject(rhs)
					{
						TryAttach();
					}

					NiPointer(const NiPointer& rhs) :
						m_pObject(rhs.m_pObject)
					{
						TryAttach();
					}

					template<class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, std::int32_t> = 0>
					NiPointer(const NiPointer<Y>& rhs) :
						m_pObject(rhs.m_pObject)
					{
						TryAttach();
					}

					NiPointer(NiPointer&& rhs) noexcept(true) :
						m_pObject(rhs.m_pObject)
					{
						rhs.m_pObject = nullptr;
					}

					template<class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, std::int32_t> = 0>
					NiPointer(NiPointer<Y>&& rhs) noexcept(true) :
						m_pObject(rhs.m_pObject)
					{
						rhs.m_pObject = nullptr;
					}

					~NiPointer() noexcept(true)
					{
						TryDetach();
					}

					inline constexpr T* GetPtr() const noexcept(true) { return m_pObject; }

					[[nodiscard]] inline operator T*() const noexcept(true)
					{ 
						return m_pObject;
					}

					[[nodiscard]] explicit constexpr operator bool() const noexcept
					{
						return static_cast<bool>(m_pObject);
					}

					[[nodiscard]] inline constexpr T& operator*() const noexcept(true)
					{
						CKPE_ASSERT(static_cast<bool>(*this));
						return m_pObject; 
					}

					[[nodiscard]] inline constexpr T* operator->() const noexcept(true)
					{
						CKPE_ASSERT(static_cast<bool>(*this));
						return m_pObject;
					}

					inline constexpr bool operator==(const T* Other) const noexcept(true) { return m_pObject == Other; }
					inline constexpr bool operator!=(const T* Other) const noexcept(true) { return m_pObject != Other; }

					inline NiPointer<T>& operator=(T* rhs)
					{
						if (m_pObject != rhs)
						{
							TryDetach();
							m_pObject = rhs;
							TryAttach();
						}

						return *this;
					}

					NiPointer& operator=(const NiPointer& rhs)
					{
						if (this != std::addressof(rhs))
						{
							TryDetach();
							m_pObject = rhs.m_pObject;
							TryAttach();
						}
						return *this;
					}

					template<class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, std::int32_t> = 0>
					NiPointer& operator=(const NiPointer<Y>& rhs)
					{
						TryDetach();
						m_pObject = rhs.m_pObject;
						TryAttach();
						return *this;
					}

					NiPointer& operator=(NiPointer&& rhs)
					{
						if (this != std::addressof(rhs)) 
						{
							TryDetach();
							m_pObject = rhs.m_pObject;
							rhs.m_pObject = nullptr;
						}
						return *this;
					}

					template<class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, std::int32_t> = 0>
					NiPointer& operator=(NiPointer<Y>&& rhs)
					{
						TryDetach();
						m_pObject = rhs.m_pObject;
						rhs.m_pObject = nullptr;
						return *this;
					}

					inline constexpr bool Empty() const noexcept(true) { return m_pObject == nullptr; }

					void Reset()
					{ 
						TryDetach();
					}

					template<class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, std::int32_t> = 0>
					void Reset(Y* ptr)
					{
						if (m_pObject != ptr)
						{
							TryDetach();
							m_pObject = ptr;
							TryAttach();
						}
					}
				};
				static_assert(sizeof(NiPointer<NiRefObject>) == 0x8);

				template<class T, class... Args>
				[[nodiscard]] NiPointer<T> MakeNiSmart(Args&&... args)
				{
					return NiPointer<T>{ new T(std::forward<Args>(args)...) };
				}

				template<class T1, class T2>
				[[nodiscard]] constexpr bool operator==(const NiPointer<T1>& lhs, const NiPointer<T2>& rhs)
				{
					return lhs.get() == rhs.get();
				}

				template<class T1, class T2>
				[[nodiscard]] constexpr bool operator!=(const NiPointer<T1>& lhs, const NiPointer<T2>& rhs)
				{
					return !(lhs == rhs);
				}

				template<class T>
				[[nodiscard]] constexpr bool operator==(const NiPointer<T>& lhs, std::nullptr_t) noexcept(true)
				{
					return !lhs;
				}

				template<class T>
				[[nodiscard]] constexpr bool operator==(std::nullptr_t, const NiPointer<T>& rhs) noexcept(true)
				{
					return !rhs;
				}

				template<class T>
				[[nodiscard]] constexpr bool operator!=(const NiPointer<T>& lhs, std::nullptr_t) noexcept(true)
				{
					return static_cast<bool>(lhs);
				}

				template<class T>
				[[nodiscard]] constexpr bool operator!=(std::nullptr_t, const NiPointer<T>& rhs) noexcept(true)
				{
					return static_cast<bool>(rhs);
				}

				template<class T>
				NiPointer(T*) -> NiPointer<T>;
			}

			template<class T>
			struct BSCRC32<NiAPI::NiPointer<T>>
			{
			public:
				[[nodiscard]] inline std::uint32_t operator()(const NiAPI::NiPointer<T>& key) const noexcept(true)
				{
					return BSCRC32<T*>()(key.get());
				}
			};
		}
	}
}