// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "ThreadPatch.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		ThreadPatch::ThreadPatch() : Module(GlobalEnginePtr)
		{}

		bool ThreadPatch::HasOption() const
		{
			return false;
		}

		bool ThreadPatch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* ThreadPatch::GetOptionName() const
		{
			return nullptr;
		}

		const char* ThreadPatch::GetName() const
		{
			return "Threads";
		}

		bool ThreadPatch::HasDependencies() const
		{
			return false;
		}

		Array<String> ThreadPatch::GetDependencies() const
		{
			return {};
		}

		bool ThreadPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return true;
		}

		bool ThreadPatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			if (lpRelocationDatabaseItem->Version() == 1)
			{
				PatchIAT(HKSetThreadPriority, "kernel32.dll", "SetThreadPriority");
				PatchIAT(HKSetThreadAffinityMask, "kernel32.dll", "SetThreadAffinityMask");
				
				PatchIAT(HKSleep, "kernel32.dll", "Sleep");
				PatchIAT(HKSleepEx, "kernel32.dll", "SleepEx");

				auto hCurrentProcess = GetCurrentProcess();
				if (hCurrentProcess)
				{
					SetPriorityClass(hCurrentProcess, HIGH_PRIORITY_CLASS/*ABOVE_NORMAL_PRIORITY_CLASS*/);

					DWORD_PTR ProcessMask, SystemMask;
					GetProcessAffinityMask(hCurrentProcess, &ProcessMask, &SystemMask);
					_MESSAGE("ProcessMask: 0x%X", ProcessMask);
					_MESSAGE("SystemMask: 0x%X", SystemMask);

					if (ProcessMask != SystemMask)
					{
						_MESSAGE("A change in the usage of processor cores has been detected");

						SetProcessAffinityMask(hCurrentProcess, SystemMask);
					}
				}

				auto hCurrentThread = GetCurrentThread();
				if (hCurrentThread)
				{
					auto ThreadID = GetThreadId(hCurrentThread);
					_MESSAGE("CKPE_ThreadMain %u", ThreadID);

					SetThreadPriority(hCurrentThread, THREAD_PRIORITY_HIGHEST);
					Utils::SetThreadName(ThreadID, "CKPE_ThreadMain");
				}

				// The system does not display the critical-error-handler message box. 
				// Instead, the system sends the error to the calling process.
				// Best practice is that all applications call the process - wide SetErrorMode 
				// function with a parameter of SEM_FAILCRITICALERRORS at startup.
				// This is to prevent error mode dialogs from hanging the application.
				DWORD OldErrMode;
				AssertMsg(SetThreadErrorMode(SEM_FAILCRITICALERRORS, &OldErrMode), "Can't set the error mode for the application.");

				return true;
			}
			
			return false;
		}

		bool ThreadPatch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}

		BOOL ThreadPatch::HKSetThreadPriority(HANDLE hThread, int nPriority)
		{
			// Don't allow a priority below normal and below current priority
			auto CurrentPriority = GetThreadPriority(hThread);
			CurrentPriority = CurrentPriority > THREAD_PRIORITY_NORMAL ? CurrentPriority : THREAD_PRIORITY_NORMAL;
			return SetThreadPriority(hThread, std::max(CurrentPriority, nPriority));
		}

		DWORD_PTR ThreadPatch::HKSetThreadAffinityMask(HANDLE hThread, DWORD_PTR dwThreadAffinityMask)
		{
			// Don't change anything
			return 0xFFFFFFFF;
		}

		void ThreadPatch::HKSleep(DWORD dwMilliseconds)
		{
			// Bethesda's spinlock calls Sleep(0) every iteration until 10,000. Then it
			// uses Sleep(1). Even with 0ms waits, there's a tiny performance penalty.
			if (dwMilliseconds == 0)
				return;

			SleepEx(dwMilliseconds, FALSE);
		}

		void ThreadPatch::HKSleepEx(DWORD dwMilliseconds, BOOL bAlertable)
		{
			// Bethesda's spinlock calls Sleep(0) every iteration until 10,000. Then it
			// uses Sleep(1). Even with 0ms waits, there's a tiny performance penalty.
			if (dwMilliseconds == 0)
				return;

			SleepEx(dwMilliseconds, bAlertable);
		}
	}
}