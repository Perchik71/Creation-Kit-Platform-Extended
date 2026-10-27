// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Common.h>

#include <cstdint>
#include <cstddef>
#include <initializer_list>

namespace CKPE
{
	namespace Common
	{
		// Stops the CK passing data (editor IDs) as a sprintf_s format string
		class CKPE_COMMON_API LiteralFormat
		{
			constexpr LiteralFormat() noexcept(true) = default;
			LiteralFormat(const LiteralFormat&) = delete;
			LiteralFormat& operator=(const LiteralFormat&) = delete;
		public:
			// Same shape as the CK's sprintf_s, but copies Text verbatim
			static std::int32_t Print(char* Buffer, std::size_t Size, const char* Text, ...) noexcept(true);
			static std::uint32_t Redirect(std::initializer_list<std::uintptr_t> Sites,
				std::uintptr_t Expected) noexcept(true);
		};
	}
}
