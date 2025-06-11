// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Module.h>
#include <CKPE.StringUtils.h>
#include <CKPE.HardwareInfo.h>
#include <CKPE.Common.Threads.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Exception.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <algorithm>

namespace CKPE
{
	namespace Common
	{
		std::uintptr_t sbase_kernel32 = 0;

		void Threads::SetThreadName(std::uint32_t ThreadID, const char* ThreadName)
		{
#pragma pack(push, 8)  
			const DWORD MS_VC_EXCEPTION = 0x406D1388;

			typedef struct tagTHREADNAME_INFO {
				DWORD dwType;		// Must be 0x1000.  
				LPCSTR szName;		// Pointer to name (in user addr space).  
				DWORD dwThreadID;	// Thread ID (-1=caller thread).  
				DWORD dwFlags;		// Reserved for future use, must be zero.  
			} THREADNAME_INFO;
#pragma pack(pop)

			THREADNAME_INFO info = { 0 };
			info.dwType = 0x1000;
			info.szName = ThreadName;
			info.dwThreadID = ThreadID;
			info.dwFlags = 0;

			__try 
			{
				RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {}
		}

		bool Threads::SetThreadDesc(void* thread, const char* ThreadName)
		{
			auto ver = HardwareInfo::OS::GetVersion();
			if ((GET_EXE_VERSION_EX_MAJOR(ver) > 10) ||
				((GET_EXE_VERSION_EX_MAJOR(ver) == 10) && (GET_EXE_VERSION_EX_BUILD(ver) == 1607)))
			{
				auto proc = (decltype(&SetThreadDescription))GetProcAddress((HMODULE)sbase_kernel32, "SetThreadDescription");
				if (proc)
					return SUCCEEDED(proc(thread, StringUtils::Utf8ToUtf16(ThreadName).c_str()));
				return false;
			}
			return false;
		}

		bool Threads::Hook::HKSetThreadPriority(void* thread, std::int32_t priority) noexcept(true)
		{
			// Don't allow a priority below normal and below current priority
			auto CurrentPriority = (std::int32_t)GetThreadPriority((HANDLE)thread);
			CurrentPriority = CurrentPriority > THREAD_PRIORITY_NORMAL ? CurrentPriority : THREAD_PRIORITY_NORMAL;
			return SetThreadPriority((HANDLE)thread, std::max(CurrentPriority, priority));
		}

		std::uintptr_t Threads::Hook::HKSetThreadAffinityMask(void* thread, std::uintptr_t affinity_mask) noexcept(true)
		{
			// Don't change anything
			return (std::uintptr_t)-1;
		}

		std::uint32_t Threads::Hook::HKSetThreadIdealProcessor(void* thread, std::uint32_t mask) noexcept(true)
		{
			// Don't change anything
			return S_FALSE;
		}

		void* Threads::Hook::HKCreateThread(void* attributes, std::uint64_t stack_size, void* start_address,
			void* parameter, std::uint32_t flags, std::uint32_t* id) noexcept(true)
		{
			auto thread = CreateThread((LPSECURITY_ATTRIBUTES)attributes, stack_size, (LPTHREAD_START_ROUTINE)start_address,
				parameter, flags, (LPDWORD)id);
			if (thread) SetThreadPriority(thread, THREAD_PRIORITY_NORMAL);
			return (void*)thread;
		}

		void Threads::Hook::HKSleep(std::uint32_t ms) noexcept(true)
		{
			// Bethesda's spinlock calls Sleep(0) every iteration until 10,000. Then it
			// uses Sleep(1). Even with 0ms waits, there's a tiny performance penalty.
			if (ms == 0)
				return;

			SleepEx(ms, false);
		}

		void Threads::Hook::HKSleepEx(std::uint32_t ms, bool alertable) noexcept(true)
		{
			// Bethesda's spinlock calls Sleep(0) every iteration until 10,000. Then it
			// uses Sleep(1). Even with 0ms waits, there's a tiny performance penalty.
			if (ms == 0)
				return;

			SleepEx(ms, alertable);
		}

		void Threads::Hook::Initialize() noexcept(true)
		{
			auto base = (std::uintptr_t)GetModuleHandleA(nullptr);
			sbase_kernel32 = (std::uintptr_t)GetModuleHandleA("kernel32.dll");

			Detours::DetourIAT(base, "kernel32.dll", "CreateThread", (std::uintptr_t)HKCreateThread);
			Detours::DetourIAT(base, "kernel32.dll", "SetThreadPriority", (std::uintptr_t)HKSetThreadPriority);
			Detours::DetourIAT(base, "kernel32.dll", "SetThreadAffinityMask", (std::uintptr_t)HKSetThreadAffinityMask);
			Detours::DetourIAT(base, "kernel32.dll", "SetThreadIdealProcessor", (std::uintptr_t)HKSetThreadIdealProcessor);
			
#if 0
			Detours::DetourIAT(base, "kernel32.dll", "Sleep", (std::uintptr_t)HKSleep);
			Detours::DetourIAT(base, "kernel32.dll", "SleepEx", (std::uintptr_t)HKSleepEx);
#endif

			auto hCurrentProcess = GetCurrentProcess();
			if (hCurrentProcess)
			{
				if (!SetPriorityClass(hCurrentProcess, HIGH_PRIORITY_CLASS))
				{
					auto ErrorLast = GetLastError();
					_ERROR("SetPriorityClass returned failed (0x%x): %s", ErrorLast, 
						ErrorHandler::GetSystemMessage(ErrorLast).c_str());
				}
				else
					_MESSAGE("Set high priority has been set for process");

				if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST))
				{
					auto ErrorLast = GetLastError();
					_ERROR("SetThreadPriority returned failed (0x%x): %s", ErrorLast, 
						ErrorHandler::GetSystemMessage(ErrorLast).c_str());
				}
				else
					_MESSAGE("Set high priority has been set for main thread");

				DWORD_PTR ProcessMask, SystemMask;
				if (!GetProcessAffinityMask(hCurrentProcess, &ProcessMask, &SystemMask))
				{
					auto ErrorLast = GetLastError();
					_ERROR("GetProcessAffinityMask returned failed (0x%x): %s", ErrorLast,
						ErrorHandler::GetSystemMessage(ErrorLast).c_str());
				}
				else
				{
					_MESSAGE("ProcessMask: 0x%X", ProcessMask);
					_MESSAGE("SystemMask: 0x%X", SystemMask);

					auto NumCores = HardwareInfo::CPU::GetTotalCores();
					if (NumCores >= 4)
					{
						// max 50% for cpu
						NumCores >>= 1;
						DWORD dwMask = 0;

						for (uint8_t i = 0; i < NumCores; i++)
							dwMask |= 1 << i;

						if (SetProcessAffinityMask(hCurrentProcess, dwMask))
							_MESSAGE("InstallMask: 0x%X", dwMask);
					}
					else
					{
						if (SetProcessAffinityMask(hCurrentProcess, SystemMask))
							_MESSAGE("InstallMask: 0x%X", SystemMask);
					}
				}
			}

			auto hCurrentThread = GetCurrentThread();
			if (hCurrentThread)
			{
				auto ThreadID = GetThreadId(hCurrentThread);
				_MESSAGE("CKPE_ThreadMain %u", ThreadID);

				SetThreadPriority(hCurrentThread, THREAD_PRIORITY_HIGHEST);
				SetThreadName(ThreadID, "CKPE_ThreadMain");
				SetThreadDesc(hCurrentThread, "CKPE_ThreadMain");
			}

			if (sbase_kernel32)
			{
				auto SetPriorityClass_addr = GetProcAddress((HMODULE)sbase_kernel32, "SetPriorityClass");
				auto SetProcessAffinityMask_addr = GetProcAddress((HMODULE)sbase_kernel32, "SetProcessAffinityMask");
				if (SetPriorityClass_addr)
					SafeWrite::Write((std::uintptr_t)SetPriorityClass_addr, { 0x31, 0xC0, 0xC3, 0x90, });
				if (SetProcessAffinityMask_addr)
					SafeWrite::Write((std::uintptr_t)SetProcessAffinityMask_addr, { 0x31, 0xC0, 0xC3, 0x90, });
			}
		}
	}
}