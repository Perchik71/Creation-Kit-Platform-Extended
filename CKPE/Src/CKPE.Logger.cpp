// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <filesystem>
#include <array>
#include <CKPE.Logger.h>
#include <CKPE.ErrorHandler.h>
#include <CKPE.StringUtils.h>

namespace CKPE
{
	static Logger _slogger;

	constexpr static std::array TYPEMSG_PREFIX{
			"",
			"[WARNING] ",
			"[ERROR] ",
			"[FATALERROR] ",
	};

	Logger::~Logger()
	{
		Close();
	}

	bool Logger::Open(const std::string& fname) noexcept(true)
	{
		return Open(StringUtils::Utf8ToUtf16(fname));
	}

	bool Logger::Open(const std::wstring& fname) noexcept(true)
	{
		ScopeCriticalSection{ _section };

		Close();

		_handle = (void*)_wfsopen(fname.c_str(), L"wb", _SH_DENYWR);
		if (!_handle)
		{
			uint32_t id = 1;
			wchar_t name[512]{ 0 };

			do
			{
				std::filesystem::path p(fname);
				swprintf_s(name, L"%s/%s%d.log", p.parent_path().c_str(), p.stem().c_str(), id);
				id++;

				_handle = _wfsopen(name, L"wb", _SH_DENYWR);
			} while (!_handle && (id <= 10));

			if (HasOpen())
			{
				_fname = new std::wstring;
				if (_fname) *_fname = name;
			}
		}

		if (HasOpen())
		{
			if (!_fname)
			{
				_fname = new std::wstring;
				if (_fname) *_fname = fname;
			}

			char timeBuffer[80];
			struct tm* timeInfo;
			time_t rawtime;
			time(&rawtime);
			timeInfo = localtime(&rawtime);
			strftime(timeBuffer, sizeof(timeBuffer), "%A %d %b %Y %r %Z", timeInfo);
			Write("### Create time: %s\n", timeBuffer);

			return true;
		}
		return false;
	}

	void Logger::Close() noexcept(true)
	{
		ScopeCriticalSection guard{ _section };

		if (HasOpen())
		{
			fflush((FILE*)_handle);
			fclose((FILE*)_handle);
			_handle = nullptr;

			if (_fname)
			{
				delete _fname;
				_fname = nullptr;
			}
		}
	}

	const Logger* Logger::GetSingleton() noexcept(true)
	{
		return &_slogger;
	}

	void Logger::Write(const std::string& formatted_message, ...) const noexcept(true)
	{
		va_list ap;
		va_start(ap, &formatted_message);
		auto len = _vscprintf(formatted_message.c_str(), (va_list)ap);
		if (len < 1) return;

		std::string string_done;
		string_done.resize((std::size_t)len);
		if (string_done.empty()) return;
		vsprintf(string_done.data(), formatted_message.c_str(), (va_list)ap);
		va_end(ap);

		WriteString(TypeMsg::tMessage, string_done);
	}

	void Logger::Write(const std::wstring& formatted_message, ...) const noexcept(true)
	{
		va_list ap;
		va_start(ap, &formatted_message);
		auto len = _vscwprintf(formatted_message.c_str(), (va_list)ap);
		if (len < 1) return;

		std::wstring string_done;
		string_done.resize((std::size_t)len);
		if (string_done.empty()) return;
		_vswprintf(string_done.data(), formatted_message.c_str(), (va_list)ap);
		va_end(ap);

		WriteString(TypeMsg::tMessage, StringUtils::Utf16ToUtf8(string_done));
	}

	void Logger::Write(TypeMsg type_msg, const std::string& formatted_message, ...) const noexcept(true)
	{
		va_list ap;
		va_start(ap, &formatted_message);
		auto len = _vscprintf(formatted_message.c_str(), (va_list)ap);
		if (len < 1) return;

		std::string string_done;
		string_done.resize((std::size_t)len);
		if (string_done.empty()) return;
		vsprintf(string_done.data(), formatted_message.c_str(), (va_list)ap);
		va_end(ap);

		WriteString(type_msg, string_done);
	}

	void Logger::Write(TypeMsg type_msg, const std::wstring& formatted_message, ...) const noexcept(true)
	{
		va_list ap;
		va_start(ap, &formatted_message);
		auto len = _vscwprintf(formatted_message.c_str(), (va_list)ap);
		if (len < 1) return;

		std::wstring string_done;
		string_done.resize((std::size_t)len);
		if (string_done.empty()) return;
		_vswprintf(string_done.data(), formatted_message.c_str(), (va_list)ap);
		va_end(ap);

		WriteString(type_msg, StringUtils::Utf16ToUtf8(string_done));
	}

	void Logger::Flush() const noexcept(true)
	{
		ScopeCriticalSection guard{ _section };

		if (HasOpen())
			fflush((FILE*)_handle);
	}

	void Logger::NewLine() const noexcept(true)
	{
		ScopeCriticalSection guard{ _section };

		if (HasOpen())
			fputc('\n', (FILE*)_handle);
	}

	void Logger::WriteString(TypeMsg type_msg, const std::string& message) const noexcept(true)
	{
		ScopeCriticalSection guard{ _section };

		if (HasOpen())
		{
			if (type_msg != TypeMsg::tMessage)
				fputs(TYPEMSG_PREFIX[type_msg], (FILE*)_handle);
			fputs(message.c_str(), (FILE*)_handle);
		}
		
		if (HasAlwaysNewLine())
			NewLine();

		if (HasAutoFlush())
			Flush();

		if (HasOutputDebugger() && IsDebuggerPresent())
			OutputDebugStringA(message.c_str());

		if ((type_msg == TypeMsg::tFatalError) && HasIfFatalErrorTriggerErrorHandler())
		{
			if (!HasAutoFlush())
				Flush();

			ErrorHandler::Trigger(message);
		}
	}

	void Logger::WriteString(TypeMsg type_msg, const std::wstring& message) const noexcept(true)
	{
		WriteString(type_msg, StringUtils::Utf16ToUtf8(message));
	}

	void _FATALERROR(const std::string_view& formatted_message, ...)
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

		_slogger.WriteString(Logger::tFatalError, string_done);
	}

	void _ERROR(const std::string_view& formatted_message, ...)
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

		_slogger.WriteString(Logger::tError, string_done);
	}

	void _WARNING(const std::string_view& formatted_message, ...)
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

		_slogger.WriteString(Logger::tWarning, string_done);
	}

	void _MESSAGE(const std::string_view& formatted_message, ...)
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

		_slogger.WriteString(Logger::tMessage, string_done);
	}

	void _FATALERROR(const std::wstring_view& formatted_message, ...)
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

		_slogger.WriteString(Logger::tFatalError, StringUtils::Utf16ToUtf8(string_done));
	}

	void _ERROR(const std::wstring_view& formatted_message, ...)
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

		_slogger.WriteString(Logger::tError, StringUtils::Utf16ToUtf8(string_done));
	}

	void _WARNING(const std::wstring_view& formatted_message, ...)
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

		_slogger.WriteString(Logger::tWarning, StringUtils::Utf16ToUtf8(string_done));
	}

	void _MESSAGE(const std::wstring_view& formatted_message, ...)
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

		_slogger.WriteString(Logger::tMessage, StringUtils::Utf16ToUtf8(string_done));
	}
}