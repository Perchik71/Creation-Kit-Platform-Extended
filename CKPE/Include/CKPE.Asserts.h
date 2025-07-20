// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdarg>
#include <CKPE.Common.h>

namespace CKPE
{
	class CKPE_API Asserts
	{
		constexpr Asserts() noexcept(true) = default;
		Asserts(const Asserts&) = delete;
		Asserts& operator=(const Asserts&) = delete;
	public:
		static void Trigger(const char* source_file, int source_line, const char* formatted_message, ...) noexcept(true);
	};
}

#define CKPE_ASSERT(Cond)					if(!(Cond)) Asserts::Trigger(__FILE__, __LINE__, #Cond);
#define CKPE_ASSERT_MSG_FMT(Cond, Msg, ...)	if(!(Cond)) Asserts::Trigger(__FILE__, __LINE__, "%s\n\n" Msg, #Cond, ##__VA_ARGS__);
#define CKPE_ASSERT_MSG(Cond, Msg)			CKPE_ASSERT_MSG_FMT(Cond, Msg)