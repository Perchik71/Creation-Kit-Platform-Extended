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

		uintptr_t FindPattern(uintptr_t StartAddress, uintptr_t MaxSize, const char* Mask)
		{
			std::vector<std::pair<uint8_t, bool>> pattern;

			for (size_t i = 0; i < strlen(Mask);) {
				if (Mask[i] != '?') {
					pattern.emplace_back((uint8_t)strtoul(&Mask[i], nullptr, 16), false);
					i += 3;
				}
				else {
					pattern.emplace_back(0x00, true);
					i += 2;
				}
			}

			const uint8_t* dataStart = (uint8_t*)StartAddress;
			const uint8_t* dataEnd = (uint8_t*)StartAddress + MaxSize + 1;

			auto ret = std::search(dataStart, dataEnd, pattern.begin(), pattern.end(),
				[](uint8_t CurrentByte, std::pair<uint8_t, bool>& Pattern) {
					return Pattern.second || (CurrentByte == Pattern.first);
				});

			if (ret == dataEnd)
				return 0;

			return std::distance(dataStart, ret) + StartAddress;
		}

		std::vector<uintptr_t> FindPatterns(uintptr_t StartAddress, uintptr_t MaxSize, const char* Mask)
		{
			std::vector<uintptr_t> results;
			std::vector<std::pair<uint8_t, bool>> pattern;

			for (size_t i = 0; i < strlen(Mask);) {
				if (Mask[i] != '?') {
					pattern.emplace_back((uint8_t)strtoul(&Mask[i], nullptr, 16), false);
					i += 3;
				}
				else {
					pattern.emplace_back(0x00, true);
					i += 2;
				}
			}

			const uint8_t* dataStart = (uint8_t*)StartAddress;
			const uint8_t* dataEnd = (uint8_t*)StartAddress + MaxSize + 1;

			for (const uint8_t* i = dataStart;;) {
				auto ret = std::search(i, dataEnd, pattern.begin(), pattern.end(),
					[](uint8_t CurrentByte, std::pair<uint8_t, bool>& Pattern) {
						return Pattern.second || (CurrentByte == Pattern.first);
					});

				// Не соответствует шаблону байтов, уходим
				if (ret == dataEnd)
					break;

				uintptr_t addr = std::distance(dataStart, ret) + StartAddress;
				results.push_back(addr);

				i = (uint8_t*)(addr + 1);
			}

			return results;
		}

		bool GetPESectionRange(uintptr_t ModuleBase, const char* Section, uintptr_t* Start, uintptr_t* End)
		{
			PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)(ModuleBase + 
				((PIMAGE_DOS_HEADER)ModuleBase)->e_lfanew);
			PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(ntHeaders);

			// Предположим, что заголовок PE отсутствует, если раздел отсутствует
			if (!Section || strlen(Section) <= 0) {
				if (Start)
					*Start = ModuleBase;

				if (End)
					*End = ModuleBase + ntHeaders->OptionalHeader.SizeOfHeaders;

				return true;
			}

			for (uint32_t i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++, section++) {
				// Имя может не заканчиваться нулем
				char sectionName[sizeof(IMAGE_SECTION_HEADER::Name) + 1] = { };
				memcpy(sectionName, section->Name, sizeof(IMAGE_SECTION_HEADER::Name));

				if (!strcmp(sectionName, Section)) {
					if (Start)
						*Start = ModuleBase + section->VirtualAddress;

					if (End)
						*End = ModuleBase + section->VirtualAddress + section->Misc.VirtualSize;

					return true;
				}
			}

			return false;
		}

		void PatchMemory(uintptr_t Address, uint8_t* Data, size_t Size) {
			DWORD d = 0;
			VirtualProtect((LPVOID)Address, Size, PAGE_EXECUTE_READWRITE, &d);

			for (uintptr_t i = Address; i < (Address + Size); i++)
				*(volatile uint8_t*)i = *Data++;

			VirtualProtect((LPVOID)Address, Size, d, &d);
			FlushInstructionCache(GetCurrentProcess(), (LPVOID)Address, Size);
		}

		void PatchMemory(uintptr_t Address, std::initializer_list<uint8_t> Data) {
			DWORD d = 0;
			VirtualProtect((LPVOID)Address, Data.size(), PAGE_EXECUTE_READWRITE, &d);

			uintptr_t i = Address;
			for (auto value : Data)
				*(volatile uint8_t*)i++ = value;

			VirtualProtect((LPVOID)Address, Data.size(), d, &d);
			FlushInstructionCache(GetCurrentProcess(), (LPVOID)Address, Data.size());
		}

		void PatchMemoryNop(uintptr_t Address, size_t Size) {
			DWORD d = 0;
			VirtualProtect((LPVOID)Address, Size, PAGE_EXECUTE_READWRITE, &d);

			for (uintptr_t i = Address; i < (Address + Size); i++)
				*(volatile uint8_t*)i = 0x90;

			VirtualProtect((LPVOID)Address, Size, d, &d);
			FlushInstructionCache(GetCurrentProcess(), (LPVOID)Address, Size);
		}

		void PatchMemoryWP(uintptr_t Address, uint8_t* Data, size_t Size)
		{
			for (uintptr_t i = Address; i < (Address + Size); i++)
				*(volatile uint8_t*)i = *Data++;
		}

		void PatchMemoryWP(uintptr_t Address, std::initializer_list<uint8_t> Data)
		{
			uintptr_t i = Address;
			for (auto value : Data)
				*(volatile uint8_t*)i++ = value;
		}

		void PatchMemoryNopWP(uintptr_t Address, size_t Size)
		{
			for (uintptr_t i = Address; i < (Address + Size); i++)
				*(volatile uint8_t*)i = 0x90;
		}

		DWORD UnlockWP(uintptr_t Address, size_t Size)
		{
			DWORD OldFlag = 0;
			if (VirtualProtect((LPVOID)Address, Size, PAGE_EXECUTE_READWRITE, &OldFlag))
				return OldFlag;
			return 0;
		}

		void LockWP(uintptr_t Address, size_t Size, DWORD OldFlags)
		{
			DWORD OldFlag = 0;
			VirtualProtect((LPVOID)Address, Size, OldFlags, &OldFlag);
			FlushInstructionCache(GetCurrentProcess(), (LPVOID)Address, Size);
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

		String GetLastErrorToStr(DWORD err, const String& namefunc)
		{
			// Retrieve the system error message for the last-error code

			String str;
			LPVOID lpMsgBuf = nullptr;
			LPVOID lpDisplayBuf = nullptr;

			FormatMessageA(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				err,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPSTR)&lpMsgBuf,
				0, NULL);

			// Display the error message and exit the process

			lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
				((size_t)strlen((LPCSTR)lpMsgBuf) + strlen(namefunc.c_str()) + 40) * sizeof(CHAR));
			StringCchPrintfA((LPSTR)lpDisplayBuf,
				LocalSize(lpDisplayBuf) / sizeof(CHAR),
				"%s failed with error %d: %s",
				namefunc.c_str(), err, lpMsgBuf);
			str = (LPSTR)lpDisplayBuf;

			LocalFree(lpMsgBuf);
			LocalFree(lpDisplayBuf);

			return str;
		}

		String GetLastErrorToStr(const String& namefunc)
		{
			return GetLastErrorToStr(GetLastError(), namefunc);
		}

		bool FileExists(const char* fileName)
		{
			if (!fileName) return false;
			auto Result = GetFileAttributesA(fileName);
			if (Result == INVALID_FILE_ATTRIBUTES || (Result & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
				return false;
			return true;
		}

		bool FileExists(const wchar_t* fileName)
		{
			if (!fileName) return false;
			auto Result = GetFileAttributesW(fileName);
			if (Result == INVALID_FILE_ATTRIBUTES || (Result & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
				return false;
			return true;
		}

		bool DirectoryExists(const char* fileName)
		{
			if (!fileName) return false;
			auto Result = GetFileAttributesA(fileName);
			if (Result == INVALID_FILE_ATTRIBUTES || (Result & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
				return false;
			return true;
		}

		bool DirectoryExists(const wchar_t* fileName)
		{
			if (!fileName) return false;
			auto Result = GetFileAttributesW(fileName);
			if (Result == INVALID_FILE_ATTRIBUTES || (Result & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
				return false;
			return true;
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

		String Wide2Ansi(const wchar_t* s)
		{
			int l = WideCharToMultiByte(CP_ACP, 0, s, -1, nullptr, 0, nullptr, nullptr);
			if (l > 0)
			{
				auto Str = std::make_unique<char[]>((size_t)l + 1);
				WideCharToMultiByte(CP_ACP, 0, s, -1, Str.get(), l, nullptr, nullptr);
				return Str.get();
			}
			return "";
		}

		WideString Ansi2Wide(const char* s)
		{
			int l = MultiByteToWideChar(CP_ACP, 0, s, -1, nullptr, 0);
			if (l > 0)
			{
				auto Str = std::make_unique<wchar_t[]>((size_t)l + 1);
				MultiByteToWideChar(CP_ACP, 0, s, -1, Str.get(), l);
				return Str.get();
			}
			return L"";
		}

		WideString Utf82Wide(const char8_t* s)
		{
			int l = MultiByteToWideChar(CP_UTF8, 0, (char*)s, -1, nullptr, 0);
			if (l > 0)
			{
				auto Str = std::make_unique<wchar_t[]>((size_t)l + 1);
				MultiByteToWideChar(CP_UTF8, 0, (char*)s, -1, Str.get(), l);
				return Str.get();
			}
			return L"";
		}

		String Wide2Utf8(const wchar_t* s)
		{
			int l = WideCharToMultiByte(CP_UTF8, 0, s, -1, nullptr, 0, nullptr, nullptr);
			if (l > 0)
			{
				auto Str = std::make_unique<char[]>((size_t)l + 1);
				WideCharToMultiByte(CP_UTF8, 0, s, -1, Str.get(), l, nullptr, nullptr);
				return Str.get();
			}
			return "";
		}

		String QuoteRemove(const char* s)
		{
			String Ret(s);
			if (Ret.size() > 1) {
				if ((Ret.front() == '"') && (Ret.back() == '"')) {
					if (Ret.size() == 2)
						Ret.erase();
					else 
					{
						Ret.erase(Ret.begin());
						Ret.erase(Ret.end() - 1);
					}
				}
			}
			return Ret;
		}

		WideString QuoteRemove(const wchar_t* s)
		{
			WideString Ret(s);
			if (Ret.size() > 1) {
				if ((Ret.front() == '"') && (Ret.back() == '"')) {
					if (Ret.size() == 2)
						Ret.erase();
					else
					{
						Ret.erase(Ret.begin());
						Ret.erase(Ret.end() - 1);
					}
				}
			}
			return Ret;
		}

		char* StrDub(const char* s)
		{
			auto l = strlen(s);
			return (char*)memcpy((char*)voltek::scalable_alloc(l + 1), s, l + 1);
		}

		void Quit()
		{
			TerminateProcess(GetCurrentProcess(), 0);
		}

		// Author: Nukem9
		uint64_t MurmurHash64A(const void* Key, size_t Len, uint64_t Seed)
		{
			/*-----------------------------------------------------------------------------
			// https://github.com/abrandoned/murmur2/blob/master/MurmurHash2.c#L65
			// MurmurHash2, 64-bit versions, by Austin Appleby
			//
			// The same caveats as 32-bit MurmurHash2 apply here - beware of alignment
			// and endian-ness issues if used across multiple platforms.
			//
			// 64-bit hash for 64-bit platforms
			*/
			const uint64_t m = 0xc6a4a7935bd1e995ull;
			const int r = 47;

			uint64_t h = Seed ^ (Len * m);

			const uint64_t* data = (const uint64_t*)Key;
			const uint64_t* end = data + (Len / 8);

			while (data != end)
			{
				uint64_t k = *data++;

				k *= m;
				k ^= k >> r;
				k *= m;

				h ^= k;
				h *= m;
			}

			const unsigned char* data2 = (const unsigned char*)data;

			switch (Len & 7)
			{
			case 7: h ^= ((uint64_t)data2[6]) << 48;
			case 6: h ^= ((uint64_t)data2[5]) << 40;
			case 5: h ^= ((uint64_t)data2[4]) << 32;
			case 4: h ^= ((uint64_t)data2[3]) << 24;
			case 3: h ^= ((uint64_t)data2[2]) << 16;
			case 2: h ^= ((uint64_t)data2[1]) << 8;
			case 1: h ^= ((uint64_t)data2[0]);
				h *= m;
			}

			h ^= h >> r;
			h *= m;
			h ^= h >> r;

			return h;
		}
	}
}