//////////////////////////////////////////
/*
* Copyright (c) 2020 Nukem9 <email:Nukem@outlook.com>
* Copyright (c) 2022-2023 Perchik71 <email:perchik71@outlook.com>
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

#include "..\BSTList.h"
#include "..\BSString.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
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

			class TESFile
			{
			private:
				TESFile() = default;
				~TESFile() = default;

			public:
				enum : uint32_t
				{
					FILE_RECORD_ESM = 0x1,			// Master plugin
					FILE_RECORD_CHECKED = 0x4,		// Pending load/loaded
					FILE_RECORD_ACTIVE = 0x8,		// Save target
					FILE_RECORD_LOCALIZED = 0x80,	// Strings removed
					FILE_RECORD_ESL = 0x200,		// Small file
				};
			private:
				/* 000 */ char _pad0[0x58];
				/* 058 */ char m_FileName[MAX_PATH];
				/* 15C */ char m_FilePath[MAX_PATH];
				/* 260 */ char _pad1[0x8];
				/* 268 */ uint32_t m_bufsize;
				/* 26C */ char _pad2[0x38];
				/* 2A4 */ uint32_t m_fileSize;
				/* 2A8 */ char _pad3[0x48];

				struct WIN32_FIND_DATA_UNKNOWN 
				{
					FILETIME ftCreationTime;
					char _pad0[0x8];
					FILETIME ftLastWriteTime;
					uint32_t nFileSizeHigh;
					uint32_t nFileSizeLow;
				};

				/* 2F0 */ WIN32_FIND_DATA_UNKNOWN m_findData;
				/* 310 */ char _pad4[0x11C];

				struct FileHeaderInfo 
				{
					FLOAT fileVersion;
					uint32_t numRecords;		// number of record blocks in file
					uint32_t nextFormID;		// including file index in highest byte
				};

				/* 42C */ FileHeaderInfo m_fileHeader;
				/* 438 */ uint32_t m_RecordFlags;

				// Data for Depend files - compared against size in findData of masters to check if they have changed since last edit
				// 08
				struct DependFileData 
				{
					/*00*/ uint32_t nFileSizeHigh;
					/*04*/ uint32_t nFileSizeLow;
				};
				typedef BSSimpleList<DependFileData*> DependDataList;
				typedef BSSimpleList<LPCSTR> DependNameList;

				/* 43C */ char _pad5[0x4];
				/* 440 */ DependNameList m_dependNames;
				/* 450 */ DependDataList m_dependData;
				/* 460 */ uint32_t m_dependCount;
				/* 464 */ char _pad6[0x4];
				/* 468 */ TESFile** m_dependArray;
				/* 470 */ char _pad7[0x8];
				/* 478 */ char m_fileIndex;						// index of this file in load order (or 0xFF if not loaded)
				/* 479 */ char _pad8[0x7];
				/* 480 */ BSString m_authorName;
				/* 490 */ BSString m_description;
				/* 4A0 */ char _pad9[0x20];
				/* 4C0 */ TESFile* m_parentFile;
				/* 4C8 */ char _pad10[0x8];
				/* 4D0 */ uint32_t* m_ownedIds;
				/* 4D8 */ uint64_t m_countOwned;
			public:
				inline BSString GetAuthorName() const { return *m_authorName ? m_authorName : "Bethesda Software"; }
				inline BSString GetDescription() const { return *m_description ? m_description : ""; }
				inline BSString GetFileName() const { return m_FileName ? m_FileName : ""; }
				inline BSString GetFilePath() const { return m_FilePath ? m_FilePath : ""; }
				inline CHAR GetFileLoadIndex() const { return m_fileIndex; }
				inline TESFile** GetDependArray() const { return m_dependArray; }
				inline DWORD GetDependCount() const { return m_dependCount; }
				inline DWORD GetFileSize() const { return m_fileSize; }
				inline FileHeaderInfo GetHeaderInfo() const { return m_fileHeader; }
				inline bool IsLoaded() const { return m_fileIndex != 0xFF; }
				inline bool IsMaster() const { return m_RecordFlags & FILE_RECORD_ESM; }
				inline bool IsSelected() const { return m_RecordFlags & FILE_RECORD_CHECKED; }
				inline bool IsActive() const { return m_RecordFlags & FILE_RECORD_ACTIVE; }
				inline bool IsLocalized() const { return m_RecordFlags & FILE_RECORD_LOCALIZED; }
				inline bool IsSmallMaster() const { return m_RecordFlags & FILE_RECORD_ESL; }
				inline TESFile* GetParentFile() { return m_parentFile; }
				inline void IncCountOwnedIds() { m_countOwned++; }
				inline uint64_t CountOwnedIds() const { return m_countOwned; }
				inline void CleanCountOwnedIds() { m_countOwned = 0; }
				inline uint32_t* GetArrayOwnedIds() const { return m_ownedIds; }
				inline void SetArrayOwnedIds(uint32_t* FormIds, uint32_t Count) { m_ownedIds = FormIds; m_countOwned = Count; }
				SYSTEMTIME GetCreationTime() const;
				SYSTEMTIME GetLastWriteTime() const;

				inline static int (*LoadTESInfo)(TESFile*);
				inline static __int64 (*WriteTESInfo)(TESFile*);
				inline static bool AllowSaveESM;
				inline static bool AllowMasterESP;

				int hk_LoadTESInfo();
				__int64 hk_WriteTESInfo();
				bool IsActiveFileBlacklist();

				uint32_t GetIndexLoader() const;
			public:
				static bool ReadFirstChunk(const char* fileName, TESChunk& chunk);
				static uint32_t GetTypeFile(const char* fileName);
			};
			static_assert(sizeof(TESFile) == 0x4E0);
		}
	}
}