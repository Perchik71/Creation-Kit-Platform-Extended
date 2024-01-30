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

			bool Has(const char* name) const;
			bool Append(Plugin* lpModule);
			void Append(std::initializer_list<Plugin*> modulesList);
			void Remove(const char* name);

			void FindPlugins();

			SmartPointer<Plugin> GetByName(const char* name) const;
			inline uint32_t Count() const { return (uint32_t)_plugins.size(); }

			void Clear();

			void QueryAll();
			void EnableAll();
		private:
			PluginManager(const PluginManager&) = default;
			PluginManager& operator=(const PluginManager&) = default;

			Map<String, SmartPointer<Plugin>> _plugins;
		};
	}
}