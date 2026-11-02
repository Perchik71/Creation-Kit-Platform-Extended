// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <type_traits>
#include <CKPE.Application.h>
#include <CKPE.Common.ID.h>
#include <CKPE.Common.RTTI.h>

// The counterpart of CommonLibF4's fallout_cast / RTTI for the Creation Kit classes of EditorAPI.
// A class that has `constexpr static Common::ID RTTI` (the type descriptor, IDs_RTTI.h) and
// `constexpr static Common::ID VTABLE` (IDs_VTABLE.h) can be used with ckpe_cast.

namespace CKPE
{
	namespace Common
	{
		// Address of the type descriptor: by its RTTI ID, if the address library has it, else from the COL of the vtable.
		[[nodiscard]] inline const void* GetTypeDescriptor(const ID& rtti, const ID& vtable) noexcept(true)
		{
			if (rtti.Id())
				return reinterpret_cast<const void*>(rtti.Address());

			if (vtable.Id())
			{
				// The pointer to the COL is placed right before the vtable, the COL holds the RVA of the type descriptor.
				const auto col = *reinterpret_cast<const std::uintptr_t*>(vtable.Address() - sizeof(std::uintptr_t));
				if (col)
					return reinterpret_cast<const void*>(
						Application::GetSingleton()->GetBase() + *reinterpret_cast<const std::uint32_t*>(col + 12));
			}

			return nullptr;
		}

		namespace detail
		{
			template <class T>
			using remove_cvpr_t = std::remove_cv_t<std::remove_pointer_t<std::remove_reference_t<T>>>;

			template <class T, class = void>
			struct implements_rtti : std::false_type {};

			template <class T>
			struct implements_rtti<T, std::void_t<decltype(remove_cvpr_t<T>::RTTI), decltype(remove_cvpr_t<T>::VTABLE)>> :
				std::true_type {};

			template <class To, class From>
			struct cast_is_valid : std::conjunction<
				std::is_pointer<To>,
				std::is_polymorphic<remove_cvpr_t<To>>,
				implements_rtti<To>,
				implements_rtti<From>> {};

			template <class T>
			[[nodiscard]] inline const void* TypeDescriptorOf() noexcept(true)
			{
				using type = remove_cvpr_t<T>;
				static const void* const descriptor = GetTypeDescriptor(type::RTTI, type::VTABLE);
				return descriptor;
			}
		}

		// Dynamic cast with the RTTI of the Creation Kit, returns nullptr if the object is not of the type To.
		template <class To, class From>
		[[nodiscard]] inline To ckpe_cast(From* from) noexcept(true)
			requires(detail::cast_is_valid<To, From*>::value)
		{
			return static_cast<To>(RTTI::DynamicCast(
				const_cast<void*>(static_cast<const volatile void*>(from)), 0,
				detail::TypeDescriptorOf<From>(), detail::TypeDescriptorOf<To>(), false));
		}
	}
}