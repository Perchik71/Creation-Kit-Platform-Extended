// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <comdef.h>

#include "Version/resource_version2.h"
#include "Engine.h"
#include "Plugin.h"
#include "DialogManager.h"
#include "RegistratorWindow.h"
#include "Editor API/BSString.h"
#include "../Plug-ins/MyFirstPlugin/CKPE/PluginAPI.h"

#pragma warning(disable : 4742)

#include "Editor API/SSE/TESDataHandler.h"
#include "Editor API/FO4/TESDataHandler.h"
#include "Editor API/SF/TESDataHandler.h"

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
			void* DataHandler;
			Core::DynamicCast* DynamicCast;
			Core::RegistratorWindow* RegistratorWindow;
		};

		extern uint32_t GlobalPluginMenuStartId;
		extern uint32_t GlobalPluginMenuEndId;

		Plugin::Plugin(Engine* lpEngine, const char* lpcstrPluginDllName) :
			Module(lpEngine), _PluginDllName(PathFindFileNameA(lpcstrPluginDllName)), _Handle(nullptr), IsInit(true),
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

			auto path = EditorAPI::BSString::Utils::GetApplicationPath();
			path += "CKPEPlugins\\";
			path += _PluginDllName.c_str();

			_Handle = LoadLibraryA(path.c_str());
			if (!_Handle)
			{
				auto ErrCode = GetLastError();
				if (!ErrCode)
					_ERROR("Couldn't open library \"%s\": Undefined error", path.c_str());
				else
					_ERROR("Couldn't open library \"%s\": #%X %s", path.c_str(), ErrCode, _com_error(ErrCode).ErrorMessage());
				
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
				_Log = new Core::DebugLog(EditorAPI::BSString::Utils::ChangeFileExt(path.c_str(), ".log").c_str());
				
				auto OsVer = lpEngine->GetSystemVersion();		
				_Log->FormattedMessage("Creation Kit Platform Extended Runtime: Initialize (Version: %s, OS: %u.%u Build %u)",
					VER_FILE_VERSION_STR, OsVer.MajorVersion, OsVer.MinorVersion, OsVer.BuildNubmer);

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

		bool Plugin::HasCanRuntimeDisabled() const noexcept(true)
		{
			auto It = _FuncMap.find("CKPEPlugin_HasCanRuntimeDisabled");
			if ((It == _FuncMap.end()) || !It->second) return false;
			return fastCall<bool>(It->second);
		}

		const char* Plugin::GetName() const noexcept(true)
		{
			auto It = _FuncMap.find("CKPEPlugin_GetName");
			if ((It == _FuncMap.end()) || !It->second) return nullptr;
			if (!fastCall<bool>(It->second, szTemp, ARRAYSIZE(szTemp)))
				return nullptr;
			return szTemp;
		}

		bool Plugin::HasDependencies() const noexcept(true)
		{
			auto It = _FuncMap.find("CKPEPlugin_HasDependencies");
			if ((It == _FuncMap.end()) || !It->second) return false;
			return fastCall<bool>(It->second);
		}

		Array<String> Plugin::GetDependencies() const noexcept(true)
		{
			auto It = _FuncMap.find("CKPEPlugin_GetDependencies");
			if ((It == _FuncMap.end()) || !It->second) return {};
			Array<String> DependList;
			fastCall<void>(It->second, &DependList);
			return DependList;
		}

		const char* Plugin::GetVersion() const noexcept(true)
		{
			auto It = _FuncMap.find("CKPEPlugin_GetVersion");
			if ((It == _FuncMap.end()) || !It->second) return nullptr;
			if (!fastCall<bool>(It->second, szTemp, ARRAYSIZE(szTemp)))
				return nullptr;
			return szTemp;
		}

		bool Plugin::HasOption() const noexcept(true)
		{
			return false;
		}

		const char* Plugin::GetOptionName() const noexcept(true)
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
			void* DataHandler = nullptr;
			auto editorVer = Core::GlobalEnginePtr->GetEditorVersion();
			if (editorVer <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST)
				DataHandler = new EditorAPI::SkyrimSpectialEdition::TESDataHandler_CKPEIntf;
			else if (editorVer <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST)
				DataHandler = new EditorAPI::Fallout4::TESDataHandler_CKPEIntf;
			else if (editorVer <= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_LAST)
				DataHandler = new EditorAPI::Starfield::TESDataHandler_CKPEIntf;
	
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
				"",
				DataHandler,
				Core::GlobalDynamicCastPtr,
				Core::GlobalRegistratorWindowPtr
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

			if (!bRet)
			{
				if (editorVer <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST)
					delete (EditorAPI::SkyrimSpectialEdition::TESDataHandler_CKPEIntf*)DataHandler;
				else if (editorVer <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST)
					delete (EditorAPI::Fallout4::TESDataHandler_CKPEIntf*)DataHandler;
				else if (editorVer <= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_LAST)
					delete (EditorAPI::Starfield::TESDataHandler_CKPEIntf*)DataHandler;
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
				"",
				nullptr,
				Core::GlobalDynamicCastPtr,
				Core::GlobalRegistratorWindowPtr
			};

			auto It = _FuncMap.find("CKPEPlugin_Shutdown");
			if ((It == _FuncMap.end()) || !It->second) return false;
			return fastCall<bool>(It->second, &Data);
		}
	}
}

#pragma warning(default : 4742)