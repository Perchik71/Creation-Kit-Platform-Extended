// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <string>
#include <string_view>
#include <cstdint>
#include <format>
#include <CKPE.Common.h>
#include <CKPE.CriticalSection.h>

namespace CKPE
{
	class CKPE_API Logger
	{
		void* _handle{ nullptr };
		std::wstring* _fname{ nullptr };
		CriticalSection _section;
	public:
		enum Setting : std::uint32_t {
			sAutoFlush							= 1 << 0,
			sAlwaysNewLine						= 1 << 1,
			sIfFatalErrorTriggerErrorHandler	= 1 << 2,
			sOutputDebugger						= 1 << 3,
		};

		enum TypeMsg
		{
			tMessage = 0,
			tWarning,
			tError,
			tFatalError
		};

		Logger() noexcept(true) = default;
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;
		~Logger();

		[[nodiscard]] inline constexpr virtual bool HasOpen() const noexcept(true) { return _handle != nullptr; }
		[[nodiscard]] inline constexpr virtual bool GetHandle() const noexcept(true) { return _handle; }
		[[nodiscard]] inline constexpr virtual const wchar_t* GetFileName() const noexcept(true) 
		{ return _fname ? _fname->c_str() : nullptr; }

		bool Open(const std::string& fname) noexcept(true);
		bool Open(const std::wstring& fname) noexcept(true);
		void Close() noexcept(true);
		[[nodiscard]] static const Logger* GetSingleton() noexcept(true);

		virtual void Write(const std::string& formatted_message, ...) const noexcept(true);
		virtual void Write(const std::wstring& formatted_message, ...) const noexcept(true);
		virtual void Write(TypeMsg type_msg, const std::string& formatted_message, ...) const noexcept(true);
		virtual void Write(TypeMsg type_msg, const std::wstring& formatted_message, ...) const noexcept(true);

		template<class... _Types>
		inline void WriteEx(const std::format_string<_Types...> _Fmt, _Types&&... _Args) const noexcept(true)
		{
			WriteString(TypeMsg::tMessage, std::vformat(_Fmt.get(), std::make_format_args(_Args...)));
		}

		template<class... _Types>
		void WriteEx(const std::wformat_string<_Types...> _Fmt, _Types&&... _Args) const noexcept(true)
		{
			WriteString(TypeMsg::tMessage, std::vformat(_Fmt.get(), std::make_wformat_args(_Args...)));
		}

		template<class... _Types>
		void WriteEx(TypeMsg type_msg, const std::format_string<_Types...> _Fmt, _Types&&... _Args) const noexcept(true)
		{
			WriteString(type_msg, std::vformat(_Fmt.get(), std::make_format_args(_Args...)));
		}

		template<class... _Types>
		void WriteEx(TypeMsg type_msg, const std::wformat_string<_Types...> _Fmt, _Types&&... _Args) const noexcept(true)
		{
			WriteString(type_msg, std::vformat(_Fmt.get(), std::make_wformat_args(_Args...)));
		}

		void WriteString(TypeMsg type_msg, const std::string& message) const noexcept(true);
		void WriteString(TypeMsg type_msg, const std::wstring& message) const noexcept(true);

		virtual void Flush() const noexcept(true);
		virtual void NewLine() const noexcept(true);

		inline virtual void SetSettings(std::uint32_t new_settings) noexcept(true) { _settings = new_settings; };
		[[nodiscard]] inline constexpr virtual bool HasAutoFlush() const noexcept(true) { return _settings & sAutoFlush; }
		[[nodiscard]] inline constexpr virtual bool HasAlwaysNewLine() const noexcept(true) { return _settings & sAlwaysNewLine; }
		[[nodiscard]] inline constexpr virtual bool HasIfFatalErrorTriggerErrorHandler() const noexcept(true) { return _settings & sIfFatalErrorTriggerErrorHandler; }
		[[nodiscard]] inline constexpr virtual bool HasOutputDebugger() const noexcept(true) { return _settings & sOutputDebugger; }
	private:
		std::uint32_t _settings{ sAutoFlush | sAlwaysNewLine | sIfFatalErrorTriggerErrorHandler };
	};

#ifndef CKPE_NO_LOGGER_FUNCTION
	void CKPE_API _FATALERROR(const std::string_view& formatted_message, ...);
	void CKPE_API _ERROR(const std::string_view& formatted_message, ...);
	void CKPE_API _WARNING(const std::string_view& formatted_message, ...);
	void CKPE_API _MESSAGE(const std::string_view& formatted_message, ...);
	void CKPE_API _FATALERROR(const std::wstring_view& formatted_message, ...);
	void CKPE_API _ERROR(const std::wstring_view& formatted_message, ...);
	void CKPE_API _WARNING(const std::wstring_view& formatted_message, ...);
	void CKPE_API _MESSAGE(const std::wstring_view& formatted_message, ...);

	template<class... _Types>
	void _FATALERROR_EX(const std::format_string<_Types...> _Fmt, _Types&&... _Args)
	{
		Logger::GetSingleton()->WriteString(Logger::tFatalError,
			std::vformat(_Fmt.get(), std::make_format_args(_Args...)));
	}
	template<class... _Types>
	void _ERROR_EX(const std::format_string<_Types...> _Fmt, _Types&&... _Args)
	{
		Logger::GetSingleton()->WriteString(Logger::tError,
			std::vformat(_Fmt.get(), std::make_format_args(_Args...)));
	}
	template<class... _Types>
	void _WARNING_EX(const std::format_string<_Types...> _Fmt, _Types&&... _Args)
	{
		Logger::GetSingleton()->WriteString(Logger::tWarning,
			std::vformat(_Fmt.get(), std::make_format_args(_Args...)));
	}
	template<class... _Types>
	void _MESSAGE_EX(const std::format_string<_Types...> _Fmt, _Types&&... _Args)
	{
		Logger::GetSingleton()->WriteString(Logger::tMessage,
			std::vformat(_Fmt.get(), std::make_format_args(_Args...)));
	}
	template<class... _Types>
	void _FATALERROR_EX(const std::wformat_string<_Types...> _Fmt, _Types&&... _Args)
	{
		Logger::GetSingleton()->WriteString(Logger::tFatalError,
			std::vformat(_Fmt.get(), std::make_wformat_args(_Args...)));
	}
	template<class... _Types>
	void _ERROR_EX(const std::wformat_string<_Types...> _Fmt, _Types&&... _Args)
	{
		Logger::GetSingleton()->WriteString(Logger::tError,
			std::vformat(_Fmt.get(), std::make_wformat_args(_Args...)));
	}
	template<class... _Types>
	void _WARNING_EX(const std::wformat_string<_Types...> _Fmt, _Types&&... _Args)
	{
		Logger::GetSingleton()->WriteString(Logger::tWarning,
			std::vformat(_Fmt.get(), std::make_wformat_args(_Args...)));
	}
	template<class... _Types>
	void _MESSAGE_EX(const std::wformat_string<_Types...> _Fmt, _Types&&... _Args)
	{
		Logger::GetSingleton()->WriteString(Logger::tMessage,
			std::vformat(_Fmt.get(), std::make_wformat_args(_Args...)));
	}
#endif // !CKPE_NO_LOGGER_FUNCTION
}