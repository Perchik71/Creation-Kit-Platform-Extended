// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Version/resource_version2.h"
#include "Engine.h"
#include "PluginManager.h"
#include "Relocator.h"

#include "..\MyFirstPlugin\CKPE\PluginAPI.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		PluginManager::PluginManager()
		{}

		PluginManager::~PluginManager()
		{
			Clear();
		}

		bool PluginManager::Has(const char* name) const
		{
			for (auto It = _plugins.begin(); It != _plugins.end(); It++)
			{
				if (!_stricmp(It->first.c_str(), name))
					return true;
			}

			return false;
		}

		bool PluginManager::Append(Plugin* lpModule)
		{
			if (!lpModule || !lpModule->GetName() || Has(lpModule->GetName()))
				return false;

			_plugins.insert(std::make_pair<String, SmartPointer<Plugin>>(lpModule->GetName(), lpModule));
			return true;
		}

		void PluginManager::Append(std::initializer_list<Plugin*> modulesList)
		{
			for (auto It = modulesList.begin(); It != modulesList.end(); It++)
				Append(*It);
		}

		void PluginManager::Remove(const char* name)
		{
			_plugins.erase(name);
		}

		void PluginManager::FindPlugins()
		{
			String PathDll = ".\\CKPEPlugins\\";
			WIN32_FIND_DATAA FindFileData;
			HANDLE hFind = FindFirstFileExA((PathDll + "*.dll").c_str(), FindExInfoStandard, 
				&FindFileData, FindExSearchNameMatch, NULL, FIND_FIRST_EX_LARGE_FETCH);
			if (hFind == INVALID_HANDLE_VALUE)
			{
				auto Ret = GetLastError();
				if ((Ret != ERROR_PATH_NOT_FOUND) && (Ret != ERROR_FILE_NOT_FOUND))
					_ERROR("FindFirstFile failed (%d)\n", Ret);
				return;
			}
			else
			{
				do
				{
					auto NewPlugin = new Plugin(GlobalEnginePtr, (PathDll + FindFileData.cFileName).c_str());
					if (!NewPlugin->IsOk())
					{
						delete NewPlugin;

						_MESSAGE("Library \"%s\" is not a plugin for CKPE or an initialization error occurred", 
							FindFileData.cFileName);
						continue;
					}
					
					if (Append(NewPlugin))
						_MESSAGE("Library \"%s\" successfully added", FindFileData.cFileName);
					else
					{
						delete NewPlugin;

						_WARNING("Library \"%s\" failed to add, the name may be busy", FindFileData.cFileName);
					}
				} while (FindNextFileA(hFind, &FindFileData) != 0);

				FindClose(hFind);
			}
		}

		SmartPointer<Plugin> PluginManager::GetByName(const char* name) const
		{
			for (auto It = _plugins.begin(); It != _plugins.end(); It++)
			{
				if (!_stricmp(It->first.c_str(), name))
					return It->second;
			}

			return nullptr;
		}

		void PluginManager::Clear()
		{
			_plugins.clear();
		}

		void PluginManager::QueryAll()
		{
			Array<Map<String, SmartPointer<Plugin>>::iterator> RejectedModules;

			for (auto It = _plugins.begin(); It != _plugins.end(); It++)
			{
				if (!It->second->Query(GlobalEnginePtr->GetEditorVersion(), VER_FILE_VERSION_STR))
					RejectedModules.push_back(It);
			}

			// Удаляем отбракованные
			for (auto It = RejectedModules.begin(); It != RejectedModules.end(); It++)
				_plugins.erase(*It);
		}

		void PluginManager::EnableAll()
		{
			uint32_t count = 0;

			for (auto It = _plugins.begin(); It != _plugins.end(); It++)
			{
				It->second->Enable(GlobalRelocatorPtr, /* hack */
					(const RelocationDatabaseItem*)((void*)1));
				if (It->second->HasActive())
					count++;

				String Name = It->second->GetName();
				String Ver = It->second->GetVersion();
				_MESSAGE("Plugin \"%s\" version \"%s\"", 
					(!Name.empty() ? Name.c_str() : "UNKNOWN"), 
					(!Ver.empty() ? Ver.c_str() : "UNKNOWN"));
			}

			_MESSAGE("Plugins installed: %u from %llu", count, _plugins.size());
		}
	}
}