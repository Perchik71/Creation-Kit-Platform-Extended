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
#include <CKPE.PluginAPI.Plugin.h>

namespace CKPE
{
	namespace PluginAPI
	{
		std::uintptr_t Plugin::SafeLoadLibraryW(const wchar_t* fname)
		{
			__try
			{
				return (std::uintptr_t)LoadLibraryW(fname);
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				// paranoia
				return 0;
			}
		}

		bool Plugin::SafeActive(const CKPEPluginInterface* interface) const
		{
			__try
			{
				return fast_call<bool>((*_funcs)[3], interface);
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				// paranoia
				return false;
			}
		}

		void Plugin::Sanitize(CKPEPluginVersionData* version) noexcept(true)
		{
			version->Name[_countof(version->Name) - 1] = 0;
			version->Author[_countof(version->Author) - 1] = 0;
		}

		const wchar_t* Plugin::CheckPluginCompatibility(const CKPEPluginVersionData* version) noexcept(true)
		{
			__try
			{
				if (!version)
					return L"disabled, data version is nullptr";

				if (version->DataVersion != CKPEPluginVersionData::kVersion)
					return L"disabled, bad version data";

				if (!version->Name[0])
					return L"disabled, no name specified";

				if (version->CommLibVersionRequired)
				{
					auto ver = Common::Interface::GetSingleton()->GetVersionDLL();

					auto ver_major = GET_EXE_VERSION_EX_MAJOR(version->CommLibVersionRequired);
					auto ver_minor = GET_EXE_VERSION_EX_MINOR(version->CommLibVersionRequired);
					auto ver_build = GET_EXE_VERSION_EX_BUILD(version->CommLibVersionRequired);

					auto ver_commlib_major = GET_EXE_VERSION_EX_MAJOR(ver);
					auto ver_commlib_minor = GET_EXE_VERSION_EX_MINOR(ver);
					auto ver_commlib_build = GET_EXE_VERSION_EX_BUILD(ver);

					if (ver_commlib_major < ver_major)
						return L"disabled, commlib needs a newer one";
					else if ((ver_major == ver_commlib_major) &&
						((ver_minor < ver_commlib_minor) || (ver_build < ver_commlib_build)))
						return L"disabled, commlib needs a newer one";
				}

				bool hasAnyGames = version->Flags & CKPEPluginVersionData::kAnyGames;
				bool hasVersionGameIndependent = version->Flags & CKPEPluginVersionData::kNoLinkedVersionGame;

				if (!hasAnyGames)
				{
					auto type = GameManager::GetSingleton()->GetType();
					switch (type)
					{
					case GameManager::Game::CK_SKYRIMSE:
						if (!(version->Game & CKPEPluginVersionData::kGameSkyrimSE))
							return L"disabled, plug-in isn't for this game";
						break;
					case GameManager::Game::CK_FALLOUT4:
						if (!(version->Game & CKPEPluginVersionData::kGameFallout4))
							return L"disabled, plug-in isn't for this game";
						break;
					case GameManager::Game::CK_STARFIELD:
						if (!(version->Game & CKPEPluginVersionData::kGameStarfield))
							return L"disabled, plug-in isn't for this game";
						break;
					default:
						return L"disabled, plug-in for unknown game";
					}
				}

				if (!hasVersionGameIndependent)
				{
					bool found = false;
					auto editor_ver = Common::Interface::GetSingleton()->GetEditorVersion();

					for (std::uint32_t i = 0; i < _countof(version->CompatibleVersions); i++)
					{
						std::uint32_t compatibleVersion = version->CompatibleVersions[i];
						if (compatibleVersion == editor_ver)
						{
							found = true;
							break;
						}
						else if (!compatibleVersion)
							break;
					}

					if (!found)
						return L"disabled, incompatible with current version of the Creation Kit";
				}

				return nullptr;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				// paranoia
				return L"disabled, fatal error occurred while checking plugin compatibility";
			}
		}

