// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.Interface.h>
#include <CKPE.Common.Relocation.h>
#include <CKPE.Common.LiteralFormat.h>
#include <cstdio>

namespace CKPE
{
	namespace Common
	{
		std::int32_t LiteralFormat::Print(char* Buffer, std::size_t Size, const char* Text, ...) noexcept(true)
		{
			if (!Buffer || !Size)
				return 0;

			return _snprintf_s(Buffer, Size, _TRUNCATE, "%s", Text ? Text : "");
		}

		std::uint32_t LiteralFormat::Redirect(std::initializer_list<std::uintptr_t> Sites,
			std::uintptr_t Expected) noexcept(true)
		{
			std::uint32_t patched = 0;

			for (auto site : Sites)
			{
				if (Relocation(site).RedirectBranch(Expected, &Print))
					patched++;
				else
					_WARNING("LiteralFormat: unexpected code at %016llX, skipped", site);
			}

			return patched;
		}
	}
}
