// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <string>
#include <cstdint>
#include <CKPE.Common.h>

namespace CKPE
{
	class CKPE_API Keyboard
	{
		constexpr Keyboard() noexcept(true) = default;
		Keyboard(const Keyboard&) = delete;
		Keyboard& operator=(const Keyboard&) = delete;
	public:
		[[nodiscard]] static bool IsShiftPressed() noexcept(true);
		[[nodiscard]] static bool IsControlPressed() noexcept(true);
		[[nodiscard]] static bool IsAltPressed() noexcept(true);
		[[nodiscard]] static bool IsLeftShiftPressed() noexcept(true);
		[[nodiscard]] static bool IsLeftControlPressed() noexcept(true);
		[[nodiscard]] static bool IsLeftAltPressed() noexcept(true);
		[[nodiscard]] static bool IsRightShiftPressed() noexcept(true);
		[[nodiscard]] static bool IsRightControlPressed() noexcept(true);
		[[nodiscard]] static bool IsRightAltPressed() noexcept(true);
	};
}