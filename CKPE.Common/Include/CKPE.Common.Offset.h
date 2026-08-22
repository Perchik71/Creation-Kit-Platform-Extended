#pragma once

#include <CKPE.Application.h>
#include <CKPE.Common.AddressLibrary.h>

namespace CKPE
{
	namespace Common
	{
		class Offset
		{
			std::size_t m_offsets[SUPPORT_RUNTIMECOUNT]{ 0 };
		public:
			static_assert(SUPPORT_RUNTIMECOUNT > 0, "SUPPORT_RUNTIMECOUNT must be at least 1.");

			constexpr Offset() noexcept(true) = default;

			explicit constexpr Offset(std::size_t a_offset) noexcept(true)
			{
				for (auto& offset : m_offsets)
					offset = a_offset;
			}

			explicit constexpr Offset(std::initializer_list<std::size_t> a_list) noexcept(true)
			{
				if (!a_list.size())
					return;

				std::size_t i = 0;
				std::size_t lastValue = 0;

				for (auto val : a_list)
				{
					if (i >= SUPPORT_RUNTIMECOUNT)
						break;

					m_offsets[i++] = val;
					lastValue = val;
				}

				while (i < SUPPORT_RUNTIMECOUNT)
					m_offsets[i++] = lastValue;
			}

			constexpr Offset& operator=(std::size_t a_offset) noexcept(true)
			{
				for (auto& offset : m_offsets)
					offset = a_offset;
				return *this;
			}

			[[nodiscard]] std::uintptr_t Address() const noexcept(true)
			{
				return Application::GetSingleton()->GetBase() + Value();
			}

			[[nodiscard]] std::size_t Value() const noexcept(true)
			{
				auto index = IDDB::GetSingleton()->GetRuntimeIndex();
				if (index >= SUPPORT_RUNTIMECOUNT)
					index = SUPPORT_RUNTIMECOUNT - 1;
				return m_offsets[index];
			}
		};
	}
}