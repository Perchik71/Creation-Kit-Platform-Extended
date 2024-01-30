// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Module.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		class Plugin : public Module
		{
		public:
			Plugin(Engine* lpEngine, const char* lpcstrPluginDllName);
			virtual ~Plugin();

			virtual bool HasCanRuntimeDisabled() const;
			virtual const char* GetName() const;
			virtual bool HasDependencies() const;
			virtual Array<String> GetDependencies() const;
			const char* GetVersion() const;
			virtual bool HasOption() const;
			virtual const char* GetOptionName() const;

			inline bool IsOk() const { return IsInit; }
		protected:
			virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion, 
				const char* lpcstrPlatformRuntimeVersion) const;
			virtual bool Activate(const Relocator* lpRelocator, 
				const RelocationDatabaseItem* lpRelocationDatabaseItem);
			virtual bool Shutdown(const Relocator* lpRelocator, 
				const RelocationDatabaseItem* lpRelocationDatabaseItem);
		private:
			Plugin(const Plugin&) = default;
			Plugin& operator=(const Plugin&) = default;

			String _PluginDllName;
			HMODULE _Handle;
			bool IsInit;
			char szTemp[MAX_PATH];
			SmartPointer<Core::DebugLog> _Log;
			Map<std::string_view, uintptr_t> _FuncMap;
		};
	}
}