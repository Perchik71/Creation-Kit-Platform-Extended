//////////////////////////////////////////
/*
* Copyright (c) 2020 Nukem9 <email:Nukem@outlook.com>
* Copyright (c) 2020-2022 Perchik71 <email:perchik71@outlook.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this
* software and associated documentation files (the "Software"), to deal in the Software
* without restriction, including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

#pragma once
#pragma pack(push, 4)

#include "..\BSString.h"
#include "..\BSTArray.h"
#include "..\BSTList.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			struct TESChunk
			{
				char type[4];
				uint32_t size;
				uint32_t flags;
				uint32_t identifier;
				uint16_t timestamp;
				uint16_t vercontrol;
				uint16_t internalver;
				uint16_t unk;
			};

			class TESFile {
			private:
				TESFile() = default;
				~TESFile() = default;
			public:
				enum : uint32_t {
					FILE_RECORD_ESM = 0x1,			// Master plugin
					FILE_RECORD_CHECKED = 0x4,		// Pending load/loaded
					FILE_RECORD_ACTIVE = 0x8,		// Save target
					FILE_RECORD_LOCALIZED = 0x80,	// Strings removed
					FILE_RECORD_ESL = 0x200,		// Small file
				};
			private:
				/* 000 */ char _pad0[0x78];
				/* 078 */ char m_FileName[MAX_PATH];
				/* 17C */ char m_FilePath[MAX_PATH];
				/* 280 */ void* m_UnkPtr280;
				/* 288 */ uint32_t m_bufferSize;					// buffer size used when opening BSFile. init to 0x800
				/* 28C */ char _pad1[0x38];
				/* 2C4 */ uint32_t m_fileSize;						// same as FileSizeLow in find data
				/* 2C8 */ uint32_t m_currentRecordOffset;			// offset of current record in file
				/* 2CC */ uint32_t m_currentChunkOffset;			// offset of current chunk in record
				/* 2D0 */ uint32_t m_fetchedChunkDataSize;			// number of bytes read in last GetChunkData() call
				/* 2D4 */ char _pad2[0x34];
				/* 308 */ bool m_headerRead;					// set after header has been successfully parsed

				struct WIN32_FIND_DATA_UNKNOWN {
					char _pad0[0xC];
					FILETIME ftCreationTime;
					FILETIME ftLastWriteTime;
					uint32_t nFileSizeHigh;
					uint32_t nFileSizeLow;
				};

				/* 30C */ WIN32_FIND_DATA_UNKNOWN m_findData;

				struct FileHeaderInfo {
					/*00*/ float fileVersion;
					/*04*/ uint32_t numRecords;		// number of record blocks in file
					/*08*/ uint32_t nextFormID;		// including file index in highest byte
				};

				/* 330 */ FileHeaderInfo m_fileHeader;
				/* 33C */ uint32_t m_Flags;

				// Data for Depend files - compared against size in findData of masters to check if they have changed since last edit
				// 08
				struct DependFileData {
					/*00*/ uint32_t nFileSizeHigh;
					/*04*/ uint32_t nFileSizeLow;
				};
				typedef BSSimpleList<DependFileData*> DependDataList;
				typedef BSSimpleList<const char*> DependNameList;

				/* 340 */ DependNameList m_dependNames;
				/* 350 */ DependDataList m_dependData;
				/* 360 */ uint32_t m_dependCount;
				/* 364 */ char _pad3[0x4];
				/* 368 */ TESFile** m_dependArray;
				/* 370 */ char _pad4[0x8];
				/* 378 */ char m_fileIndex;						// index of this file in load order (or 0xFF if not loaded)
				/* 379 */ char _pad5[0x7];
				/* 380 */ BSString m_authorName;
				/* 390 */ BSString m_description;
				/* 3A0 */ char _pad6[0x6E0];
			public:
				BSString GetAuthorName() const;
				inline BSString GetDescription() const { return *m_description ? m_description : ""; }
				inline BSString GetFileName() const { return m_FileName ? m_FileName : ""; }
				inline BSString GetFilePath() const { return m_FilePath ? m_FilePath : ""; }
				inline char GetFileLoadIndex() const { return m_fileIndex; }
				inline TESFile** GetDependArray() const { return m_dependArray; }
				inline uint32_t GetDependCount() const { return m_dependCount; }
				inline uint32_t GetFileSize() const { return m_fileSize; }
				inline uint32_t GetFlags() const { return m_Flags; }
				inline FileHeaderInfo GetHeaderInfo() const { return m_fileHeader; }
				inline bool IsLoaded() const { return m_fileIndex != 0xFF; }
				inline bool IsMaster() const { return m_Flags & FILE_RECORD_ESM; }
				inline bool IsSelected() const { return m_Flags & FILE_RECORD_CHECKED; }
				inline bool IsActive() const { return m_Flags & FILE_RECORD_ACTIVE; }
				inline bool IsLocalized() const { return m_Flags & FILE_RECORD_LOCALIZED; }
				inline bool IsSmallMaster() const { return m_Flags & FILE_RECORD_ESL; }
				SYSTEMTIME GetCreationTime() const;
				SYSTEMTIME GetLastWriteTime() const;
				static bool ReadFirstChunk(const char* fileName, TESChunk& chunk);
				static uint32_t GetTypeFile(const char* fileName);
			public:
				inline static int32_t(*LoadTESInfo)(TESFile*);
				inline static int64_t(*WriteTESInfo)(TESFile*);
				int32_t hk_LoadTESInfo();
				int64_t hk_WriteTESInfo();
				bool IsMasterFileToBlacklist();
				void Dump();
			public:
				inline static bool AllowSaveESM;
				inline static bool AllowMasterESP;
			public:
				READ_PROPERTY(GetAuthorName) BSString AuthorName;
				READ_PROPERTY(GetDescription) BSString Description;
				READ_PROPERTY(GetFileName) BSString FileName;
				READ_PROPERTY(GetFilePath) BSString FilePath;
				READ_PROPERTY(GetFlags) uint32_t Flags;
				READ_PROPERTY(GetFileLoadIndex) char FileLoadIndex;
				READ_PROPERTY(GetDependArray) TESFile** DependArray;
				READ_PROPERTY(GetDependCount) uint32_t DependCount;
				READ_PROPERTY(GetFileSize) uint32_t FileSize;
				READ_PROPERTY(GetCreationTime) SYSTEMTIME CreationTime;
				READ_PROPERTY(GetLastWriteTime) SYSTEMTIME LastWriteTime;
			};

			static_assert(sizeof(TESFile) == 0xA80, "TESFile class should be the size of 0xA80");
		}
	}
}

#pragma pack(pop)