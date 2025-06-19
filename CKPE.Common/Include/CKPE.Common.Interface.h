// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.GameManager.h>
#include <CKPE.Logger.h>
#include <CKPE.CommandLineParser.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.SettingCollection.h>
#include <CKPE.Common.LogWindow.h>
#include <string_view>
#include <string>
#include <format>

namespace CKPE
{
	namespace Common
	{
		class CKPE_COMMON_API Interface
		{
			const CKPEGameLibraryInterface* _interface{ nullptr };
			TOMLSettingCollection* _settings{ nullptr };
			TOMLSettingCollection* _theme_settings{ nullptr };
			CommandLineParser* _cmdline{ nullptr };
			std::uint64_t _version{ 0 };
			std::uintptr_t _instDll{ 0 };

			Interface(const Interface&) = delete;
			Interface& operator=(const Interface&) = delete;
		public:
			constexpr Interface() noexcept(true) = default;
			virtual ~Interface() noexcept(true);

			void Initialize(const CKPEGameLibraryInterface* a_interface, std::uint64_t a_version, 
				const std::wstring& a_dialogs_fn, const std::wstring& a_databases_fn, const std::wstring& a_resources_fn, 
				bool support_more_theme = true) noexcept(true); 
			void CmdLineHandler();

			[[nodiscard]] bool HasCustomThemeSetting() const noexcept(true);

			[[nodiscard]] static Interface* GetSingleton() noexcept(true);
			[[nodiscard]] constexpr inline std::uintptr_t GetInstanceDLL() const noexcept(true) { return _instDll; }
			[[nodiscard]] constexpr inline const Logger* GetLogger() const noexcept(true) { return _interface->logger; }
			[[nodiscard]] constexpr inline Application* GetApplication() const noexcept(true) { return _interface->application; }
			[[nodiscard]] constexpr inline TOMLSettingCollection* GetSettings() const noexcept(true)
			{ return _settings; }
			[[nodiscard]] constexpr inline TOMLSettingCollection* GetCustomThemeSettings() const noexcept(true)
			{ return _theme_settings; }
			[[nodiscard]] constexpr inline CommandLineParser* GetCommandLineParser() const noexcept(true)
			{ return _cmdline; }
		};
	}

#ifdef CKPE_NO_LOGGER_FUNCTION
	CKPE_COMMON_API void _FATALERROR(const std::string_view& formatted_message, ...);
	CKPE_COMMON_API void _ERROR(const std::string_view& formatted_message, ...);
	CKPE_COMMON_API void _WARNING(const std::string_view& formatted_message, ...);
	CKPE_COMMON_API void _MESSAGE(const std::string_view& formatted_message, ...);
	CKPE_COMMON_API void _FATALERROR(const std::wstring_view& formatted_message, ...);
	CKPE_COMMON_API void _ERROR(const std::wstring_view& formatted_message, ...);
	CKPE_COMMON_API void _WARNING(const std::wstring_view& formatted_message, ...);
	CKPE_COMMON_API void _MESSAGE(const std::wstring_view& formatted_message, ...);

