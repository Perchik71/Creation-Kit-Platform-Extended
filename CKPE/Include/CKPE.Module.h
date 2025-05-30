// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <cstdint>
#include <CKPE.Common.h>

//	x-------	major
//	-xxx----	minor
//	----xxxx	build

#define MAKE_EXE_VERSION(major, minor, build)	((((major) & 0xF) << 28) | (((minor) & 0xFFF) << 16) | ((build) & 0xFFFF))

#define GET_EXE_VERSION_MAJOR(a)	(((a) & 0xF0000000) >> 28)
#define GET_EXE_VERSION_MINOR(a)	(((a) & 0x0FFF0000) >> 16)
#define GET_EXE_VERSION_BUILD(a)	(((a) & 0x0000FFFF))

namespace CKPE
{
	class CKPE_API Module
	{
		const void* _handle{ nullptr };

		Module(const Module&) = delete;
		Module& operator=(const Module&) = delete;
	public:
		constexpr Module() noexcept(true) = default;
		Module(const void* m) noexcept(true);

		bool Is64() noexcept(true);
		const void* GetResourceLibraryViaProcAddress(const char* exportName) noexcept(true);
	};
}
