// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <cstdarg>
#include <CKPE.PathUtils.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.LogWindow.h>
#include <CKPE.Common.RTTI.h>

namespace CKPE
{
	namespace Common
	{
		static Interface _sinterface;

		constexpr static auto _ssettings_fname = L"CreationKitPlatformExtended.toml";
		constexpr static auto _stheme_settings_fname = L"CreationKitPlatformExtendedCustomTheme.toml";

		Interface::~Interface() noexcept(true)
		{
			LogWindow::Shutdown();

			if (_settings)
			{
				delete _settings;
				_settings = nullptr;
			}

			if (_theme_settings)
			{
				delete _theme_settings;
				_theme_settings = nullptr;
			}

			if (_cmdline)
			{
				delete _cmdline;
				_cmdline = nullptr;
			}
		}

		void Interface::Initialize(const CKPEGameLibraryInterface* a_interface) noexcept(true)
		{
			_interface = a_interface;
			_cmdline = new CommandLineParser;
			std::wstring spath = _interface->application->GetPath();
			_settings = new TOMLSettingCollection(spath + _ssettings_fname);
			_theme_settings = new TOMLSettingCollection(spath + _stheme_settings_fname);
			/* call constructor */ new LogWindow();
			RTTI::GetSingleton()->Initialize();
		}

		void Interface::CmdLineHandler()
		{
			CommandLineParser cmd;
			if (cmd.HasCommandRun() && _interface)
			{
				auto scmd = StringUtils::ToLower(cmd.GetCommand());
				if (scmd.compare(L"-PEExportRTTI"))
				{
					if (cmd.Count() != 2)
					{
						_ERROR("Invalid number of command arguments: %u", cmd.Count());
						_MESSAGE("Example: CreationKit -PEExportRTTI \"rtti.txt\"");
					}
					else
						RTTI::GetSingleton()->Dump(cmd[1]);

					// Закрываем Creation Kit
					_interface->application->Terminate();
				}
			}
		}

		Interface* Interface::GetSingleton() noexcept(true)
		{
			return &_sinterface;
		}
	}

#ifdef CKPE_NO_LOGGER_FUNCTION
	CKPE_COMMON_API void _FATALERROR(const std::string_view& formatted_message, ...)
	{
		va_list ap;
		va_start(ap, &formatted_message);
		auto len = _vscprintf(formatted_message.data(), (va_list)ap);
		if (len < 1) return;

		std::string string_done;
		string_done.resize((std::size_t)len);
		if (string_done.empty()) return;
		vsprintf(string_done.data(), formatted_message.data(), (va_list)ap);
		va_end(ap);

		Common::Interface::GetSingleton()->GetLogger()->WriteString(Logger::tFatalError, string_done);
	}

	CKPE_COMMON_API void _ERROR(const std::string_view& formatted_message, ...)
	{
		va_list ap;
		va_start(ap, &formatted_message);
		auto len = _vscprintf(formatted_message.data(), (va_list)ap);
		if (len < 1) return;

		std::string string_done;
		string_done.resize((std::size_t)len);
		if (string_done.empty()) return;
		vsprintf(string_done.data(), formatted_message.data(), (va_list)ap);
		va_end(ap);

		Common::Interface::GetSingleton()->GetLogger()->WriteString(Logger::tError, string_done);
	}

	CKPE_COMMON_API void _WARNING(const std::string_view& formatted_message, ...)
	{
		va_list ap;
		va_start(ap, &formatted_message);
		auto len = _vscprintf(formatted_message.data(), (va_list)ap);
		if (len < 1) return;

		std::string string_done;
		string_done.resize((std::size_t)len);
		if (string_done.empty()) return;
		vsprintf(string_done.data(), formatted_message.data(), (va_list)ap);
		va_end(ap);

		Common::Interface::GetSingleton()->GetLogger()->WriteString(Logger::tWarning, string_done);
	}

