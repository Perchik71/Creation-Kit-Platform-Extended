// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Utils.h>
#include <CKPE.ErrorHandler.h>
#include <CKPE.GameManager.h>
#include <CKPE.Module.h>
#include <CKPE.StringUtils.h>
#include <CKPE.PathUtils.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.LogWindow.h>
#include <CKPE.PluginAPI.PluginAPI.h>
#include <CKPE.PluginAPI.PluginManager.h>

namespace CKPE
{
	namespace PluginAPI
	{
		CKPE_PLUGINAPI_API Logger UserPluginLogger;

		CKPE_PLUGINAPI_API void _FATALERROR(const std::string_view& formatted_message, ...) noexcept(true)
		{
			va_list ap;
			va_start(ap, &formatted_message);
			UserPluginLogger.WriteString(Logger::tFatalError, StringUtils::FormatStringVa(formatted_message.data(), ap));
			va_end(ap);
		}

		CKPE_PLUGINAPI_API void _ERROR(const std::string_view& formatted_message, ...) noexcept(true)
		{
			va_list ap;
			va_start(ap, &formatted_message);
			UserPluginLogger.WriteString(Logger::tError, StringUtils::FormatStringVa(formatted_message.data(), ap));
			va_end(ap);
		}

		CKPE_PLUGINAPI_API void _WARNING(const std::string_view& formatted_message, ...) noexcept(true)
		{
			va_list ap;
			va_start(ap, &formatted_message);
			UserPluginLogger.WriteString(Logger::tWarning, StringUtils::FormatStringVa(formatted_message.data(), ap));
			va_end(ap);
		}

		CKPE_PLUGINAPI_API void _MESSAGE(const std::string_view& formatted_message, ...) noexcept(true)
		{
			va_list ap;
			va_start(ap, &formatted_message);
			UserPluginLogger.WriteString(Logger::tMessage, StringUtils::FormatStringVa(formatted_message.data(), ap));
			va_end(ap);
		}

		CKPE_PLUGINAPI_API void _FATALERROR(const std::wstring_view& formatted_message, ...) noexcept(true)
		{
			va_list ap;
			va_start(ap, &formatted_message);
			UserPluginLogger.WriteString(Logger::tFatalError, StringUtils::Utf16ToUtf8(StringUtils::FormatStringVa(formatted_message.data(), ap)));
			va_end(ap);
		}

		CKPE_PLUGINAPI_API void _ERROR(const std::wstring_view& formatted_message, ...) noexcept(true)
		{
			va_list ap;
			va_start(ap, &formatted_message);
			UserPluginLogger.WriteString(Logger::tError, StringUtils::Utf16ToUtf8(StringUtils::FormatStringVa(formatted_message.data(), ap)));
			va_end(ap);
		}

		CKPE_PLUGINAPI_API void _WARNING(const std::wstring_view& formatted_message, ...) noexcept(true)
		{
			va_list ap;
			va_start(ap, &formatted_message);
			UserPluginLogger.WriteString(Logger::tWarning, StringUtils::Utf16ToUtf8(StringUtils::FormatStringVa(formatted_message.data(), ap)));
			va_end(ap);
		}

		CKPE_PLUGINAPI_API void _MESSAGE(const std::wstring_view& formatted_message, ...) noexcept(true)
		{
			va_list ap;
			va_start(ap, &formatted_message);
			UserPluginLogger.WriteString(Logger::tMessage, StringUtils::Utf16ToUtf8(StringUtils::FormatStringVa(formatted_message.data(), ap)));
			va_end(ap);
		}
	}
}