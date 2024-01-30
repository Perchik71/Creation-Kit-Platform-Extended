// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		enum DebugLogMessageLevel
		{
			vmlFatalError = 0,
			vmlError,
			vmlWarning,
			vmlMessage,
			vmlDebugMessage
		};

		class DebugLog
		{
		public:
			DebugLog();
			DebugLog(const char* path);
			DebugLog(const wchar_t* path);
			DebugLog(int folderID, const char* relPath);
			DebugLog(int folderID, const wchar_t* relPath);
			~DebugLog();

			void Open(const char* path);
			void Open(const wchar_t* path);
			void OpenRelative(int folderID, const char* relPath);
			void OpenRelative(int folderID, const wchar_t* relPath);
			void Close();

			inline bool IsOpen() const { return _handleFile != nullptr; }
			inline FILE* GetHandle() const { return _handleFile; }
			inline void SetAutoFlush(bool inAutoFlush) { _autoFlush = inAutoFlush; }

			virtual void Message(const char* message, bool newLine = true);
			virtual void Message(const wchar_t* message, bool newLine = true);
			virtual void FormattedMessage(const char* fmt, ...);
			virtual void FormattedMessage(const wchar_t* fmt, ...);
			virtual void FormattedMessageVa(const char* fmt, va_list args);
			virtual void FormattedMessageVa(const wchar_t* fmt, va_list args);
			virtual void LogVa(DebugLogMessageLevel level, const char* fmt, va_list args);
			virtual void LogVa(DebugLogMessageLevel level, const wchar_t* fmt, va_list args);
			virtual void Log(DebugLogMessageLevel level, const char* fmt, ...);
			virtual void Log(DebugLogMessageLevel level, const wchar_t* fmt, ...);
		private:
			DebugLog(const DebugLog&) = default;
			DebugLog& operator=(const DebugLog&) = default;

			inline void AutoFlush() { if (_autoFlush) fflush(_handleFile); }

			void PrintSpaces(int numSpaces);
			void PrintText(const char* buf);
			void PrintText(const wchar_t* buf);
			void NewLine(void);
		private:
			FILE* _handleFile;
			bool _autoFlush;
			std::unique_ptr<wchar_t[]> _formatBufW;
			std::unique_ptr<char[]> _formatBufA;
		};

		extern DebugLog* GlobalDebugLogPtr;
	}

	inline void _FATALERROR(const char* fmt, ...)
	{
		va_list args;

		va_start(args, fmt);
		Core::GlobalDebugLogPtr->Log(Core::vmlFatalError, fmt, args);
		va_end(args);

		TerminateProcess(GetCurrentProcess(), 1);
		__assume(0);
	}

	inline void _ERROR(const char* fmt, ...)
	{
		va_list args;

		va_start(args, fmt);
		Core::GlobalDebugLogPtr->Log(Core::vmlError, fmt, args);
		va_end(args);
	}

	inline void _WARNING(const char* fmt, ...)
	{
		va_list args;

		va_start(args, fmt);
		Core::GlobalDebugLogPtr->Log(Core::vmlWarning, fmt, args);
		va_end(args);
	}

	inline void _MESSAGE(const char* fmt, ...)
	{
		va_list args;

		va_start(args, fmt);
		Core::GlobalDebugLogPtr->Log(Core::vmlMessage, fmt, args);
		va_end(args);
	}

	inline void _DMESSAGE(const char* fmt, ...)
	{
		va_list args;

		va_start(args, fmt);
		Core::GlobalDebugLogPtr->Log(Core::vmlDebugMessage, fmt, args);
		va_end(args);
	}

	inline void _FATALERROR(const wchar_t* fmt, ...)
	{
		va_list args;

		va_start(args, fmt);
		Core::GlobalDebugLogPtr->Log(Core::vmlFatalError, fmt, args);
		va_end(args);

		TerminateProcess(GetCurrentProcess(), 1);
		__assume(0);
	}

	inline void _ERROR(const wchar_t* fmt, ...)
	{
		va_list args;

		va_start(args, fmt);
		Core::GlobalDebugLogPtr->Log(Core::vmlError, fmt, args);
		va_end(args);
	}

	inline void _WARNING(const wchar_t* fmt, ...)
	{
		va_list args;

		va_start(args, fmt);
		Core::GlobalDebugLogPtr->Log(Core::vmlWarning, fmt, args);
		va_end(args);
	}

	inline void _MESSAGE(const wchar_t* fmt, ...)
	{
		va_list args;

		va_start(args, fmt);
		Core::GlobalDebugLogPtr->Log(Core::vmlMessage, fmt, args);
		va_end(args);
	}

	inline void _DMESSAGE(const wchar_t* fmt, ...)
	{
		va_list args;

		va_start(args, fmt);
		Core::GlobalDebugLogPtr->Log(Core::vmlDebugMessage, fmt, args);
		va_end(args);
	}

	/// UTF8

	inline void _FATALERROR(const char8_t* fmt, ...)
	{
		va_list args;

		va_start(args, fmt);
		Core::GlobalDebugLogPtr->Log(Core::vmlFatalError, (char*)fmt, args);
		va_end(args);

		TerminateProcess(GetCurrentProcess(), 1);
		__assume(0);
	}

	inline void _ERROR(const char8_t* fmt, ...)
	{
		va_list args;

		va_start(args, fmt);
		Core::GlobalDebugLogPtr->Log(Core::vmlError, (char*)fmt, args);
		va_end(args);
	}

	inline void _WARNING(const char8_t* fmt, ...)
	{
		va_list args;

		va_start(args, fmt);
		Core::GlobalDebugLogPtr->Log(Core::vmlWarning, (char*)fmt, args);
		va_end(args);
	}

	inline void _MESSAGE(const char8_t* fmt, ...)
	{
		va_list args;

		va_start(args, fmt);
		Core::GlobalDebugLogPtr->Log(Core::vmlMessage, (char*)fmt, args);
		va_end(args);
	}

	inline void _DMESSAGE(const char8_t* fmt, ...)
	{
		va_list args;

		va_start(args, fmt);
		Core::GlobalDebugLogPtr->Log(Core::vmlDebugMessage, (char*)fmt, args);
		va_end(args);
	}
}