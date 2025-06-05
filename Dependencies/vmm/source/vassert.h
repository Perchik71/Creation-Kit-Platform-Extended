// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#ifdef _DEBUG

namespace voltek
{
	namespace core
	{
		namespace _internal
		{
			void assert(bool cond, const char* msg);
			void assert_format(bool cond, const char* fmt, ...);
		}
	}
}

#endif

#if !defined(NDEBUG)
#	define _vassert(cond) voltek::core::_internal::assert_format(cond, #cond"\nFile: %s\nLine: %d", __FILE__, __LINE__)
#	define _vassert_msg(cond, msg) voltek::core::_internal::assert_format(cond, msg"\nFile: %s\nLine: %d", __FILE__, __LINE__)
#	define _vassert_format(cond, fmt, ...) voltek::core::_internal::assert_format(cond, fmt"\nFile: %s\nLine: %d", ##__VA_ARGS__, __FILE__, __LINE__)
#else
#	define _vassert(cond) ((void)0)
#	define _vassert_msg(cond, msg) ((void)0)
#	define _vassert_format(cond, fmt, ...) ((void)0)
#endif