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

		class CrashDumpPatch : public Module
		{
		public:
			CrashDumpPatch();

			virtual bool HasOption() const;
			virtual bool HasCanRuntimeDisabled() const;
			virtual const char* GetOptionName() const;
			virtual const char* GetName() const;

			static DWORD WINAPI DumpWriterThread(LPVOID Arg);
			static LONG WINAPI DumpExceptionHandler(PEXCEPTION_POINTERS ExceptionInfo);
		protected:
			virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const;
			virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
		private:
			CrashDumpPatch(const CrashDumpPatch&) = default;
			CrashDumpPatch& operator=(const CrashDumpPatch&) = default;
		};
	}
}