		bool Plugin::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
		{
			auto log_fname = PathUtils::ChangeFileExt(*_dllname, L".log");	
			if (!_log->Open(log_fname))
			{
				CKPE::_ERROR(L"Couldn't create \"%s\" file", log_fname.c_str());
				return false;
			}

			char timeBuffer[80];
			struct tm* timeInfo;
			time_t rawtime;
			time(&rawtime);
			timeInfo = localtime(&rawtime);
			strftime(timeBuffer, sizeof(timeBuffer), "%A %d %b %Y %r %Z", timeInfo);
			_log->Write("Now: %s", timeBuffer);

			return SafeActive((const CKPEPluginInterface*)db);
		}

		bool Plugin::DoQuery() const noexcept(true)
		{
			return true; // no need, CanLoad() did this job
		}

		Plugin::Plugin() noexcept(true) :
			_dllname(new std::wstring), _funcs(new std::vector<std::uintptr_t>), _log(new Logger())
		{}

		Plugin::~Plugin() noexcept(true)
		{
			Unload();

			if (_log)
			{
				delete _log;
				_log = nullptr;
			}

			if (_funcs)
			{
				delete _funcs;
				_funcs = nullptr;
			}

			if (_dllname)
			{
				delete _dllname;
				_dllname = nullptr;
			}
		}

		bool Plugin::HasOption() const noexcept(true)
		{
			return false;
		}

		const char* Plugin::GetOptionName() const noexcept(true)
		{
			return nullptr;
		}

		bool Plugin::HasDependencies() const noexcept(true)
		{
			return fast_call<bool>((*_funcs)[0]);
		}

		std::vector<std::string> Plugin::GetDependencies() const noexcept(true)
		{
			std::vector<std::string> result;
			auto count = fast_call<std::uint32_t>((*_funcs)[1]);
			for (std::uint32_t i = 0; i < count; i++)
				result.push_back(fast_call<const char*>((*_funcs)[2], i));
			return result;
		}

		std::int32_t Plugin::CanLoad(const char* fname) noexcept(true)
		{
			return CanLoad(StringUtils::WinCPToUtf16(fname));
		}

		std::int32_t Plugin::CanLoad(const wchar_t* fname) noexcept(true)
		{
			if (!_dllname || !_funcs)
				return ErrorNoInit;

			if (!fname || !fname[0])
			{
				CKPE::_WARNING("Load CKPE plug-in, but fname is empty");
				return ErrorInvalidArgs;
			}

			auto sfname = PathUtils::ExtractFileName(fname);
			if (sfname.empty())
			{
				CKPE::_WARNING("Load CKPE plug-in, but fname isn't contain filename");
				return ErrorInvalidArgs;
			}

			if (_wcsicmp(PathUtils::ExtractFileExt(sfname).c_str(), L".dll"))
			{
				CKPE::_ERROR(L"Load CKPE plug-in, but \"%s\" isn't .dll", sfname.c_str());
				return ErrorInvalidArgs;
			}

			auto dllname = PathUtils::GetCKPEPluginPath() + sfname;

			auto hmem = LoadLibraryExW(dllname.c_str(), nullptr, LOAD_LIBRARY_AS_IMAGE_RESOURCE);
			if (!hmem)
			{
				CKPE::_ERROR(L"Couldn't load CKPE plug-in \"%s\"", sfname.c_str(),
					StringUtils::WinCPToUtf16(ErrorHandler::GetSystemMessage(GetLastError())).c_str());
				return ErrorLoadLib;
			}

			Module module_plugin((void*)hmem);
			ScopeLoadedModule guard(module_plugin);

			if (!module_plugin.Is64())
			{
				CKPE::_ERROR(L"32-bit CKPE plug-in \"%s\" can never work", sfname.c_str());
				return ErrorInvalidLib;
			}

			auto* version = (CKPEPluginVersionData*)module_plugin.Resources.GetProcAddress("CKPEPlugin_Version");
			if (!version)
			{
				CKPE::_ERROR(L"CKPE plug-in \"%s\" no version data", sfname.c_str());
				return ErrorInvalidLib;
			}

			Sanitize(version);

			auto result_check = CheckPluginCompatibility(version);
			if (result_check)
			{
				CKPE::_ERROR(L"CKPE plug-in \"%s\" (%08X %s %08X) %s",
					sfname.c_str(), version->DataVersion, StringUtils::WinCPToUtf16(version->Name).c_str(),
					version->PluginVersion, result_check);
				return ErrorNoCompatibility;
			}

			if (!module_plugin.Resources.GetProcAddress("CKPEPlugin_HasDependencies") ||
				!module_plugin.Resources.GetProcAddress("CKPEPlugin_GetDependCount") ||
				!module_plugin.Resources.GetProcAddress("CKPEPlugin_GetDependAt") ||
				!module_plugin.Resources.GetProcAddress("CKPEPlugin_Load"))
			{
				CKPE::_ERROR(L"CKPE plug-in \"%s\" does not have suitable functions", sfname.c_str());
				return ErrorInvalidLib;
			}

			return NoError;
		}

