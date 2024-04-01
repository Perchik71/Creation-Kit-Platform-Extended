// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "ProfileUtil.h"
#include "StringUtil.h"

namespace CreationKitPlatformExtended
{
	namespace Utils
	{
		bool FileExists(const char* fileName);
		bool FileExists(const wchar_t* fileName);
		bool DirectoryExists(const char* fileName);
		bool DirectoryExists(const wchar_t* fileName);

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
}