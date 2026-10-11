#pragma once

#include <atomic>
#include <cstdint>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			template <class T>
			class BSTAtomicValue
			{
			public:
				static_assert(std::is_integral_v<T>);

				using value_type = T;

				constexpr BSTAtomicValue() noexcept = default;

				explicit constexpr BSTAtomicValue(T a_rhs) noexcept :
					_value(a_rhs)
				{}

				T operator++()
				{
					std::atomic_ref value{ _value };
					return ++value;
				}

				[[nodiscard]] T operator++(int)
				{
					std::atomic_ref value{ _value };
					return value++;
				}

				T operator--()
				{
					std::atomic_ref value{ _value };
					return --value;
				}

				[[nodiscard]] T operator--(int)
				{
					std::atomic_ref value{ _value };
					return value--;
				}

				[[nodiscard]] value_type& load_unchecked() noexcept { return _value; }
				[[nodiscard]] const value_type& load_unchecked() const noexcept { return _value; }

			private:
				// members
				value_type _value{ 0 };  // 0
			};

			extern template class BSTAtomicValue<std::int32_t>;
			extern template class BSTAtomicValue<std::uint32_t>;
			extern template class BSTAtomicValue<std::int64_t>;
			extern template class BSTAtomicValue<std::uint64_t>;

			class BSEventFlag
			{
			public:
				// members
				void* event{ nullptr };  // 0
			};
			static_assert(sizeof(BSEventFlag) == 0x8);
		}
	}
}
