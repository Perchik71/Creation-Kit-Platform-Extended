// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Version/resource_version2.h"
#include "Engine.h"
#include "PluginManager.h"
#include "Relocator.h"

#include "Editor API/UI/UIMenus.h"
#include "../MyFirstPlugin/CKPE/PluginAPI.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		uint32_t GlobalPluginMenuStartId;
		uint32_t GlobalPluginMenuEndId;

		PluginManager::PluginManager() : _PluginsMenu(nullptr)
		{
			GlobalPluginMenuStartId = PLUGIN_MENUID_MIN;
			GlobalPluginMenuEndId = GlobalPluginMenuStartId + 4;
		}

		PluginManager::~PluginManager()
		{
			Clear();
			if (_PluginsMenu) 
				DestroyMenu(_PluginsMenu);
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

		void PluginManager::CreatePluginsMenu(HMENU MainMenu, uint32_t MenuID)
		{
			uint32_t Uses = 0;
			_PluginsMenu = CreateMenu();
			_PlugingsActionManager.clear();

			for (auto It = _plugins.begin(); It != _plugins.end(); It++)
			{
				auto Plugin = It->second.Get();	
				if (Plugin->HasMenu())
				{
					::Core::Classes::UI::CUIMenu SubMenu = Plugin->SubMenu;

					// From the names of the menu items, take the address of the function and restore the name

					for (UINT i = 0; i < SubMenu.Count(); i++)
					{
						auto Item = SubMenu.GetItemByPos(i);
						
						String Caption = Item.Text.c_str();
						auto It = Caption.find_first_of('\1');
						if (It == String::npos)
							continue;

						// Get address function
						char* EndPrefix = nullptr;
						_PlugingsActionManager[Item.ID] = 
							(LONG_PTR)_strtoui64(Caption.substr(It + 1).c_str(), &EndPrefix, 16);
						
						// Restore name
						Item.Text = Caption.substr(0, It).c_str();
					}

					MENUITEMINFOA mii = { 0 };
					mii.cbSize = sizeof(MENUITEMINFOA);
					mii.fMask = MIIM_STRING | MIIM_SUBMENU;
					mii.dwTypeData = (LPSTR)Plugin->SubMenuName;
					mii.cch = (UINT)strlen(mii.dwTypeData);
					mii.hSubMenu = Plugin->SubMenu;
			
					if (InsertMenuItemA(_PluginsMenu, 0, TRUE, &mii))
						Uses++;
				}
			}

			if (!Uses)
				DestroyMenu(_PluginsMenu);
			else
			{
				MENUITEMINFO mii
				{
					.cbSize = sizeof(MENUITEMINFO),
					.fMask = MIIM_SUBMENU | MIIM_ID | MIIM_STRING,
					.wID = MenuID,
					.hSubMenu = _PluginsMenu,
					.dwTypeData = const_cast<LPSTR>("Plug-ins"),
					.cch = static_cast<uint32_t>(strlen(mii.dwTypeData))
				};

				AssertMsg(InsertMenuItemA(MainMenu, -1, TRUE, &mii), "Failed to create plug-ins submenu");
			}
		}

		LRESULT PluginManager::ProcessingMenuMessages(HWND Hwnd, UINT Message, WPARAM wParam,
			LPARAM lParam, bool bContinue)
		{
			bContinue = true;

			if (Message == WM_COMMAND)
			{
				const uint32_t menuID = LOWORD(wParam);
				if ((menuID >= PLUGIN_MENUID_MIN) && (menuID < PLUGIN_MENUID_MAX))
				{
					if (_PlugingsActionManager.count(menuID) > 0)
					{
						auto Data = _PlugingsActionManager[menuID];
						if (Data)
						{
							// Calling the plugin function from memory
							fastCall<void>((uintptr_t)Data, menuID);
							// This completes any processing
							bContinue = false;
							return S_OK;
						}
					}
				}
			}

			return S_FALSE;
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
			_PlugingsActionManager.clear();
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