		std::int32_t Plugin::CanLoad(const std::string& fname) noexcept(true)
		{
			return CanLoad(StringUtils::WinCPToUtf16(fname));
		}

		std::int32_t Plugin::CanLoad(const std::wstring& fname) noexcept(true)
		{
			return CanLoad(fname.c_str());
		}

		bool Plugin::Load(const char* fname, bool needCheck) noexcept(true)
		{
			return Load(StringUtils::WinCPToUtf16(fname), needCheck);
		}

		bool Plugin::Load(const wchar_t* fname, bool needCheck) noexcept(true)
		{
			if (needCheck && (CanLoad(fname) != NoError))
				return false;

			auto sfname = PathUtils::ExtractFileName(fname);
			*_dllname = PathUtils::GetCKPEPluginPath() + sfname;

			auto hmem = LoadLibraryExW(_dllname->c_str(), nullptr, LOAD_LIBRARY_AS_IMAGE_RESOURCE);
			if (!hmem)
			{
				CKPE::_ERROR(L"Couldn't load CKPE plug-in \"%s\"", sfname.c_str(),
					StringUtils::WinCPToUtf16(ErrorHandler::GetSystemMessage(GetLastError())).c_str());
				_dllname->clear();

				return false;
			}

			{
				Module module_plugin((void*)hmem);
				ScopeLoadedModule guard(module_plugin);
				auto* version = (CKPEPluginVersionData*)module_plugin.Resources.GetProcAddress("CKPEPlugin_Version");
				if (!version)
				{
					CKPE::_ERROR(L"CKPE plug-in \"%s\" no version data", sfname.c_str());
					_dllname->clear();

					return false;
				}

				Sanitize(version);
				memcpy(&_ver_data, version, sizeof(CKPEPluginVersionData));
			}

			_base = SafeLoadLibraryW(_dllname->c_str());
			if (!_base)
			{
				CKPE::_ERROR(L"Couldn't load CKPE plug-in \"%s\"", sfname.c_str(),
					StringUtils::WinCPToUtf16(ErrorHandler::GetSystemMessage(GetLastError())).c_str());
				_dllname->clear();

				return false;
			}

			SetName(_ver_data.Name);

			_funcs->push_back((std::uintptr_t)GetProcAddress((HMODULE)_base, "CKPEPlugin_HasDependencies"));
			_funcs->push_back((std::uintptr_t)GetProcAddress((HMODULE)_base, "CKPEPlugin_GetDependCount"));
			_funcs->push_back((std::uintptr_t)GetProcAddress((HMODULE)_base, "CKPEPlugin_GetDependAt"));
			_funcs->push_back((std::uintptr_t)GetProcAddress((HMODULE)_base, "CKPEPlugin_Load"));

			CKPE::_MESSAGE(L"CKPE plug-in \"%s\" (%08X %s %08X) loaded correctly",
				sfname.c_str(), _ver_data.DataVersion, StringUtils::WinCPToUtf16(_ver_data.Name).c_str(),
				_ver_data.PluginVersion);

			return true;
		}

		bool Plugin::Load(const std::string& fname, bool needCheck) noexcept(true)
		{
			return Load(StringUtils::WinCPToUtf16(fname), needCheck);
		}

