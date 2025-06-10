// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#ifdef CKPE_EXPORTS
#	define CKPE_API __declspec(dllexport)
#else
#	define CKPE_API __declspec(dllimport)
#endif

#ifndef CKPE_PROPERTY
#define CKPE_PROPERTY(read_func, write_func) __declspec(property(get = read_func, put = write_func))
#endif // !CKPE_PROPERTY

#ifndef CKPE_READ_PROPERTY
#define CKPE_READ_PROPERTY(read_func) __declspec(property(get = read_func))
#endif // !CKPE_READ_PROPERTY

#ifndef CKPE_READ_PROPERTY2
#define CKPE_READ_PROPERTY2(type, Name, Var) \
	__forceinline type Get##Name(VOID) const { return Var; } \
	__declspec(property(get = Get##Name)) type Name
#endif // !CKPE_READ_PROPERTY2