	template<class... _Types>
	void _FATALERROR_EX(const std::format_string<_Types...> _Fmt, _Types&&... _Args)
	{
		Common::Interface::GetSingleton()->GetLogger()->WriteString(Logger::tFatalError,
			std::vformat(_Fmt.get(), std::make_format_args(_Args...)));
	}
	template<class... _Types>
	void _ERROR_EX(const std::format_string<_Types...> _Fmt, _Types&&... _Args)
	{
		Common::Interface::GetSingleton()->GetLogger()->WriteString(Logger::tError,
			std::vformat(_Fmt.get(), std::make_format_args(_Args...)));
	}
	template<class... _Types>
	void _WARNING_EX(const std::format_string<_Types...> _Fmt, _Types&&... _Args)
	{
		Common::Interface::GetSingleton()->GetLogger()->WriteString(Logger::tWarning,
			std::vformat(_Fmt.get(), std::make_format_args(_Args...)));
	}
	template<class... _Types>
	void _MESSAGE_EX(const std::format_string<_Types...> _Fmt, _Types&&... _Args)
	{
		Common::Interface::GetSingleton()->GetLogger()->WriteString(Logger::tMessage,
			std::vformat(_Fmt.get(), std::make_format_args(_Args...)));
	}
	template<class... _Types>
	void _FATALERROR_EX(const std::wformat_string<_Types...> _Fmt, _Types&&... _Args)
	{
		Common::Interface::GetSingleton()->GetLogger()->WriteString(Logger::tFatalError,
			std::vformat(_Fmt.get(), std::make_wformat_args(_Args...)));
	}
	template<class... _Types>
	void _ERROR_EX(const std::wformat_string<_Types...> _Fmt, _Types&&... _Args)
	{
		Common::Interface::GetSingleton()->GetLogger()->WriteString(Logger::tError,
			std::vformat(_Fmt.get(), std::make_wformat_args(_Args...)));
	}
	template<class... _Types>
	void _WARNING_EX(const std::wformat_string<_Types...> _Fmt, _Types&&... _Args)
	{
		Common::Interface::GetSingleton()->GetLogger()->WriteString(Logger::tWarning,
			std::vformat(_Fmt.get(), std::make_wformat_args(_Args...)));
	}
	template<class... _Types>
	void _MESSAGE_EX(const std::wformat_string<_Types...> _Fmt, _Types&&... _Args)
	{
		Common::Interface::GetSingleton()->GetLogger()->WriteString(Logger::tMessage,
			std::vformat(_Fmt.get(), std::make_wformat_args(_Args...)));
	}
#endif // !CKPE_NO_LOGGER_FUNCTION

	CKPE_COMMON_API bool _READ_OPTION_BOOL(const std::string& section, const std::string& option, bool def);
	CKPE_COMMON_API char _READ_OPTION_CHAR(const std::string& section, const std::string& option, char def);
	CKPE_COMMON_API long _READ_OPTION_INT(const std::string& section, const std::string& option, long def);
	CKPE_COMMON_API unsigned long _READ_OPTION_UINT(const std::string& section, const std::string& option, unsigned long def);
	CKPE_COMMON_API unsigned long _READ_OPTION_HEX(const std::string& section, const std::string& option, unsigned long def);
	CKPE_COMMON_API float _READ_OPTION_FLOAT(const std::string& section, const std::string& option, float def);
	CKPE_COMMON_API Common::CustomSettingCollection::color_value _READ_OPTION_RGB(const std::string& section,
		const std::string& option, Common::CustomSettingCollection::color_value def);
	CKPE_COMMON_API Common::CustomSettingCollection::color_value _READ_OPTION_RGBA(const std::string& section,
		const std::string& option, Common::CustomSettingCollection::color_value def);
	CKPE_COMMON_API std::string _READ_OPTION_STR(const std::string& section, const std::string& option, const std::string& def);
	CKPE_COMMON_API std::wstring _READ_OPTION_USTR(const std::string& section, const std::string& option, const std::wstring& def);
	CKPE_COMMON_API void _WRITE_OPTION_BOOL(const std::string& section, const std::string& option, bool value);
	CKPE_COMMON_API void _WRITE_OPTION_CHAR(const std::string& section, const std::string& option, char value);
	CKPE_COMMON_API void _WRITE_OPTION_INT(const std::string& section, const std::string& option, long value);
	CKPE_COMMON_API void _WRITE_OPTION_UINT(const std::string& section, const std::string& option, unsigned long value);
	CKPE_COMMON_API void _WRITE_OPTION_HEX(const std::string& section, const std::string& option, unsigned long value);
	CKPE_COMMON_API void _WRITE_OPTION_FLOAT(const std::string& section, const std::string& option, float value);
	CKPE_COMMON_API void _WRITE_OPTION_RGB(const std::string& section, const std::string& option,
		Common::CustomSettingCollection::color_value value);
	CKPE_COMMON_API void _WRITE_OPTION_RGBA(const std::string& section, const std::string& option,
		Common::CustomSettingCollection::color_value value);
	CKPE_COMMON_API void _WRITE_OPTION_STR(const std::string& section, const std::string& option, const std::string& value);
	CKPE_COMMON_API void _WRITE_OPTION_USTR(const std::string& section, const std::string& option, const std::wstring& value);
}