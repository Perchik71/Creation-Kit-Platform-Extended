// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <atomic>

#include "CRC.h"
#include <CKPE.Asserts.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			struct BSIntrusiveRefCounted
			{
			public:
				constexpr BSIntrusiveRefCounted() noexcept(true) {}

				[[nodiscard]] std::uint32_t DecRef() const
				{
					std::atomic_ref myRefCount{ refCount };
					return --myRefCount;
				}

				std::uint32_t IncRef() const
				{
					std::atomic_ref myRefCount{ refCount };
					return ++myRefCount;
				}

				[[nodiscard]] constexpr std::uint32_t QRefCount() const noexcept(true) { return refCount; }

				mutable std::uint32_t refCount{ 0 };  // 0
			};
			static_assert(sizeof(BSIntrusiveRefCounted) == 0x4);

			template<class T>
			struct BSTSmartPointerIntrusiveRefCount
			{
			public:
				static void Acquire(T* ptr) noexcept(true) { if (ptr) ptr->IncRef(); }
				static void Release(T* ptr)
				{
					if (ptr && (ptr->DecRef() == 0))
						delete ptr;
				}
			};

			template<class T>
			struct BSTSmartPointerAutoPtr
			{
			public:
				constexpr static void Acquire(T* ptr) {}
				static void Release(T* ptr) { delete ptr; }
			};

			template<class T>
			struct BSTSmartPointerGamebryoRefCount
			{
			public:
				constexpr static void Acquire(T* ptr) { if (ptr) ptr->IncRefCount(); }
				static void Release(T* ptr) { if (ptr) ptr->DecRefCount(); }
			};

			template <class T, template <class> class RefManager = BSTSmartPointerIntrusiveRefCount>
			class BSTSmartPointer
			{
			public:
				using element_type = T;
				using reference_manager = RefManager<T>;

				constexpr BSTSmartPointer() noexcept(true) = default;
				constexpr BSTSmartPointer(std::nullptr_t) noexcept(true) {}
				template<class Y, std::enable_if_t<std::is_convertible_v<Y*, element_type*>, std::int32_t> = 0>
				explicit BSTSmartPointer(Y* rhs) : _ptr(rhs) { TryAttach(); }

				BSTSmartPointer(const BSTSmartPointer& rhs) : _ptr(rhs._ptr) { TryAttach(); }

				template<class Y, std::enable_if_t<std::is_convertible_v<Y*, element_type*>, std::int32_t> = 0>
				BSTSmartPointer(const BSTSmartPointer<Y>& rhs) : _ptr(rhs._ptr) { TryAttach(); }

				BSTSmartPointer(BSTSmartPointer&& rhs) noexcept(true) : _ptr(rhs._ptr) { rhs._ptr = nullptr; }

				template<class Y, std::enable_if_t<std::is_convertible_v<Y*, element_type*>, std::int32_t> = 0>
				BSTSmartPointer(BSTSmartPointer<Y>&& rhs) noexcept(true) : _ptr(rhs._ptr) { rhs._ptr = nullptr; }

				~BSTSmartPointer() { TryDetach(); }

				BSTSmartPointer& operator=(const BSTSmartPointer& rhs)
				{
					if (this != std::addressof(rhs))
					{
						TryDetach();
						_ptr = rhs._ptr;
						TryAttach();
					}
					return *this;
				}

				template<class Y, std::enable_if_t<std::is_convertible_v<Y*, element_type*>, std::int32_t> = 0>
				BSTSmartPointer& operator=(const BSTSmartPointer<Y>& rhs)
				{
					TryDetach();
					_ptr = rhs._ptr;
					TryAttach();
					return *this;
				}

				BSTSmartPointer& operator=(BSTSmartPointer&& rhs)
				{
					if (this != std::addressof(rhs))
					{
						TryDetach();
						_ptr = rhs._ptr;
						rhs._ptr = nullptr;
					}
					return *this;
				}

				template<class Y, std::enable_if_t<std::is_convertible_v<Y*, element_type*>, std::int32_t> = 0>
				BSTSmartPointer& operator=(BSTSmartPointer<Y>&& rhs)
				{
					TryDetach();
					_ptr = rhs._ptr;
					rhs._ptr = nullptr;
					return *this;
				}

				void reset() { TryDetach(); }

				template<class Y, std::enable_if_t<std::is_convertible_v<Y*, element_type*>, std::int32_t> = 0>
				void reset(Y* ptr)
				{
					if (_ptr != ptr)
					{
						TryDetach();
						_ptr = ptr;
						TryAttach();
					}
				}

				[[nodiscard]] constexpr element_type* get() const noexcept(true) { return _ptr; }
				[[nodiscard]] explicit constexpr operator bool() const noexcept(true) { return static_cast<bool>(_ptr); }
				[[nodiscard]] constexpr element_type& operator*() const noexcept(true)
				{
					CKPE_ASSERT(static_cast<bool>(*this));
					return *_ptr;
				}

				[[nodiscard]] constexpr element_type* operator->() const noexcept(true)
				{
					CKPE_ASSERT(static_cast<bool>(*this));
					return _ptr;
				}

			protected:
				template<class, template<class> class>
				friend class BSTSmartPointer;

				void TryAttach() { if (_ptr) reference_manager::Acquire(_ptr); }
				void TryDetach()
				{
					if (_ptr)
					{
						reference_manager::Release(_ptr);
						_ptr = nullptr;
					}
				}

				element_type* _ptr{ nullptr };
			};
			static_assert(sizeof(BSTSmartPointer<void*>) == 0x8);

			template<class T, class... Args>
			[[nodiscard]] BSTSmartPointer<T> make_smart(Args&&... args)
			{
				return BSTSmartPointer<T>{ new T(std::forward<Args>(args)...) };
			}

			template<class T1, class T2, template<class> class R>
			[[nodiscard]] constexpr bool operator==(const BSTSmartPointer<T1, R>& lhs, const BSTSmartPointer<T2, R>& rhs)
			{
				return lhs.get() == rhs.get();
			}

			template<class T, template<class> class R>
			[[nodiscard]] constexpr bool operator==(const BSTSmartPointer<T, R>& lhs, std::nullptr_t) noexcept(true)
			{
				return !lhs;
			}

			template<class T>
			BSTSmartPointer(T*) -> BSTSmartPointer<T, BSTSmartPointerIntrusiveRefCount>;

			template<class T>
			using BSTAutoPointer = BSTSmartPointer<T, BSTSmartPointerAutoPtr>;
			static_assert(sizeof(BSTAutoPointer<void*>) == 0x8);

			template<class T, template <class> class R>
			struct BSCRC32<BSTSmartPointer<T, R>>
			{
			public:
				[[nodiscard]] std::uint32_t operator()(const BSTSmartPointer<T, R>& ptr) const noexcept(true)
				{
					return BSCRC32<const void*>()(ptr.get());
				}
			};
		}
	}
}
