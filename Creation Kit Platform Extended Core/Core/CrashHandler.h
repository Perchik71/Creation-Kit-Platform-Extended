// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		class Engine;

		class CrashHandler
		{
		public:
			struct Region { uintptr_t Start, End; };
			struct Module { uintptr_t Start, End; Region SecCode; };
			using ModuleMapInfo = UnorderedMap<String, Module>;
			using ArrayMemoryInfo = Array<Region>;

			CrashHandler();
			~CrashHandler();

			void InstallThreadTheCrashSystem();

			static void PrintException(FILE* Stream, ModuleMapInfo* Modules, PEXCEPTION_RECORD lpExceptionRecord);
			static void PrintSettings(FILE* Stream);
			static void PrintSysInfo(FILE* Stream);
			static void PrintMemory(FILE* Stream, ArrayMemoryInfo* Memory);
			static void PrintRegistry(FILE* Stream, ModuleMapInfo* Modules, ArrayMemoryInfo* Memory, PEXCEPTION_POINTERS lpExceptionRecord);
			static void PrintRegistrySafe(FILE* Stream, ModuleMapInfo* Modules, ArrayMemoryInfo* Memory, PEXCEPTION_POINTERS lpExceptionRecord);
			static void PrintCallStack(FILE* Stream, ModuleMapInfo* Modules);
			static void PrintCallStackSafe(FILE* Stream, ModuleMapInfo* Modules);
			static void PrintStack(FILE* Stream, ModuleMapInfo* Modules, ArrayMemoryInfo* Memory, PEXCEPTION_POINTERS lpExceptionInfo);
			static void PrintStackSafe(FILE* Stream, ModuleMapInfo* Modules, ArrayMemoryInfo* Memory, PEXCEPTION_POINTERS lpExceptionInfo);
			static void PrintPatches(FILE* Stream);
			static void PrintModules(FILE* Stream, ModuleMapInfo* Modules);
			static void PrintPlugins(FILE* Stream);

			static LONG WINAPI DumpExceptionHandler(PEXCEPTION_POINTERS ExceptionInfo);
			static void ContextWriteToCrashLogSafe(FILE* Stream, PEXCEPTION_POINTERS ExceptionInfo);
			static void ContextWriteToCrashLog(FILE* Stream, PEXCEPTION_POINTERS ExceptionInfo);
			
			static void AssertWithCrashReport(LPCSTR File, int Line, LPCSTR ErrorMessage, PEXCEPTION_POINTERS ExceptionInfo);
		private:
			void SetProcessExceptionHandlers();
			void SetThreadExceptionHandlers();
		};

		extern CrashHandler* GlobalCrashHandlerPtr;
	}
}