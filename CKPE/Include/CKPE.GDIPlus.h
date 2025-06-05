// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <string>
#include <CKPE.Common.h>

namespace CKPE
{
	class CKPE_API GDIPlus
	{
		GDIPlus(const GDIPlus&) = delete;
		GDIPlus& operator=(const GDIPlus&) = delete;
	public:
		GDIPlus() noexcept(true);
		virtual ~GDIPlus() noexcept(true);

		[[nodiscard]] static const GDIPlus* GetSingleton() noexcept(true);
	};
}