	CKPE_COMMON_API void _MESSAGE(const std::string_view& formatted_message, ...)
	{
		va_list ap;
		va_start(ap, &formatted_message);
		auto len = _vscprintf(formatted_message.data(), (va_list)ap);
		if (len < 1) return;

		std::string string_done;
		string_done.resize((std::size_t)len);
		if (string_done.empty()) return;
		vsprintf(string_done.data(), formatted_message.data(), (va_list)ap);
		va_end(ap);

		Common::Interface::GetSingleton()->GetLogger()->WriteString(Logger::tMessage, string_done);
	}

	CKPE_COMMON_API void _FATALERROR(const std::wstring_view& formatted_message, ...)
	{
		va_list ap;
		va_start(ap, &formatted_message);
		auto len = _vscwprintf(formatted_message.data(), (va_list)ap);
		if (len < 1) return;

		std::wstring string_done;
		string_done.resize((std::size_t)len);
		if (string_done.empty()) return;
		_vswprintf(string_done.data(), formatted_message.data(), (va_list)ap);
		va_end(ap);

		Common::Interface::GetSingleton()->GetLogger()->WriteString(Logger::tFatalError, StringUtils::Utf16ToUtf8(string_done));
	}

	CKPE_COMMON_API void _ERROR(const std::wstring_view& formatted_message, ...)
	{
		va_list ap;
		va_start(ap, &formatted_message);
		auto len = _vscwprintf(formatted_message.data(), (va_list)ap);
		if (len < 1) return;

		std::wstring string_done;
		string_done.resize((std::size_t)len);
		if (string_done.empty()) return;
		_vswprintf(string_done.data(), formatted_message.data(), (va_list)ap);
		va_end(ap);

		Common::Interface::GetSingleton()->GetLogger()->WriteString(Logger::tError, StringUtils::Utf16ToUtf8(string_done));
	}

	CKPE_COMMON_API void _WARNING(const std::wstring_view& formatted_message, ...)
	{
		va_list ap;
		va_start(ap, &formatted_message);
		auto len = _vscwprintf(formatted_message.data(), (va_list)ap);
		if (len < 1) return;

		std::wstring string_done;
		string_done.resize((std::size_t)len);
		if (string_done.empty()) return;
		_vswprintf(string_done.data(), formatted_message.data(), (va_list)ap);
		va_end(ap);

		Common::Interface::GetSingleton()->GetLogger()->WriteString(Logger::tWarning, StringUtils::Utf16ToUtf8(string_done));
	}

	CKPE_COMMON_API void _MESSAGE(const std::wstring_view& formatted_message, ...)
	{
		va_list ap;
		va_start(ap, &formatted_message);
		auto len = _vscwprintf(formatted_message.data(), (va_list)ap);
		if (len < 1) return;

		std::wstring string_done;
		string_done.resize((std::size_t)len);
		if (string_done.empty()) return;
		_vswprintf(string_done.data(), formatted_message.data(), (va_list)ap);
		va_end(ap);

		Common::Interface::GetSingleton()->GetLogger()->WriteString(Logger::tMessage, StringUtils::Utf16ToUtf8(string_done));
	}
#endif // !CKPE_NO_LOGGER_FUNCTION

	CKPE_COMMON_API bool _READ_OPTION_BOOL(const std::string& section, const std::string& option, bool def)
	{
		return Common::Interface::GetSingleton()->GetSettings()->ReadBool(section, option, def);
	}

	CKPE_COMMON_API char _READ_OPTION_CHAR(const std::string& section, const std::string& option, char def)
	{
		return Common::Interface::GetSingleton()->GetSettings()->ReadChar(section, option, def);
	}

	CKPE_COMMON_API long _READ_OPTION_INT(const std::string& section, const std::string& option, long def)
	{
		return Common::Interface::GetSingleton()->GetSettings()->ReadInt(section, option, def);
	}

	CKPE_COMMON_API unsigned long _READ_OPTION_UINT(const std::string& section, const std::string& option, unsigned long def)
	{
		return Common::Interface::GetSingleton()->GetSettings()->ReadUInt(section, option, def);
	}

