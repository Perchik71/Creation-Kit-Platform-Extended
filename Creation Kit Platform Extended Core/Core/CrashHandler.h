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
			CrashHandler();
			~CrashHandler();

			void InstallThreadTheCrashSystem();

			static LONG WINAPI DumpExceptionHandler(PEXCEPTION_POINTERS ExceptionInfo);
			static void ContextWriteToCrashLog(FILE* Stream, PEXCEPTION_POINTERS ExceptionInfo);
		private:
			void SetProcessExceptionHandlers();
			void SetThreadExceptionHandlers();
		};

		extern CrashHandler* GlobalCrashHandlerPtr;
	}
}