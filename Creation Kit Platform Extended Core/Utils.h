// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace Utils
	{
		void __Assert(LPCSTR File, int Line, LPCSTR Format, ...);
		void __Assert(LPCWSTR File, int Line, LPCWSTR Format, ...);
		uintptr_t FindPattern(uintptr_t StartAddress, uintptr_t MaxSize, const char* Mask);
		Array<uintptr_t> FindPatterns(uintptr_t StartAddress, uintptr_t MaxSize, const char* Mask);
		bool GetPESectionRange(uintptr_t ModuleBase, const char* Section, uintptr_t* Start, uintptr_t* End);
		void PatchMemory(uintptr_t Address, uint8_t* Data, size_t Size);
		void PatchMemory(uintptr_t Address, std::initializer_list<uint8_t> Data);
		void PatchMemoryNop(uintptr_t Address, size_t Size);
		void PatchMemoryWP(uintptr_t Address, uint8_t* Data, size_t Size);
		void PatchMemoryWP(uintptr_t Address, std::initializer_list<uint8_t> Data);
		void PatchMemoryNopWP(uintptr_t Address, size_t Size);
		DWORD UnlockWP(uintptr_t Address, size_t Size);
		void LockWP(uintptr_t Address, size_t Size, DWORD OldFlags);
		void SetThreadName(uint32_t ThreadID, LPCSTR ThreadName);

		// https://docs.microsoft.com/en-us/windows/win32/debug/retrieving-the-last-error-code
		String GetLastErrorToStr(DWORD err, const String& namefunc);
		String GetLastErrorToStr(const String& namefunc);

		// DirNameOf https://stackoverflow.com/questions/8518743/get-directory-from-file-path-c/34740989
		static inline String DirNameOf(const String& path) {
			size_t pos = path.find_last_of("\\/");
			return (std::string::npos == pos)
				? ""
				: path.substr(0, pos + 1);
		}

		bool FileExists(const char* fileName);
		bool FileExists(const wchar_t* fileName);
		bool DirectoryExists(const char* fileName);
		bool DirectoryExists(const wchar_t* fileName);

		uint64_t GetTotalPhysicalMemory();
		uint64_t GetTotalPageFileMemory();
		uint64_t GetAvailableTotalPhysicalMemory();
		uint64_t GetAvailableTotalPageFileMemory();

		String Wide2Ansi(const wchar_t* s);
		WideString Ansi2Wide(const char* s);
		WideString Utf82Wide(const char8_t* s);
		String Wide2Utf8(const wchar_t* s);

		String QuoteRemove(const char* s);
		WideString QuoteRemove(const wchar_t* s);

		String GetApplicationPath();

		void Quit();

		bool GetProfileValue(const char* relfilename, const char* section, const char* option, bool defvalue);
		int GetProfileValue(const char* relfilename, const char* section, const char* option, int defvalue);
		float GetProfileValue(const char* relfilename, const char* section, const char* option, float defvalue);
		String GetProfileValue(const char* relfilename, const char* section, const char* option, const char* defvalue);
		void UpdateProfileValue(const char* relfilename, const char* section, const char* option, bool value);
		void UpdateProfileValue(const char* relfilename, const char* section, const char* option, int value);
		void UpdateProfileValue(const char* relfilename, const char* section, const char* option, float value);
		void UpdateProfileValue(const char* relfilename, const char* section, const char* option, const char* value);

		template<typename T>
		inline bool FileReadBuffer(FILE* fileStream, T* nValue, uint32_t nCount = 1)
		{
			return fread(nValue, sizeof(T), nCount, fileStream) == nCount;
		}

		template<typename T>
		inline bool FileWriteBuffer(FILE* fileStream, T nValue, uint32_t nCount = 1)
		{
			return fwrite(&nValue, sizeof(T), nCount, fileStream) == nCount;
		}

		inline bool FileReadString(FILE* fileStream, String& sStr)
		{
			uint16_t nLen = 0;
			if (fread(&nLen, sizeof(uint16_t), 1, fileStream) != 1)
				return false;

			sStr.resize((size_t)nLen + 1);
			sStr[nLen] = '\0';

			return fread(sStr.data(), 1, nLen, fileStream) == nLen;
		}

		inline bool FileWriteString(FILE* fileStream, const String& sStr)
		{
			uint16_t nLen = (uint16_t)sStr.length();
			if (fwrite(&nLen, sizeof(uint16_t), 1, fileStream) != 1)
				return false;

			return fwrite(sStr.c_str(), 1, nLen, fileStream) == nLen;
		}

		static const char* whitespaceDelimiters = " \t\n\r\f\v";

		inline String& Trim(String& str) {

			str.erase(str.find_last_not_of(whitespaceDelimiters) + 1);
			str.erase(0, str.find_first_not_of(whitespaceDelimiters));

			return str;
		}

		inline String Trim(const char* s) {
			String str(s);
			return Trim(str);
		}

		inline void DetourJump(uintptr_t Target, uintptr_t Destination) {
			Detours::X64::DetourFunction(Target, Destination, Detours::X64Option::USE_REL32_JUMP);
		}

		inline void DetourCall(uintptr_t Target, uintptr_t Destination) {
			Detours::X64::DetourFunction(Target, Destination, Detours::X64Option::USE_REL32_CALL);
		}

		char* StrDub(const char* s);
		uint64_t MurmurHash64A(const void* Key, size_t Len, uint64_t Seed = 0);

		class ScopeFileStream
		{
		public:
			inline ScopeFileStream(FILE* fileStream) : _fileStream(fileStream) {}
			inline ~ScopeFileStream() { if (_fileStream) fclose(_fileStream); }
		private:
			ScopeFileStream(const ScopeFileStream&) = default;
			ScopeFileStream& operator=(const ScopeFileStream&) = default;

			FILE* _fileStream;
		};
	}

	// thread-safe template versions of thisVirtualCall()

	template<typename TR>
	__forceinline TR thisVirtualCall(size_t reloff, const void* ths) {
		return (*(TR(__fastcall**)(const void*))(*(__int64*)ths + reloff))(ths);
	}

	template<typename TR, typename T1>
	__forceinline TR thisVirtualCall(size_t reloff, const void* ths, T1 a1) {
		return (*(TR(__fastcall**)(const void*, T1))(*(__int64*)ths + reloff))(ths, a1);
	}

	template<typename TR, typename T1, typename T2>
	__forceinline TR thisVirtualCall(size_t reloff, const void* ths, T1 a1, T2 a2) {
		return (*(TR(__fastcall**)(const void*, T1, T2))(*(__int64*)ths + reloff))(ths, a1, a2);
	}

	template<typename TR, typename T1, typename T2, typename T3>
	__forceinline TR thisVirtualCall(size_t reloff, const void* ths, T1 a1, T2 a2, T3 a3) {
		return (*(TR(__fastcall**)(const void*, T1, T2, T3))(*(__int64*)ths + reloff))(ths, a1, a2, a3);
	}

	template<typename TR, typename T1, typename T2, typename T3, typename T4>
	__forceinline TR thisVirtualCall(size_t reloff, const void* ths, T1 a1, T2 a2, T3 a3, T4 a4) {
		return (*(TR(__fastcall**)(const void*, T1, T2, T3, T4))(*(__int64*)ths + reloff))(ths, a1, a2, a3, a4);
	}

	template <size_t Offset, size_t RequiredOffset>
	struct __declspec(empty_bases) CheckOffset
	{
		static_assert(Offset <= RequiredOffset, "Offset is larger than expected");
		static_assert(Offset >= RequiredOffset, "Offset is smaller than expected");
	};
}