	CKPE_COMMON_API unsigned long _READ_OPTION_HEX(const std::string& section, const std::string& option, unsigned long def)
	{
		return Common::Interface::GetSingleton()->GetSettings()->ReadHex(section, option, def);
	}

	CKPE_COMMON_API float _READ_OPTION_FLOAT(const std::string& section, const std::string& option, float def)
	{
		return Common::Interface::GetSingleton()->GetSettings()->ReadFloat(section, option, def);
	}

	CKPE_COMMON_API Common::CustomSettingCollection::color_value _READ_OPTION_RGB(const std::string& section,
		const std::string& option, Common::CustomSettingCollection::color_value def)
	{
		return Common::Interface::GetSingleton()->GetSettings()->ReadRgbColor(section, option, def);
	}

	CKPE_COMMON_API Common::CustomSettingCollection::color_value _READ_OPTION_RGBA(const std::string& section,
		const std::string& option, Common::CustomSettingCollection::color_value def)
	{
		return Common::Interface::GetSingleton()->GetSettings()->ReadRgbaColor(section, option, def);
	}

	CKPE_COMMON_API std::string _READ_OPTION_STR(const std::string& section, const std::string& option,
		const std::string& def)
	{
		return Common::Interface::GetSingleton()->GetSettings()->ReadString(section, option, def);
	}

	CKPE_COMMON_API std::wstring _READ_OPTION_USTR(const std::string& section, const std::string& option,
		const std::wstring& def)
	{
		return Common::Interface::GetSingleton()->GetSettings()->ReadUnicodeString(section, option, def);
	}

	CKPE_COMMON_API void _WRITE_OPTION_BOOL(const std::string& section, const std::string& option, bool value)
	{
		Common::Interface::GetSingleton()->GetSettings()->WriteBool(section, option, value);
	}

	CKPE_COMMON_API void _WRITE_OPTION_CHAR(const std::string& section, const std::string& option, char value)
	{
		Common::Interface::GetSingleton()->GetSettings()->WriteChar(section, option, value);
	}

	CKPE_COMMON_API void _WRITE_OPTION_INT(const std::string& section, const std::string& option, long value)
	{
		Common::Interface::GetSingleton()->GetSettings()->WriteInt(section, option, value);
	}

	CKPE_COMMON_API void _WRITE_OPTION_UINT(const std::string& section, const std::string& option, unsigned long value)
	{
		Common::Interface::GetSingleton()->GetSettings()->WriteUInt(section, option, value);
	}

	CKPE_COMMON_API void _WRITE_OPTION_HEX(const std::string& section, const std::string& option, unsigned long value)
	{
		Common::Interface::GetSingleton()->GetSettings()->WriteHex(section, option, value);
	}

	CKPE_COMMON_API void _WRITE_OPTION_FLOAT(const std::string& section, const std::string& option, float value)
	{
		Common::Interface::GetSingleton()->GetSettings()->WriteFloat(section, option, value);
	}

	CKPE_COMMON_API void _WRITE_OPTION_RGB(const std::string& section, const std::string& option,
		Common::CustomSettingCollection::color_value value)
	{
		Common::Interface::GetSingleton()->GetSettings()->WriteRgbColor(section, option, value);
	}

	CKPE_COMMON_API void _WRITE_OPTION_RGBA(const std::string& section, const std::string& option,
		Common::CustomSettingCollection::color_value value)
	{
		Common::Interface::GetSingleton()->GetSettings()->WriteRgbaColor(section, option, value);
	}

	CKPE_COMMON_API void _WRITE_OPTION_STR(const std::string& section, const std::string& option,
		const std::string& value)
	{
		Common::Interface::GetSingleton()->GetSettings()->WriteString(section, option, value);
	}

	CKPE_COMMON_API void _WRITE_OPTION_USTR(const std::string& section, const std::string& option,
		const std::wstring& value)
	{
		Common::Interface::GetSingleton()->GetSettings()->WriteUnicodeString(section, option, value);
	}
}