		bool Plugin::Load(const std::wstring& fname, bool needCheck) noexcept(true)
		{
			return Load(fname.c_str(), needCheck);
		}

		bool Plugin::Unload() noexcept(true)
		{
			if (IsActive() || !IsLoaded())
				return false;

			auto result = FreeLibrary((HMODULE)_base);
			if (result)
			{
				_base = 0;
				_dllname->clear();
			}

			return result;
		}

		void Plugin::_FATALERROR(const std::string_view& formatted_message, ...) noexcept(true)
		{
			va_list ap;
			va_start(ap, &formatted_message);
			_log->WriteString(Logger::tFatalError, StringUtils::FormatStringVa(formatted_message.data(), ap));
			va_end(ap);
		}

		void Plugin::_ERROR(const std::string_view& formatted_message, ...) noexcept(true)
		{
			va_list ap;
			va_start(ap, &formatted_message);
			_log->WriteString(Logger::tError, StringUtils::FormatStringVa(formatted_message.data(), ap));
			va_end(ap);
		}

		void Plugin::_WARNING(const std::string_view& formatted_message, ...) noexcept(true)
		{
			va_list ap;
			va_start(ap, &formatted_message);
			_log->WriteString(Logger::tWarning, StringUtils::FormatStringVa(formatted_message.data(), ap));
			va_end(ap);
		}

		void Plugin::_MESSAGE(const std::string_view& formatted_message, ...) noexcept(true)
		{
			va_list ap;
			va_start(ap, &formatted_message);
			_log->WriteString(Logger::tMessage, StringUtils::FormatStringVa(formatted_message.data(), ap));
			va_end(ap);
		}

		void Plugin::_FATALERROR(const std::wstring_view& formatted_message, ...) noexcept(true)
		{
			va_list ap;
			va_start(ap, &formatted_message);
			_log->WriteString(Logger::tFatalError, StringUtils::Utf16ToUtf8(StringUtils::FormatStringVa(formatted_message.data(), ap)));
			va_end(ap);
		}

		void Plugin::_ERROR(const std::wstring_view& formatted_message, ...) noexcept(true)
		{
			va_list ap;
			va_start(ap, &formatted_message);
			_log->WriteString(Logger::tError, StringUtils::Utf16ToUtf8(StringUtils::FormatStringVa(formatted_message.data(), ap)));
			va_end(ap);
		}

		void Plugin::_WARNING(const std::wstring_view& formatted_message, ...) noexcept(true)
		{
			va_list ap;
			va_start(ap, &formatted_message);
			_log->WriteString(Logger::tWarning, StringUtils::Utf16ToUtf8(StringUtils::FormatStringVa(formatted_message.data(), ap)));
			va_end(ap);
		}

		void Plugin::_MESSAGE(const std::wstring_view& formatted_message, ...) noexcept(true)
		{
			va_list ap;
			va_start(ap, &formatted_message);
			_log->WriteString(Logger::tMessage, StringUtils::Utf16ToUtf8(StringUtils::FormatStringVa(formatted_message.data(), ap)));
			va_end(ap);
		}

		void Plugin::_CONSOLE(const std::string_view& formatted_message, ...) noexcept(true)
		{
			va_list va;
			va_start(va, &formatted_message);
			_CONSOLEVA(formatted_message, va);
			va_end(va);
		}

		void Plugin::_CONSOLEVA(const std::string_view& formatted_message, va_list va) noexcept(true)
		{
			auto s = Common::LogWindow::GetSingleton();
			if (s) s->InputLogVa(formatted_message, va);
		}

		void Plugin::_CONSOLE(const std::wstring_view& formatted_message, ...) noexcept(true)
		{
			va_list va;
			va_start(va, &formatted_message);
			_CONSOLEVA(formatted_message, va);
			va_end(va);
		}

		void Plugin::_CONSOLEVA(const std::wstring_view& formatted_message, va_list va) noexcept(true)
		{
			auto s = Common::LogWindow::GetSingleton();
			if (s) s->InputLogVa(formatted_message, va);
		}
	}
}