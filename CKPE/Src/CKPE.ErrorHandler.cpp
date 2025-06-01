// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <comdef.h>
#include <CKPE.ErrorHandler.h>
#include <CKPE.MessageBox.h>
#include <format>

namespace CKPE
{
	void ErrorHandler::Trigger(const std::string& message) noexcept(true)
	{
		MessageBox::OpenError(message);
		if (!TerminateProcess(GetCurrentProcess(), EXIT_FAILURE))
			abort();
		__assume(0);
	}

	void ErrorHandler::Trigger(const std::string& message, std::ptrdiff_t error_code) noexcept(true)
	{
		std::string system_msg = GetSystemMessage(error_code);
		Trigger(std::vformat("{}\n\nError code: 0x{:x}\nSystem message: \"{}\"",
			std::make_format_args(message, error_code, system_msg)));
	}

	std::string ErrorHandler::GetSystemMessage(std::ptrdiff_t error_code) noexcept(true)
	{
		return _com_error((HRESULT)error_code).ErrorMessage();
	}
}