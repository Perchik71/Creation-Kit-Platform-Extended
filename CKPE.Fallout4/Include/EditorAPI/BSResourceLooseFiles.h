// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BSEntryString.h"
#include "BSString.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace BSResource
			{
				class LooseFileStream
				{
				public:
					virtual LooseFileStream* __dtor__(LooseFileStream* Thisptr, std::uint8_t flag);
				protected:
					// As a result, I see that the files are limited to 4 GB
					/* 008 */ std::uint32_t dwFileSize;
					/* 00C */ std::uint32_t dwUnkC;			// Flags ??
					/* 010 */ BSEntryString* _AppPath;		// init "<DIR FALLOUT 4>"
					/* 018 */ BSEntryString* _DataPath;		// init "/Data"
					/* 020 */ BSEntryString* _FileName;		// for example "Homemaker - Main.ba2"
					/* 028 */ std::uint32_t BufferSize;
					/* 02C */ std::uint32_t dwUnk2C;
					/* 030 */ std::uint32_t dwUnk30;
					/* 034 */ std::uint32_t dwUnk34;
					/* 038 */ char szFullName[0x100];
					/* 138 */ std::uint32_t dwUnk138;
					/* 13C */ std::uint32_t dwUnk13C;
					/* 140 */ std::uint32_t dwUnk140;
					/* 144 */ std::uint32_t dwUnk144;
					/* 148 */ void* lpData;					// BSResource::LooseFileLocation
					/* 150 */ std::uint64_t UsefulDataSize;
					/* 158 */ char szUnk150[0x8];
					///////////////////////////////
					/// F4CK FIXED ADDED
					///////////////////////////////
					/* 160 */ char szReserved[0x10];
					/* 170 */ std::uint64_t dw64FileSize;	// If zero, then the file is not found or inside the archive
					/* 178 */ std::uint64_t dw64Position;	// while unused
				public:
					static void CreateInstance(const char* FileName, LooseFileStream** Instance) noexcept(true);
				public:
					void Initialize64BitData(const char* FileName) noexcept(true);
				public:
					inline std::uint32_t GetFileSize() const noexcept(true) { return dwFileSize; }
					inline const char* GetFullName() const noexcept(true) { return szFullName; }
					inline const BSEntryString* GetAppPath() const noexcept(true) { return _AppPath; }
					inline const BSEntryString* GetDataPath() const noexcept(true) { return _DataPath; }
					inline const BSEntryString* GetFileName() const noexcept(true) { return _FileName; }
				public:
					CKPE_READ_PROPERTY(GetFileSize) std::uint32_t FileSize;
					CKPE_READ_PROPERTY(GetFullName) const char* FullName;
					CKPE_READ_PROPERTY(GetAppPath) const BSEntryString* AppPath;
					CKPE_READ_PROPERTY(GetDataPath) const BSEntryString* DataPath;
					CKPE_READ_PROPERTY(GetFileName) const BSEntryString* FileName;
				};

				static_assert(sizeof(LooseFileStream) == 0x180, "LooseFileStream class should be the size of 0x180");
			}
		}
	}
}