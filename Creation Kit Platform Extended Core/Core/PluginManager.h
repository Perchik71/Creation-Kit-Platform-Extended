// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Plugin.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		class PluginManager
		{
		public:
			PluginManager();
			virtual ~PluginManager();

			[[nodiscard]] bool Has(const char* name) const;
			bool Append(Plugin* lpModule);
			void Append(std::initializer_list<Plugin*> modulesList);
			void Remove(const char* name);

			void FindPlugins();
			[[nodiscard]] SmartPointer<Plugin> GetByName(const char* name) const;
			[[nodiscard]] inline uint32_t Count() const noexcept(true) { return (uint32_t)_plugins.size(); }
			[[nodiscard]] const Map<String, SmartPointer<Plugin>>* GetPluginMap() const noexcept(true) { return &_plugins; }

			void CreatePluginsMenu(HMENU MainMenu, uint32_t MenuID);
			LRESULT ProcessingMenuMessages(HWND Hwnd, UINT Message, WPARAM wParam,
				LPARAM lParam, bool bContinue);
			[[nodiscard]] inline HMENU GetPluginsMenu() const noexcept(true) { return _PluginsMenu; }

			void Clear();

			void QueryAll();
			void EnableAll();
		private:
			PluginManager(const PluginManager&) = default;
			PluginManager& operator=(const PluginManager&) = default;

			Map<String, SmartPointer<Plugin>> _plugins;
			Map<uint32_t, uintptr_t> _PlugingsActionManager;
			HMENU _PluginsMenu;
		};
	}
}