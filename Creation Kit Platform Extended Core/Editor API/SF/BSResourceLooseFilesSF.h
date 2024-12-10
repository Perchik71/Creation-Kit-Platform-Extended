// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once
#pragma pack(push, 1)

#include "BSEntryString.h"
#include "..\BSString.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Starfield
		{
			namespace BSResource
			{
				class LooseFileStream
				{
				public:
					virtual LooseFileStream* __dtor__(LooseFileStream* Thisptr, unsigned __int8 flag);
				protected:
					// As a result, I see that the files are limited to 4 GB
					/* 008 */ uint32_t dwFileSize;
					/* 00C */ uint32_t dwUnkC;				// Flags ??
					/* 010 */ BSEntryString* _AppPath;		// init "<DIR FALLOUT 4>"
					/* 018 */ BSEntryString* _DataPath;		// init "/Data"
					/* 020 */ BSEntryString* _FileName;		// for example "Homemaker - Main.ba2"
					/* 028 */ uint32_t BufferSize;
					/* 02C */ uint32_t dwUnk2C;
					/* 030 */ uint32_t dwUnk30;
					/* 034 */ uint32_t dwUnk34;
					/* 038 */ char szFullName[0x100];
					/* 138 */ uint32_t dwUnk138;
					/* 13C */ uint32_t dwUnk13C;
					/* 140 */ uint32_t UsefulDataSize;
					/* 144 */ uint32_t dwUnk144;
					/* 148 */ void* lpData;					// BSResource::LooseFileLocation
					/* 150 */ char szUnk150[0x18];
					///////////////////////////////
					/// SFCK FIXED ADDED
					///////////////////////////////
					/* 178 */ uint64_t dw64FileSize;		// If zero, then the file is not found or inside the archive
					/* 170 */ uint64_t dw64Position;		// while unused
				public:
					static void CreateInstance(const char* FileName, LooseFileStream** Instance);
				public:
					void Initialize64BitData(const char* FileName);
				public:
					inline DWORD GetFileSize() const { return dwFileSize; }
					inline String GetFullName() const { return szFullName; }
					inline BSEntryString* GetAppPath() const { return _AppPath; }
					inline BSEntryString* GetDataPath() const { return _DataPath; }
					inline BSEntryString* GetFileName() const { return _FileName; }
				public:
					READ_PROPERTY(GetFileSize) DWORD FileSize;
					READ_PROPERTY(GetFullName) String FullName;
					READ_PROPERTY(GetAppPath) BSEntryString* AppPath;
					READ_PROPERTY(GetDataPath) BSEntryString* DataPath;
					READ_PROPERTY(GetFileName) BSEntryString* FileName;
				};

				static_assert(sizeof(LooseFileStream) == 0x178, "LooseFileStream class should be the size of 0x178");
			}
		}
	}
}

#pragma pack(pop)