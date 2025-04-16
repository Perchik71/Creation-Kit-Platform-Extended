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

			[[nodiscard]] bool Has(const char* name) const;
			bool Append(Module* lpModule);
			void Append(std::initializer_list<Module*> modulesList);
			void Remove(const char* name);

			[[nodiscard]] SmartPointer<Module> GetByName(const char* name) const;
			[[nodiscard]] inline uint32_t Count() const { return (uint32_t)_modules.size(); }
			[[nodiscard]] const Map<String, SmartPointer<Module>>* GetModuleMap() const noexcept(true) { return &_modules; }

			void Clear();

			void QueryAll();
			void EnableAll();
		private:
			ModuleManager(const ModuleManager&) = default;
			ModuleManager& operator=(const ModuleManager&) = default;

			Map<String, SmartPointer<Module>> _modules;
			Array<String> _filter;
		};
	}
}