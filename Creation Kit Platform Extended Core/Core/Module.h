// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		class Engine;
		class Relocator;
		class RelocationDatabaseItem;
		enum EDITOR_EXECUTABLE_TYPE;

		class Module
		{
		public:
			Module(Engine* lpEngine);
			virtual ~Module();

			bool Query(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion, const char* lpcstrPlatformRuntimeVersion) const;
			void Enable(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			void Disable();

			inline bool HasActive() const { return _Active; }

			virtual bool HasOption() const = 0;
			virtual bool HasCanRuntimeDisabled() const = 0;
			virtual const char* GetOptionName() const = 0;
			virtual const char* GetName() const = 0;
		protected:
			virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion, const char* lpcstrPlatformRuntimeVersion) const = 0;
			virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem) = 0;
			virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem) = 0;
		protected:
			Engine* _engine;
			const Relocator* _relocator;
			const RelocationDatabaseItem* _relocationDatabaseItem;
			bool _Active;
		};
	}
}