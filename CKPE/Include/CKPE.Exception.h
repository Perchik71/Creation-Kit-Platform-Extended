// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.StringUtils.h>
#include <CKPE.ErrorHandler.h>
#include <stdexcept>
#include <string>
#include <format>

namespace CKPE
{
	class RuntimeError : public std::runtime_error 
	{
	public:
		template<class... _Types>
		RuntimeError(std::format_string<_Types...> _Fmt, _Types&&... _Args) :
			std::runtime_error(std::vformat(_Fmt.get(), std::make_format_args(_Args...)))
		{}
		template<class... _Types>
		RuntimeError(std::wformat_string<_Types...> _Fmt, _Types&&... _Args) :
			std::runtime_error(StringUtils::Utf16ToWinCP(std::vformat(_Fmt.get(), std::make_wformat_args(_Args...))))
		{}
	};

	class SystemError : public std::runtime_error
	{
	public:
		template<class... _Types>
		SystemError(std::int32_t error_code, std::format_string<_Types...> _Fmt, _Types&&... _Args) :
			std::runtime_error(std::vformat(_Fmt.get(), std::make_format_args(_Args...)) + ": " +
				ErrorHandler::GetSystemMessage(error_code))
		{}
		template<class... _Types>
		SystemError(std::int32_t error_code, std::wformat_string<_Types...> _Fmt, _Types&&... _Args) :
			std::runtime_error(StringUtils::Utf16ToWinCP(std::vformat(_Fmt.get(), std::make_wformat_args(_Args...))) + ": " +
				ErrorHandler::GetSystemMessage(error_code))
		{}
	};
}