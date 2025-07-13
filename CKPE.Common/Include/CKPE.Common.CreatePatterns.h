// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Common.h>
#include <CKPE.Patterns.h>
#include <string>
#include <cstdint>


namespace CKPE
{
	namespace Common
	{
		CKPE_COMMON_API std::string ZydisCreateMask(std::uintptr_t start_address, std::size_t size,
			Patterns::CreateFlag flag = Patterns::DEFAULT_MASK) noexcept(true);
	}
}