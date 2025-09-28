// Copyright © 2022-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <windows.h>
#include "BSTList.h"
#include "BSString.h"
#include "BSSystemFile.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			struct TESChunk
			{
				char type[4];
				std::uint32_t size;
				std::uint32_t flags;
				std::uint32_t identifier;
				std::uint16_t timestamp;
				std::uint16_t vercontrol;
				std::uint16_t internalver;
				std::uint16_t unk;
			};

			class TESFile
			{
			private:
				TESFile() = default;
				~TESFile() = default;

			public:
				enum : std::uint32_t
				{
					FILE_RECORD_ESM			= 0x1,		// Master plugin
					FILE_RECORD_CHECKED		= 0x4,		// Pending load/loaded
					FILE_RECORD_ACTIVE		= 0x8,		// Save target
					FILE_RECORD_LOCALIZED	= 0x80,		// Strings removed
					FILE_RECORD_ESL			= 0x200,	// Small file
				};
			private:
				/* 000 */ char _pad0[0x78];
				/* 078 */ char m_FileName[MAX_PATH];
				/* 17C */ char m_FilePath[MAX_PATH];
				/* 280 */ char _pad1[0x8];
				/* 288 */ std::uint32_t m_bufsize;
				/* 28C */ char _pad2[0x18];
				/* 2A4 */ TESChunk m_header_chunk;						// should be 'TES4'
				/* 2BC */ std::uint32_t m_header_type;					// should be 'HEDR'
				/* 2C0 */ std::uint32_t m_size_chunk_may_unused;		
				/* 2C4 */ std::uint32_t m_fileSize;
				/* 2C8 */ std::uint32_t m_currentRecordOffset;			// offset of current record in file
				/* 2CC */ std::uint32_t m_currentChunkOffset;			// offset of current chunk in record
				/* 2D0 */ std::uint32_t m_fetchedChunkDataSize;			// number of bytes read in last GetChunkData() call
				/* 2D4 */ char _pad3[0x3C];

				BSSystemFile::Info m_findData;

				struct FileHeaderInfo 
				{
					float fileVersion;
					std::uint32_t numRecords;		// number of record blocks in file
					std::uint32_t nextFormID;		// including file index in highest byte
				};

				/* 330 */ FileHeaderInfo m_fileHeader;
				/* 33C */ std::uint32_t m_RecordFlags;

				// Data for Depend files - compared against size in findData of masters to check if they have changed since last edit
				// 08
				struct DependFileData 
				{
					/*00*/ std::uint32_t nFileSizeHigh;
					/*04*/ std::uint32_t nFileSizeLow;
				};
				typedef BSSimpleList<DependFileData*> DependDataList;
				typedef BSSimpleList<const char*> DependNameList;

				/* 340 */ DependNameList m_dependNames;
				/* 350 */ DependDataList m_dependData;
				/* 360 */ std::uint32_t m_dependCount;
				/* 364 */ char _pad6[0x4];
				/* 368 */ TESFile** m_dependArray;
				/* 370 */ char _pad7[0x8];
				/* 378 */ std::uint8_t m_fileIndex;			// index of this file in load order (or 0xFF if not loaded)
				/* 379 */ char _pad8[0x7];
				/* 380 */ BSString m_authorName;
				/* 390 */ BSString m_description;
				/* 3A0 */ char _pad9[0x20];
				/* 3C0 */ TESFile* m_parentFile;
				/* 3C8 */ char _pad10[0x8];
				/* 3D0 */ std::uint32_t* m_ownedIds;
				/* 3D8 */ std::uint64_t m_countOwned;
			public:
				[[nodiscard]] inline BSString GetAuthorName() const noexcept(true) { return *m_authorName ? m_authorName : "DEFAULT"; }
				[[nodiscard]] inline BSString GetDescription() const noexcept(true) { return m_description; }
				[[nodiscard]] inline BSString GetFileName() const noexcept(true) { return m_FileName ? m_FileName : ""; }
				[[nodiscard]] inline BSString GetFilePath() const noexcept(true) { return m_FilePath ? m_FilePath : ""; }
				[[nodiscard]] inline std::uint8_t GetFileLoadIndex() const noexcept(true) { return m_fileIndex; }
				[[nodiscard]] inline TESFile** GetDependArray() const noexcept(true) { return m_dependArray; }
				[[nodiscard]] inline std::uint32_t GetDependCount() const noexcept(true) { return m_dependCount; }
				[[nodiscard]] inline std::uint32_t GetFileSize() const noexcept(true) { return m_fileSize; }
				[[nodiscard]] inline FileHeaderInfo GetHeaderInfo() const noexcept(true) { return m_fileHeader; }
				[[nodiscard]] inline bool IsLoaded() const noexcept(true) { return m_fileIndex != 0xFF; }
				[[nodiscard]] inline bool IsMaster() const noexcept(true) { return m_RecordFlags & FILE_RECORD_ESM; }
				[[nodiscard]] inline bool IsSelected() const noexcept(true) { return m_RecordFlags & FILE_RECORD_CHECKED; }
				[[nodiscard]] inline bool IsActive() const noexcept(true) { return m_RecordFlags & FILE_RECORD_ACTIVE; }
				[[nodiscard]] inline bool IsLocalized() const noexcept(true) { return m_RecordFlags & FILE_RECORD_LOCALIZED; }
				[[nodiscard]] inline bool IsSmallMaster() const noexcept(true) { return m_RecordFlags & FILE_RECORD_ESL; }
				[[nodiscard]] inline TESFile* GetParentFile() noexcept(true) { return m_parentFile; }
				inline void IncCountOwnedIds() noexcept(true) { m_countOwned++; }
				[[nodiscard]] inline std::uint64_t CountOwnedIds() const noexcept(true) { return m_countOwned; }
				inline void CleanCountOwnedIds() noexcept(true) { m_countOwned = 0; }
				[[nodiscard]] inline std::uint32_t* GetArrayOwnedIds() const noexcept(true) { return m_ownedIds; }
				inline void SetArrayOwnedIds(std::uint32_t* FormIds, std::uint32_t Count) noexcept(true) { m_ownedIds = FormIds; m_countOwned = Count; }
				[[nodiscard]] SYSTEMTIME GetCreateTime() const noexcept(true);
				[[nodiscard]] SYSTEMTIME GetModifyTime() const noexcept(true);

				inline static int (*LoadTESInfo)(TESFile*);
				inline static __int64 (*WriteTESInfo)(TESFile*);
				inline static bool AllowSaveESM;
				inline static bool AllowMasterESP;

				int hk_LoadTESInfo();
				__int64 hk_WriteTESInfo();
				bool IsActiveFileBlacklist(bool showDialog = true) const noexcept(true);
			public:
				static bool ReadFirstChunk(const char* fileName, TESChunk& chunk) noexcept(true);
				static std::uint32_t GetTypeFile(const char* fileName) noexcept(true);
			};
			static_assert(sizeof(TESFile) == 0x3E0);
		}
	}
}