// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Version/resource_version2.h"
#include "Engine.h"
#include "Plugin.h"
#include "DialogManager.h"
#include "Editor API/BSString.h"
#include "../MyFirstPlugin/CKPE/PluginAPI.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		struct DataTAG
		{
			uint32_t Version;
			Core::MemoryManager* MemoryManager;
			Core::Relocator* Relocator;
			Core::ConsoleWindow* Console;
			Core::DebugLog* Log;
			Core::Engine* Engine;
			Core::DialogManager* DialogManager;
			uint32_t MenuYourStartId;
			uint32_t MenuYourEndId;
			HMENU* SubMenu;
			char szSubMenuName[33];
		};

		extern uint32_t GlobalPluginMenuStartId;
		extern uint32_t GlobalPluginMenuEndId;

		Plugin::Plugin(Engine* lpEngine, const char* lpcstrPluginDllName) :
			Module(lpEngine), _PluginDllName(lpcstrPluginDllName), _Handle(nullptr), IsInit(true),
			Menu(nullptr)
		{
			_FuncMap.insert(std::make_pair<std::string_view, uintptr_t>("CKPEPlugin_HasCanRuntimeDisabled", 0));
			_FuncMap.insert(std::make_pair<std::string_view, uintptr_t>("CKPEPlugin_GetName", 0));
			_FuncMap.insert(std::make_pair<std::string_view, uintptr_t>("CKPEPlugin_HasDependencies", 0));
			_FuncMap.insert(std::make_pair<std::string_view, uintptr_t>("CKPEPlugin_GetDependencies", 0));
			_FuncMap.insert(std::make_pair<std::string_view, uintptr_t>("CKPEPlugin_Query", 0));
			_FuncMap.insert(std::make_pair<std::string_view, uintptr_t>("CKPEPlugin_Init", 0));
			_FuncMap.insert(std::make_pair<std::string_view, uintptr_t>("CKPEPlugin_Shutdown", 0));
			_FuncMap.insert(std::make_pair<std::string_view, uintptr_t>("CKPEPlugin_GetVersion", 0));

			_Handle = LoadLibrary(_PluginDllName.c_str());
			if (!_Handle)
			{
				_ERROR("Library \"%s\" not found", _PluginDllName.c_str());
				IsInit = false;
			}
			else
			{
				for (auto it = _FuncMap.begin(); it != _FuncMap.end(); it++)
				{
					auto Addr = (uintptr_t)GetProcAddress(_Handle, it->first.data());
					if (!Addr)
					{
						_ERROR("Library \"%s\" does not have a function \"%s\"",
							_PluginDllName.c_str(), it->first.data());
						IsInit = false;
					}

					it->second = Addr;
				}
			}

			if (IsInit)
			{
				_Log = new Core::DebugLog(EditorAPI::BSString::Utils::ChangeFileExt(lpcstrPluginDllName, ".log").c_str());
			
				ULONG _osMajorVersion = 0;
				ULONG _osMinorVersion = 0;
				ULONG _osBuildNubmer = 0;

				LONG(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEXW) = nullptr;
				OSVERSIONINFOEXW osInfo = { 0 };
				*(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");
				if (RtlGetVersion)
				{
					osInfo.dwOSVersionInfoSize = sizeof(osInfo);
					RtlGetVersion(&osInfo);

					_osMajorVersion = osInfo.dwMajorVersion;
					_osMinorVersion = osInfo.dwMinorVersion;
					_osBuildNubmer = osInfo.dwBuildNumber;
				}

				_Log->FormattedMessage("Creation Kit Platform Extended Runtime: Initialize (Version: %s, OS: %u.%u Build %u)",
					VER_FILE_VERSION_STR, _osMajorVersion, _osMinorVersion, _osBuildNubmer);

				char timeBuffer[80];
				struct tm* timeInfo;
				time_t rawtime;
				time(&rawtime);
				timeInfo = localtime(&rawtime);
				strftime(timeBuffer, sizeof(timeBuffer), "%A %d %b %Y %r %Z", timeInfo);

				_Log->FormattedMessage("Current time: %s", timeBuffer);
			}
		}

		Plugin::~Plugin()
		{
			FreeLibrary(_Handle);
		}

		bool Plugin::HasCanRuntimeDisabled() const
		{
			auto It = _FuncMap.find("CKPEPlugin_HasCanRuntimeDisabled");
			if ((It == _FuncMap.end()) || !It->second) return false;
			return fastCall<bool>(It->second);
		}

		const char* Plugin::GetName() const
		{
			auto It = _FuncMap.find("CKPEPlugin_GetName");
			if ((It == _FuncMap.end()) || !It->second) return nullptr;
			if (!fastCall<bool>(It->second, szTemp, ARRAYSIZE(szTemp)))
				return nullptr;
			return szTemp;
		}

		bool Plugin::HasDependencies() const
		{
			auto It = _FuncMap.find("CKPEPlugin_HasDependencies");
			if ((It == _FuncMap.end()) || !It->second) return false;
			return fastCall<bool>(It->second);
		}

		Array<String> Plugin::GetDependencies() const
		{
			auto It = _FuncMap.find("CKPEPlugin_GetDependencies");
			if ((It == _FuncMap.end()) || !It->second) return {};
			Array<String> DependList;
			fastCall<void>(It->second, &DependList);
			return DependList;
		}

		const char* Plugin::GetVersion() const
		{
			auto It = _FuncMap.find("CKPEPlugin_GetVersion");
			if ((It == _FuncMap.end()) || !It->second) return nullptr;
			if (!fastCall<bool>(It->second, szTemp, ARRAYSIZE(szTemp)))
				return nullptr;
			return szTemp;
		}

		bool Plugin::HasOption() const
		{
			return false;
		}

		const char* Plugin::GetOptionName() const
		{
			return nullptr;
		}

		bool Plugin::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			// Check name
			String NamePlugin = GetName();
			if (NamePlugin.find_first_of("CKPEPlugin_"))
			{
				_ERROR("Library \"%s\" has an invalid plugin name \"%s\"",
					_PluginDllName.c_str(), NamePlugin.c_str());
				return false;
			}

			auto It = _FuncMap.find("CKPEPlugin_Query");
			if ((It == _FuncMap.end()) || !It->second) return false;
			return fastCall<bool>(It->second, eEditorCurrentVersion, lpcstrPlatformRuntimeVersion);
		}

		bool Plugin::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			DataTAG Data = 
			{
				PLUGINAPI_CURRENT_VERSION,
				Core::GlobalMemoryManagerPtr, 
				const_cast<Relocator*>(lpRelocator),
				Core::GlobalConsoleWindowPtr,
				_Log.Get(),
				Core::GlobalEnginePtr,
				Core::GlobalDialogManagerPtr,
				GlobalPluginMenuStartId,
				GlobalPluginMenuEndId,
				&Menu,
			};

			auto It = _FuncMap.find("CKPEPlugin_Init");
			if ((It == _FuncMap.end()) || !It->second) return false;
			bool bRet = fastCall<bool>(It->second, &Data);

			if (bRet && Menu)
			{
				_MenuName = Data.szSubMenuName;
				GlobalPluginMenuStartId = std::min(GlobalPluginMenuEndId + 1, PLUGIN_MENUID_MAX);
				GlobalPluginMenuEndId = std::min(GlobalPluginMenuStartId + 4, PLUGIN_MENUID_MAX);
			}

			return bRet;
		}

		bool Plugin::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			DataTAG Data =
			{
				PLUGINAPI_CURRENT_VERSION,
				Core::GlobalMemoryManagerPtr,
				const_cast<Relocator*>(lpRelocator),
				Core::GlobalConsoleWindowPtr,
				_Log.Get(),
				Core::GlobalEnginePtr,
				Core::GlobalDialogManagerPtr,
				0,
				0,
				nullptr,
			};

			auto It = _FuncMap.find("CKPEPlugin_Shutdown");
			if ((It == _FuncMap.end()) || !It->second) return false;
			return fastCall<bool>(It->second, &Data);
		}
	}
}