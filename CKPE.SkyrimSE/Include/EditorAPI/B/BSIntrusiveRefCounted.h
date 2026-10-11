#pragma once

#include <cstdint>
#include <atomic>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			class BSIntrusiveRefCounted
			{
			public:
				template <class>
				friend struct BSTSmartPointerIntrusiveRefCount;

				inline std::uint32_t IncRef() const noexcept(true)
				{
					std::atomic_ref myRefCount{ _refCount };
					return ++myRefCount;
				}

				inline std::uint32_t DecRef() const noexcept(true)
				{
					std::atomic_ref myRefCount{ _refCount };
					return --myRefCount;
				}

				[[nodiscard]] inline std::uint32_t GetRefCount() const noexcept(true)
				{
					return _refCount;
				}
			protected:
				// members
				mutable volatile std::uint32_t _refCount{ 0 };
			};
			static_assert(sizeof(BSIntrusiveRefCounted) == 0x4);
		}
	}
}