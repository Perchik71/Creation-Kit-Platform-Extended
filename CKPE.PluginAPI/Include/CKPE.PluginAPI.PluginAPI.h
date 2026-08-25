﻿// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <CKPE.Logger.h>
#include <CKPE.Version.h>
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
			kInterface_AddressLibrary,
		};

		struct CKPEAddressLibraryInterface
		{
			enum : std::uint32_t
			{
				kInterfaceVersion = 2
			};

			std::uint32_t InterfaceVersion;

			// Returns true if an AddressLibrary database is loaded for the currently running
			// CreationKit.exe. If this returns false, Resolve() will always return 0 - your
			// plugin should fall back to hardcoded/legacy addressing (or refuse to patch) instead.
			bool (*IsLoaded)();
			// Resolves a stable AddressLibrary id (+ optional byte offset) to an absolute address
			// in the currently running CreationKit.exe. Returns 0 if the id is unknown for this
			// build, or if no AddressLibrary database is loaded at all.
			std::uintptr_t (*Resolve)(const std::uint64_t id, const std::ptrdiff_t offset);

			// Write helpers below mirror CKPE::SafeWrite / CKPE::Detours - each handles its own
			// VirtualProtect internally, so you don't need to. Pass addresses you got from
			// Resolve() (or your own).

			// Overwrites `size` bytes at `address` with `data`.
			void (*Write)(std::uintptr_t address, const std::uint8_t* data, std::size_t size);
			// Fills `size` bytes at `address` with the single repeated byte `value`.
			void (*WriteSet)(std::uintptr_t address, std::uint8_t value, std::size_t size);
			// Overwrites `size` bytes at `address` with 0x90 (NOP).
			void (*WriteNop)(std::uintptr_t address, std::size_t size);
			// Redirects a function's entry point (target) to jump straight to destination.
			// Returns a callable pointer to the original implementation (either the address the
			// site already pointed to, or a generated trampoline), or 0 on failure.
			std::uintptr_t (*DetourJump)(std::uintptr_t target, std::uintptr_t destination);
			// Redirects a single call SITE (target) to call destination instead. Returns a
			// callable pointer to what the site originally called, or 0 on failure.
			std::uintptr_t (*DetourCall)(std::uintptr_t target, std::uintptr_t destination);
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