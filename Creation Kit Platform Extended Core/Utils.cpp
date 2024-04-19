// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

namespace CreationKitPlatformExtended
{
	namespace Utils
	{
		void __Assert(LPCSTR File, int Line, LPCSTR Format, ...)
		{
			CHAR buffer[2048] = { 0 };
			CHAR message[2048] = { 0 };

			va_list ap;
			va_start(ap, Format);
			vsprintf_s(buffer, _TRUNCATE, Format, ap);
			sprintf_s(message, "%s(%d):\n\n%s", File, Line, buffer);
			va_end(ap);

			MessageBoxA(nullptr, message, "ASSERTION", MB_ICONERROR);

			if (IsDebuggerPresent()) {
				OutputDebugStringA(message);
				__debugbreak();
			}

			TerminateProcess(GetCurrentProcess(), 1);
			__assume(0);
		}

		void __Assert(LPCWSTR File, int Line, LPCWSTR Format, ...)
		{
			WCHAR buffer[2048] = { 0 };
			WCHAR message[2048] = { 0 };

			va_list ap;
			va_start(ap, Format);
			vswprintf_s(buffer, _TRUNCATE, Format, ap);
			swprintf_s(message, L"%s(%d):\n\n%s", File, Line, buffer);
			va_end(ap);

			MessageBoxW(nullptr, message, L"ASSERTION", MB_ICONERROR);

			if (IsDebuggerPresent()) {
				OutputDebugStringW(message);
				__debugbreak();
			}

			TerminateProcess(GetCurrentProcess(), 1);
			__assume(0);
		}

		void SetThreadName(uint32_t ThreadID, LPCSTR ThreadName)
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

			__try {
				RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {}
		}

		uint64_t GetTotalPhysicalMemory()
		{
			MEMORYSTATUSEX statex = { 0 };
			statex.dwLength = sizeof(MEMORYSTATUSEX);
			if (!GlobalMemoryStatusEx(&statex)) return 0;
			return (uint64_t)statex.ullTotalPhys;
		}

		uint64_t GetTotalPageFileMemory()
		{
			MEMORYSTATUSEX statex = { 0 };
			statex.dwLength = sizeof(MEMORYSTATUSEX);
			if (!GlobalMemoryStatusEx(&statex)) return 0;
			return (uint64_t)statex.ullTotalPageFile;
		}

		uint64_t GetAvailableTotalPhysicalMemory()
		{
			MEMORYSTATUSEX statex = { 0 };
			statex.dwLength = sizeof(MEMORYSTATUSEX);
			if (!GlobalMemoryStatusEx(&statex)) return 0;
			return (uint64_t)statex.ullAvailPhys;
		}

		uint64_t GetAvailableTotalPageFileMemory()
		{
			MEMORYSTATUSEX statex = { 0 };
			statex.dwLength = sizeof(MEMORYSTATUSEX);
			if (!GlobalMemoryStatusEx(&statex)) return 0;
			return (uint64_t)statex.ullAvailPageFile;
		}

		char* StrDub(const char* s)
		{
			auto l = strlen(s);
			return (char*)memcpy((char*)Core::GlobalMemoryManagerPtr->MemAlloc(l + 1), s, l + 1);
		}

		void Quit()
		{
			TerminateProcess(GetCurrentProcess(), 0);
		}
	}
}