// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Common.h>

namespace CKPE
{
	namespace Common
	{
		class CKPE_COMMON_API Interface
		{
			Interface(const Interface&) = delete;
			Interface& operator=(const Interface&) = delete;
		public:
			Interface() noexcept(true);
		};
	}
}