#ifdef _UNICODE
#	define __VSTR(x) TEXT(x)
#else
#	define __VSTR(x) x
#endif

#define PROPERTY(read_func, write_func)	__declspec(property(get = read_func, put = write_func))
#define READ_PROPERTY(read_func)		__declspec(property(get = read_func))
#define READ_PROPERTY2(type, Name, Var) \
	__forceinline type Get##Name(VOID) const { return Var; } \
	__declspec(property(get = Get##Name)) type Name

#define Assert(Cond)					if(!(Cond)) CreationKitPlatformExtended::Utils::__Assert(__FILE__, __LINE__, #Cond);
#define AssertMsgVa(Cond, Msg, ...)		if(!(Cond)) CreationKitPlatformExtended::Utils::__Assert(__FILE__, __LINE__, "%s\n\n" Msg, #Cond, ##__VA_ARGS__);
#define AssertMsg(Cond, Msg)			AssertMsgVa(Cond, Msg)

#define static_assert_offset(Structure, Member, Offset) struct __declspec(empty_bases) : CreationKitPlatformExtended::CheckOffset<offsetof(Structure, Member), Offset> { }
#define PatchIAT(detour, module, procname) Detours::IATHook(GlobalEnginePtr->GetModuleBase(), (module), (procname), (uintptr_t)(detour));

#define DECLARE_CONSTRUCTOR_HOOK(Class) \
	static Class *__ctor__(void *Instance) \
	{ \
		return new (Instance) Class(); \
	} \
	\
	static Class *__dtor__(Class *Thisptr, unsigned __int8) \
	{ \
		Thisptr->~Class(); \
		return Thisptr; \
	}