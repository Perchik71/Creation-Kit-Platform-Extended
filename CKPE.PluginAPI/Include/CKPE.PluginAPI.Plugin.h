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

		enum : CKPEPluginHandle
		{
			kPluginHandle_Invalid = (CKPEPluginHandle)-1
		};

		struct CKPE_PLUGINAPI_API CKPEPluginVersionData
		{
			enum : std::uint32_t
			{
				kVersion = 1,
			};

			enum : std::uint32_t
			{
				kGameUnknown = 0,
				kGameSkyrimSE = 1 << 0,
				kGameFallout4 = 1 << 1,
				kGameStarfield = 1 << 2,
			};

			enum : std::uint32_t
			{
				kAnyGames = 1 << 0,					// any game
				kNoLinkedVersionGame = 1 << 1,		// is no linked to the game version
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

			void* (*QueryInterface)(std::uint32_t id);

			CKPEPluginHandle (*GetPluginHandle)();
		};

		class CKPE_PLUGINAPI_API Plugin : public Common::Patch
		{
			Logger* _log{ nullptr };
			std::wstring* _dllname{ nullptr };
			std::uintptr_t _base{ 0 };
			CKPEPluginVersionData _ver_data{};
			std::vector<std::uintptr_t>* _funcs{ nullptr };

			Plugin(const Plugin&) = delete;
			Plugin& operator=(const Plugin&) = delete;

			[[nodiscard]] static std::uintptr_t SafeLoadLibraryW(const wchar_t* fname);
			[[nodiscard]] bool SafeActive(const CKPEPluginInterface* intf) const;
			static void Sanitize(CKPEPluginVersionData* version) noexcept(true);
			[[nodiscard]] static const wchar_t* CheckPluginCompatibility(const CKPEPluginVersionData* version) noexcept(true);
		protected:
			virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
			virtual bool DoQuery() const noexcept(true);
		public:
			enum : std::int32_t
			{
				NoError = 0,
				ErrorInvalidArgs = -1,
				ErrorNoInit = -2,
				ErrorLoadLib = -3,
				ErrorInvalidLib = -4,
				ErrorNoCompatibility = -5,
			};

			Plugin() noexcept(true);
			virtual ~Plugin() noexcept(true);

			[[nodiscard]] virtual bool HasOption() const noexcept(true);
			[[nodiscard]] virtual const char* GetOptionName() const noexcept(true);
			[[nodiscard]] virtual bool HasDependencies() const noexcept(true);
			[[nodiscard]] virtual std::vector<std::string> GetDependencies() const noexcept(true);

			[[nodiscard]] constexpr virtual std::string GetAuthor() const noexcept(true) { return _ver_data.Author; }
			[[nodiscard]] constexpr virtual std::uint32_t GetVersion() const noexcept(true) { return _ver_data.PluginVersion; }
			[[nodiscard]] constexpr virtual const wchar_t* GetFileNameDLL() const noexcept(true) 
			{ return _dllname ? _dllname->c_str() : L""; }

			[[nodiscard]] constexpr virtual bool IsLoaded() const noexcept(true) { return _base != 0; }

			[[nodiscard]] virtual std::int32_t CanLoad(const char* fname) noexcept(true);
			[[nodiscard]] virtual std::int32_t CanLoad(const wchar_t* fname) noexcept(true);
			[[nodiscard]] virtual std::int32_t CanLoad(const std::string& fname) noexcept(true);
			[[nodiscard]] virtual std::int32_t CanLoad(const std::wstring& fname) noexcept(true);

			[[nodiscard]] virtual bool Load(const char* fname, bool needCheck = true) noexcept(true);
			[[nodiscard]] virtual bool Load(const wchar_t* fname, bool needCheck = true) noexcept(true);
			[[nodiscard]] virtual bool Load(const std::string& fname, bool needCheck = true) noexcept(true);
			[[nodiscard]] virtual bool Load(const std::wstring& fname, bool needCheck = true) noexcept(true);

			virtual bool Unload() noexcept(true);

			virtual void _FATALERROR(const std::string_view& formatted_message, ...) noexcept(true);
			virtual void _ERROR(const std::string_view& formatted_message, ...) noexcept(true);
			virtual void _WARNING(const std::string_view& formatted_message, ...) noexcept(true);
			virtual void _MESSAGE(const std::string_view& formatted_message, ...) noexcept(true);
			virtual void _FATALERROR(const std::wstring_view& formatted_message, ...) noexcept(true);
			virtual void _ERROR(const std::wstring_view& formatted_message, ...) noexcept(true);
			virtual void _WARNING(const std::wstring_view& formatted_message, ...) noexcept(true);
			virtual void _MESSAGE(const std::wstring_view& formatted_message, ...) noexcept(true);

			template<class... _Types>
			inline void _FATALERROR_EX(const std::format_string<_Types...> _Fmt, _Types&&... _Args) noexcept(true)
			{
				_log->WriteString(Logger::tFatalError, std::vformat(_Fmt.get(), std::make_format_args(_Args...)));
			}
			template<class... _Types>
			inline void _ERROR_EX(const std::format_string<_Types...> _Fmt, _Types&&... _Args) noexcept(true)
			{
				_log->WriteString(Logger::tError, std::vformat(_Fmt.get(), std::make_format_args(_Args...)));
			}
			template<class... _Types>
			inline void _WARNING_EX(const std::format_string<_Types...> _Fmt, _Types&&... _Args) noexcept(true)
			{
				_log->WriteString(Logger::tWarning, std::vformat(_Fmt.get(), std::make_format_args(_Args...)));
			}
			template<class... _Types>
			inline void _MESSAGE_EX(const std::format_string<_Types...> _Fmt, _Types&&... _Args) noexcept(true)
			{
				_log->WriteString(Logger::tMessage, std::vformat(_Fmt.get(), std::make_format_args(_Args...)));
			}
			template<class... _Types>
			inline void _FATALERROR_EX(const std::wformat_string<_Types...> _Fmt, _Types&&... _Args) noexcept(true)
			{
				_log->WriteString(Logger::tFatalError, std::vformat(_Fmt.get(), std::make_wformat_args(_Args...)));
			}
			template<class... _Types>
			inline void _ERROR_EX(const std::wformat_string<_Types...> _Fmt, _Types&&... _Args) noexcept(true)
			{
				_log->WriteString(Logger::tError, std::vformat(_Fmt.get(), std::make_wformat_args(_Args...)));
			}
			template<class... _Types>
			inline void _WARNING_EX(const std::wformat_string<_Types...> _Fmt, _Types&&... _Args) noexcept(true)
			{
				_log->WriteString(Logger::tWarning, std::vformat(_Fmt.get(), std::make_wformat_args(_Args...)));
			}
			template<class... _Types>
			inline void _MESSAGE_EX(const std::wformat_string<_Types...> _Fmt, _Types&&... _Args) noexcept(true)
			{
				_log->WriteString(Logger::tMessage, std::vformat(_Fmt.get(), std::make_wformat_args(_Args...)));
			}

			virtual void _CONSOLE(const std::string_view& formatted_message, ...) noexcept(true);
			virtual void _CONSOLEVA(const std::string_view& formatted_message, va_list va) noexcept(true);
			virtual void _CONSOLE(const std::wstring_view& formatted_message, ...) noexcept(true);
			virtual void _CONSOLEVA(const std::wstring_view& formatted_message, va_list va) noexcept(true);
		};
	}
}