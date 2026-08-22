// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Application.h>
#include <CKPE.Common.AddressLibrary.h>

namespace CKPE
{
	namespace Common
	{
		class CKPE_COMMON_API ID
		{
			std::uint64_t m_ids[SUPPORT_RUNTIMECOUNT]{ 0 };
		public:
			static_assert(SUPPORT_RUNTIMECOUNT > 0, "SUPPORT_RUNTIMECOUNT must be at least 1.");

			constexpr ID() noexcept(true) = default;

			explicit constexpr ID(std::uint64_t a_id) noexcept(true)
			{
				for (auto& id : m_ids)
					id = a_id;
			}

			explicit constexpr ID(std::initializer_list<std::uint64_t> a_list) noexcept(true)
			{
				if (!a_list.size())
					return;

				std::size_t i = 0;
				std::uint64_t lastValue = 0;

				for (auto val : a_list)
				{
					if (i >= SUPPORT_RUNTIMECOUNT)
						break;

					m_ids[i++] = val;
					lastValue = val;
				}

				while (i < SUPPORT_RUNTIMECOUNT)
					m_ids[i++] = lastValue;
			}

			constexpr ID& operator=(std::uint64_t a_id) noexcept(true)
			{
				for (auto& id : m_ids)
					id = a_id;
				return *this;
			}

			[[nodiscard]] std::uintptr_t Address() const noexcept(true)
			{
				return Application::GetSingleton()->GetBase() + Offset();
			}

			[[nodiscard]] std::uint64_t Id() const noexcept(true)
			{
				auto index = IDDB::GetSingleton()->GetRuntimeIndex();
				if (index >= SUPPORT_RUNTIMECOUNT)
					index = SUPPORT_RUNTIMECOUNT - 1;
				return m_ids[index];
			}

			[[nodiscard]] std::size_t Offset() const noexcept(true)
			{
				const auto iddb = IDDB::GetSingleton();
				return iddb->ResolveOffset(Id());
			}
		};
	}
}