// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <CKPE.Logger.h>
#include <CKPE.Common.Patch.h>
#include <CKPE.PluginAPI.Common.h>

namespace CKPE
{
	namespace PluginAPI
	{
		typedef std::uint32_t CKPEPluginHandle;
		constexpr static auto kPluginHandle_Invalid = (CKPEPluginHandle)-1;

		struct CKPE_PLUGINAPI_API CKPEPluginVersionData
		{
			enum : std::uint32_t
			{
				kVersion = 1,
			};

			enum : std::uint32_t
			{
				kGameUnknown	= 0,
				kGameSkyrimSE	= 1 << 0,
				kGameFallout4	= 1 << 1,
				kGameStarfield	= 1 << 2,
			};

			enum : std::uint32_t
			{
				kAnyGames				= 1 << 0,	// any game
				kNoLinkedVersionGame	= 1 << 1,	// is no linked to the game version
			};

			std::uint32_t	DataVersion;			// set to kVersion
			std::uint32_t	PluginVersion;			// version number of your plugin
			char			Name[256];				// null-terminated UTF16 plugin name
			char			Author[256];			// null-terminated UTF16 plugin author name (can be empty)

			std::uint8_t	Game;					// set to [kGameSkyrimSE, kGameFallout4 or kGameStarfield]
			std::uint64_t	CommLibVersionRequired;	// minimum version of the CKPE.Common required (MAKE_EXE_VERSION_EX)
			// you probably should just set this to 0 unless you know what you are doing
			std::uint64_t	CompatibleVersions[16];	// zero-terminated list MAKE_EXE_VERSION_EX() of defines your plugin is compatible with
			std::uint32_t	Flags;					// 
			std::uint8_t	Reserved[512];			// set to 0
		};

		struct CKPE_PLUGINAPI_API CKPEPluginInterface
		{
			std::uint64_t	CKPEVersion;			// CKPE version
			std::uint64_t	CKPECommonVersion;		// CKPE.Common version
			std::uint64_t	CKPEGameLibraryVersion;	// CKPE.<GAME> version
			std::uint64_t	RuntimeVersion;			// CreationKit.exe version

			void* (*QueryInterface)(std::uint32_t);

			CKPEPluginHandle(*GetPluginHandle)();
		};

		enum : std::uint32_t
		{
			kInterface_Invalid = 0,
			kInterface_DialogManager,
		};

		struct CKPEDialogManagerInterface
		{
			enum : std::uint32_t
			{
				kInterfaceVersion = 1
			};

			std::uint32_t InterfaceVersion;

			// The function returns TRUE if a dialog with such an identifier (in the uid argument) exists.
			bool (*HasDialog)(const std::uintptr_t uid);
			// The function returns TRUE if the dialog is successfully loaded from the file .json is used behind the identifier (in the uid argument).
			// The identifier must be free, otherwise the function will refuse to enter the dialog.
			bool (*AddDialog)(const char* json_file, const std::uintptr_t uid);
			// The function returns TRUE if the dialog is successfully loaded from text in the format .json is used behind the identifier (in the uid argument).
			// The identifier must be free, otherwise the function will refuse to enter the dialog.
			bool (*AddDialogByCode)(const char* json_code, const std::uintptr_t uid);
			// The function returns true if the file exists, but it does not guarantee the success of the entire operation.
			// The file must have the file extension ".pak" and be opened with a regular zip archiver.
			// In essence, it is ".zip" renamed to ".pak" (a reference to the game Quake III Arena).
			// Unlike AddDialog and AddDialogByCode, in case of an error, it will generate a RuntimeError and write the error to the CKPE log itself.
			bool (*LoadFromFilePackage)(const char* filename);
		};

		CKPE_PLUGINAPI_API extern Logger UserPluginLogger;

		CKPE_PLUGINAPI_API void _FATALERROR(const std::string_view& formatted_message, ...) noexcept(true);
		CKPE_PLUGINAPI_API void _ERROR(const std::string_view& formatted_message, ...) noexcept(true);
		CKPE_PLUGINAPI_API void _WARNING(const std::string_view& formatted_message, ...) noexcept(true);
		CKPE_PLUGINAPI_API void _MESSAGE(const std::string_view& formatted_message, ...) noexcept(true);
		CKPE_PLUGINAPI_API void _FATALERROR( const std::wstring_view& formatted_message, ...) noexcept(true);
		CKPE_PLUGINAPI_API void _ERROR(const std::wstring_view& formatted_message, ...) noexcept(true);
		CKPE_PLUGINAPI_API void _WARNING(const std::wstring_view& formatted_message, ...) noexcept(true);
		CKPE_PLUGINAPI_API void _MESSAGE(const std::wstring_view& formatted_message, ...) noexcept(true);
	
		template<class... _Types>
		inline void _FATALERROR_EX(const std::format_string<_Types...> _Fmt, _Types&&... _Args) noexcept(true)
		{
			UserPluginLogger.WriteString(Logger::tFatalError, std::vformat(_Fmt.get(), std::make_format_args(_Args...)));
		}
		template<class... _Types>
		inline void _ERROR_EX(const std::format_string<_Types...> _Fmt, _Types&&... _Args) noexcept(true)
		{
			UserPluginLogger.WriteString(Logger::tError, std::vformat(_Fmt.get(), std::make_format_args(_Args...)));
		}
		template<class... _Types>
		inline void _WARNING_EX(const std::format_string<_Types...> _Fmt, _Types&&... _Args) noexcept(true)
		{
			UserPluginLogger.WriteString(Logger::tWarning, std::vformat(_Fmt.get(), std::make_format_args(_Args...)));
		}
		template<class... _Types>
		inline void _MESSAGE_EX(const std::format_string<_Types...> _Fmt, _Types&&... _Args) noexcept(true)
		{
			UserPluginLogger.WriteString(Logger::tMessage, std::vformat(_Fmt.get(), std::make_format_args(_Args...)));
		}
		template<class... _Types>
		inline void _FATALERROR_EX(const std::wformat_string<_Types...> _Fmt, _Types&&... _Args) noexcept(true)
		{
			UserPluginLogger.WriteString(Logger::tFatalError, std::vformat(_Fmt.get(), std::make_wformat_args(_Args...)));
		}
		template<class... _Types>
		inline void _ERROR_EX(const std::wformat_string<_Types...> _Fmt, _Types&&... _Args) noexcept(true)
		{
			UserPluginLogger.WriteString(Logger::tError, std::vformat(_Fmt.get(), std::make_wformat_args(_Args...)));
		}
		template<class... _Types>
		inline void _WARNING_EX(const std::wformat_string<_Types...> _Fmt, _Types&&... _Args) noexcept(true)
		{
			UserPluginLogger.WriteString(Logger::tWarning, std::vformat(_Fmt.get(), std::make_wformat_args(_Args...)));
		}
		template<class... _Types>
		inline void _MESSAGE_EX(const std::wformat_string<_Types...> _Fmt, _Types&&... _Args) noexcept(true)
		{
			UserPluginLogger.WriteString(Logger::tMessage, std::vformat(_Fmt.get(), std::make_wformat_args(_Args...)));
		}
	}
}