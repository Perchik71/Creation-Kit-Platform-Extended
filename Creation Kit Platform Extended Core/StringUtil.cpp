// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

namespace CreationKitPlatformExtended
{
	namespace Utils
	{
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

		String GetApplicationPath()
		{
			String Path;
			Path.resize(MAX_PATH);
			Path.resize(GetModuleFileName(GetModuleHandle(NULL), Path.data(), MAX_PATH));
			return DirNameOf(Path);
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
	}
}