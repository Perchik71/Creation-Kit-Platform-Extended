// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "DebugLog.h"

constexpr static size_t FORMATBUF_SIZE = 8096;

static std::vector<std::string_view> MESSAGETYPE_STR = 
{
	"FATALERROR",
	"ERROR",
	"WARNING",
	"MESSAGE",
	"DEBUGMESSAGE"
};

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		DebugLog* GlobalDebugLogPtr = nullptr;

		DebugLog::DebugLog() : _handleFile(nullptr), _autoFlush(true)
		{
			_formatBufW = std::make_unique<wchar_t[]>(FORMATBUF_SIZE + 1);
			_formatBufA = std::make_unique<char[]>(FORMATBUF_SIZE + 1);
		}

		DebugLog::DebugLog(const char* path) : DebugLog()
		{
			Open(path);
		}

		DebugLog::DebugLog(const wchar_t* path) : DebugLog()
		{
			Open(path);
		}

		DebugLog::DebugLog(int folderID, const char* relPath) : DebugLog()
		{
			OpenRelative(folderID, relPath);
		}

		DebugLog::DebugLog(int folderID, const wchar_t* relPath) : DebugLog()
		{
			OpenRelative(folderID, relPath);
		}

		DebugLog::~DebugLog()
		{
			Close();
		}

		void DebugLog::Open(const char* path)
		{
			Open(Utils::Ansi2Wide(path).c_str());
		}

		void DebugLog::Open(const wchar_t* path)
		{
			_handleFile = _wfsopen(path, L"wt", _SH_DENYWR);
			if (!_handleFile)
			{
				uint32_t Id = 0;
				wchar_t	FileName[1024];

				do
				{
					Id++;
					swprintf_s(FileName, L"%s%d", path, Id);
					_handleFile = _wfsopen(FileName, L"wt", _SH_DENYWR);
				} while (!_handleFile && (Id < 6));
			}
		}

		void DebugLog::OpenRelative(int folderID, const char* relPath)
		{
			OpenRelative(folderID, Utils::Ansi2Wide(relPath).c_str());
		}

		void DebugLog::OpenRelative(int folderID, const wchar_t* relPath)
		{
			wchar_t path[MAX_PATH];
			HRESULT err = SHGetFolderPathW(NULL, folderID | CSIDL_FLAG_CREATE,
				NULL, SHGFP_TYPE_CURRENT, path);
			if (SUCCEEDED(err))
			{
				wcscat_s(path, relPath);
				PathRemoveFileSpecW(path);
				auto attr = GetFileAttributesW(path);
				if ((attr == INVALID_FILE_ATTRIBUTES) ||
					((attr & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY))
					SHCreateDirectoryExW(0, path, nullptr);
				Open(path);
			}
		}

		void DebugLog::Close()
		{
			if (_handleFile)
			{
				fclose(_handleFile);
				_handleFile = nullptr;
			}
		}

		void DebugLog::Message(const char* message, bool newLine)
		{
			PrintText(message);

			if (newLine)
				NewLine();
		}

		void DebugLog::Message(const wchar_t* message, bool newLine)
		{
			PrintText(message);

			if (newLine)
				NewLine();
		}

		void DebugLog::FormattedMessage(const char* fmt, ...)
		{
			va_list	argList;
			va_start(argList, fmt);
			vsprintf_s(_formatBufA.get(), FORMATBUF_SIZE, fmt, argList);
			Message(_formatBufA.get());
			va_end(argList);
		}

		void DebugLog::FormattedMessage(const wchar_t* fmt, ...)
		{
			va_list	argList;
			va_start(argList, fmt);
			vswprintf_s(_formatBufW.get(), FORMATBUF_SIZE, fmt, argList);
			Message(_formatBufW.get());
			va_end(argList);
		}

		void DebugLog::FormattedMessage(const char* fmt, va_list args)
		{
			vsprintf_s(_formatBufA.get(), FORMATBUF_SIZE, fmt, args);
			Message(_formatBufA.get());
		}

		void DebugLog::FormattedMessage(const wchar_t* fmt, va_list args)
		{
			vswprintf_s(_formatBufW.get(), FORMATBUF_SIZE, fmt, args);
			Message(_formatBufW.get());
		}

		void DebugLog::Log(DebugLogMessageLevel level, const char* fmt, va_list args)
		{
			char szBuf[64] = { 0 };
			if (level != vmlMessage)
			{
				sprintf_s(szBuf, "[%s] ", MESSAGETYPE_STR[(int)level].data());
				Message(szBuf, false);
			}
			FormattedMessage(fmt, args);
		}

		void DebugLog::Log(DebugLogMessageLevel level, const wchar_t* fmt, va_list args)
		{
			char szBuf[64] = { 0 };
			if (level != vmlMessage)
			{
				sprintf_s(szBuf, "[%s] ", MESSAGETYPE_STR[(int)level].data());
				Message(szBuf, false);
			}
			FormattedMessage(fmt, args);
		}

		void DebugLog::PrintSpaces(int numSpaces)
		{
			if (!_handleFile) return;
			while (numSpaces > 0)
			{
				numSpaces--;
				fputc(' ', _handleFile);
			}
		}

		void DebugLog::PrintText(const char* buf)
		{
			if (!_handleFile || !buf) return;

			fputs(buf, _handleFile);
			AutoFlush();
		}

		void DebugLog::PrintText(const wchar_t* buf)
		{
			if (!_handleFile || !buf) return;

			fputs(Utils::Wide2Utf8(buf).c_str(), _handleFile);
			AutoFlush();
		}

		void DebugLog::NewLine(void)
		{
			if (!_handleFile) return;

			fputc('\n', _handleFile);
			AutoFlush();
		}
	}
}