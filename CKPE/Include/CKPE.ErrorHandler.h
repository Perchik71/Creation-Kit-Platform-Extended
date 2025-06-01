// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <string>
#include <cstdint>
#include <CKPE.Common.h>

namespace CKPE
{
	class CKPE_API ErrorHandler
	{
		constexpr ErrorHandler() noexcept(true) = default;
		ErrorHandler(const ErrorHandler&) = delete;
		ErrorHandler& operator=(const ErrorHandler&) = delete;
	public:
		static void Trigger(const std::string& message) noexcept(true);
		static void Trigger(const std::string& message, std::ptrdiff_t error_code) noexcept(true);
		static std::string GetSystemMessage(std::ptrdiff_t error_code) noexcept(true);
	};
}