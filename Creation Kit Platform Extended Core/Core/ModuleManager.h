// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Module.h" 

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		class ModuleManager
		{
		public:
			ModuleManager();
			virtual ~ModuleManager();

			bool Has(const char* name) const;
			bool Append(Module* lpModule);
			void Append(std::initializer_list<Module*> modulesList);

			SmartPointer<Module> GetByName(const char* name) const;
			inline uint32_t Count() const { return (uint32_t)_modules.size(); }

			void Clear();

			void QueryAll();
			void EnableAll();
		private:
			ModuleManager(const ModuleManager&) = default;
			ModuleManager& operator=(const ModuleManager&) = default;

			Map<String, SmartPointer<Module>> _modules;
		};
	}
}