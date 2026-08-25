// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <intrin.h>

namespace voltek
{
	inline static int ctzll(unsigned long long m) noexcept
	{
#if defined(_MSC_VER)
		unsigned long where;
#if (defined(_M_AMD64) || defined(__x86_64__))
		if (_BitScanForward64(&where, m))
			return static_cast<int>(where);
#else
		if (_BitScanForward(&where, (unsigned long)m))
			return (int)where;
		if (_BitScanForward(&where, (unsigned long)(m >> 32)))
			return (int)where + 32;
#endif
#else
		return (m > 0) ? static_cast<int>(__builtin_ctzll(m)) : 64;
#endif
		return 64;
	}

	inline static int ctzl(unsigned long m) noexcept
	{
#if defined(_MSC_VER)
		unsigned long where;
		if (_BitScanForward(&where, m))
			return static_cast<int>(where);
		return 32;
#else
		return (m > 0) ? static_cast<int>(__builtin_ctzl(m)) : 32;
#endif
	}

	inline static int ctz(unsigned long m) noexcept
	{
		return ctzl(m);
	}
}
