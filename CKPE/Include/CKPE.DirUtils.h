// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <string>
#include <cstdint>
#include <CKPE.Common.h>

namespace CKPE
{
	class CKPE_API DirUtils
	{
		constexpr DirUtils() noexcept(true) = default;
		DirUtils(const DirUtils&) = delete;
		DirUtils& operator=(const DirUtils&) = delete;
	public:
		static bool DirCreate(const std::wstring& path) noexcept(true);
	};
}