// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Module.h"
#include "Core/Relocator.h"
#include "Core/RelocationDatabase.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		using namespace CreationKitPlatformExtended::Core;

		class ConsolePatch : public Module
		{
		public:
			ConsolePatch();

			virtual bool HasOption() const;
			virtual bool HasCanRuntimeDisabled() const;
			virtual const char* GetOptionName() const;
			virtual const char* GetName() const;
			virtual bool HasDependencies() const;
			virtual Array<String> GetDependencies() const;

			static void Log(const char* Format, ...);
			static void LogVa(const char* Format, va_list Va);
			static void LogWarning(int Type, const char* Format, ...);
			static void LogWarningVa(int Type, const char* Format, va_list& Va);
			static void LogWarningUnknown1(const char* Format, ...);
			static void LogWarningUnknown2(__int64 Unused, const char* Format, ...);
			static void LogAssert(const char* File, int Line, const char* Message, ...);
		protected:
			virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const;
			virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
		private:
			ConsolePatch(const ConsolePatch&) = default;
			ConsolePatch& operator=(const ConsolePatch&) = default;
		};
	}
}