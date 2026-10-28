// Copyright © 2022-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/IDs.h>
#include <cstdint>
#include <windows.h>
#include <CKPE.Common.Relocation.h>
#include <EditorAPI/B/BSTList.h>
#include <EditorAPI/B/BSString.h>
#include <EditorAPI/B/BSSystemFile.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/B/BSTHashMap.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			struct TESGroupHeader
			{
				char type[4];
				std::uint32_t size;
				std::uint32_t label;
				std::int32_t groupType;
				char pad10[0x8];
			};
			static_assert(sizeof(TESGroupHeader) == 0x18);
			static_assert(offsetof(TESGroupHeader, groupType) == 0xC);

			struct TESFileGroup
			{
				TESGroupHeader header;
				std::uint8_t unk18[8];
			};
			static_assert(sizeof(TESFileGroup) == 0x20);

			class TESFile;

			// Both .fud ownership and .fvd version files use this layout.
			struct TESBitArrayFile
			{
				/* 000 */ TESFile* File;
				/* 008 */ char FileName[MAX_PATH];
				/* 10C */ char pad10C[0x4];
				/* 110 */ HANDLE FileHandle;
				/* 118 */ HANDLE MappingHandle;
				/* 120 */ std::uint8_t* Data;
				/* 128 */ std::uint32_t DataSize;
				/* 12C */ bool Detached;
				/* 12D */ char pad12D[0x3];

				[[nodiscard]] bool GetBit(std::uint32_t formID) const noexcept(true)
				{
					using func_t = bool (*)(const TESBitArrayFile*, std::uint32_t);
					static Common::Relocation<func_t> func{
						EditorAPI::IDs::TESBitArrayFile::GetBit };
					return func(this, formID);
				}

				void SetBit(std::uint32_t formID, bool value) noexcept(true)
				{
					using func_t = void (*)(TESBitArrayFile*, std::uint32_t, bool);
					static Common::Relocation<func_t> func{ EditorAPI::IDs::TESBitArrayFile::SetBit };
					func(this, formID, value);
				}

				[[nodiscard]] std::uint8_t GetByte(std::uint32_t formID) const noexcept(true)
				{
					using func_t = std::uint8_t (*)(const TESBitArrayFile*, std::uint32_t);
					static Common::Relocation<func_t> func{
						EditorAPI::IDs::TESBitArrayFile::GetByte };
					return func(this, formID);
				}

				void SetByte(std::uint32_t formID, std::uint8_t value) noexcept(true)
				{
					using func_t = void (*)(TESBitArrayFile*, std::uint32_t, std::uint8_t);
					static Common::Relocation<func_t> func{
						EditorAPI::IDs::TESBitArrayFile::SetByte };
					func(this, formID, value);
				}

				// Even form IDs occupy the high nibble.
				[[nodiscard]] std::uint8_t GetNibble(std::uint32_t formID) const noexcept(true)
				{
					using func_t = std::uint8_t (*)(const TESBitArrayFile*, std::uint32_t);
					static Common::Relocation<func_t> func{
						EditorAPI::IDs::TESBitArrayFile::GetNibble };
					return func(this, formID);
				}

				void SetNibble(std::uint32_t formID, std::uint8_t value) noexcept(true)
				{
					using func_t = void (*)(TESBitArrayFile*, std::uint32_t, std::uint8_t);
					static Common::Relocation<func_t> func{
						EditorAPI::IDs::TESBitArrayFile::SetNibble };
					func(this, formID, value);
				}
			};
			static_assert(sizeof(TESBitArrayFile) == 0x130);
			static_assert(offsetof(TESBitArrayFile, FileHandle) == 0x110);
			static_assert(offsetof(TESBitArrayFile, Data) == 0x120);
			static_assert(offsetof(TESBitArrayFile, Detached) == 0x12C);

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
			static_assert(sizeof(TESChunk) == 0x18);
			static_assert(offsetof(TESChunk, identifier) == 0xC);

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
				/* 000 */ std::uint32_t m_lastError;
				/* 004 */ char pad004[4];
				/* 008 */ TESFile* m_threadSafeParent;
				/* 010 */ BSTHashMap<std::uint32_t, TESFile*> m_threadSafeFiles;
				/* 048 */ void* unk048;
				/* 050 */ class BSFile* m_temporaryFile;
				/* 058 */ BSFile* m_file;
				/* 060 */ TESBitArrayFile* m_formUserData;
				/* 068 */ TESBitArrayFile* m_formVersionData;
				/* 070 */ TESBitArrayFile* m_formIDData;
				/* 078 */ char m_FileName[MAX_PATH];
				/* 17C */ char m_FilePath[MAX_PATH];
				/* 280 */ char* m_buffer;
				/* 288 */ std::uint32_t m_bufsize;
				/* 28C */ std::uint32_t unk28C;
				/* 290 */ std::uint32_t unk290;
				/* 294 */ char pad294[4];
				/* 298 */ void* unk298;
				/* 2A0 */ std::uint32_t unk2A0;
				/* 2A4 */ TESChunk m_header_chunk;						// should be 'TES4'
				/* 2BC */ std::uint32_t m_header_type;					// should be 'HEDR'
				/* 2C0 */ std::uint32_t m_size_chunk_may_unused;		
				/* 2C4 */ std::uint32_t m_fileSize;
				/* 2C8 */ std::uint32_t m_currentRecordOffset;			// offset of current record in file
				/* 2CC */ std::uint32_t m_currentChunkOffset;			// offset of current chunk in record
				/* 2D0 */ std::uint32_t m_fetchedChunkDataSize;			// number of bytes read in last GetChunkData() call
				/* 2D4 */ TESChunk m_savedRecord;
				/* 2EC */ std::uint32_t m_savedRecordOffset;
				/* 2F0 */ std::uint32_t m_savedChunkOffset;
				/* 2F4 */ char pad2F4[4];
				/* 2F8 */ BSSimpleList<TESFileGroup> m_groups;
				/* 308 */ bool unk308;
				/* 309 */ char pad309[7];

				/* 310 */ BSSystemFile::Info m_findData;

				struct FileHeaderInfo 
				{
					float fileVersion;
					std::uint32_t numRecords;		// number of record blocks in file
					std::uint32_t nextFormID;		// including file index in highest byte
				};
				static_assert(sizeof(FileHeaderInfo) == 0xC);
				static_assert(offsetof(FileHeaderInfo, nextFormID) == 0x8);

				/* 330 */ FileHeaderInfo m_fileHeader;
				/* 33C */ std::uint32_t m_RecordFlags;

				// Data for Depend files - compared against size in findData of masters to check if they have changed since last edit
				// 08
				struct DependFileData 
				{
					/*00*/ std::uint32_t nFileSizeHigh;
					/*04*/ std::uint32_t nFileSizeLow;
				};
				static_assert(sizeof(DependFileData) == 0x8);
				static_assert(offsetof(DependFileData, nFileSizeLow) == 0x4);
				typedef BSSimpleList<DependFileData> DependDataList;
				typedef BSSimpleList<const char> DependNameList;
				static_assert(sizeof(DependDataList) == 0x10);
				static_assert(sizeof(DependNameList) == 0x10);

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
				/* 3A0 */ char* m_decompressedRecord;
				/* 3A8 */ std::uint32_t m_decompressedRecordSize;
				/* 3AC */ char pad3AC[4];
				/* 3B0 */ void* m_reservedDecompressionBuffer;
				/* 3B8 */ std::uint32_t m_reservedDecompressionBufferSize;
				/* 3BC */ char pad3BC[4];
				/* 3C0 */ TESFile* m_parentFile;
				/* 3C8 */ std::uint32_t m_versionControlID;
				/* 3CC */ char pad3CC[4];
				/* 3D0 */ std::uint32_t* m_ownedIds;
				/* 3D8 */ std::uint32_t m_countOwned;
				/* 3DC */ char pad3DC[4];
				/* 3E0 */ BSTArray<std::uint32_t>* m_transientFormIDs[0x9F];

				static void CheckLayout() noexcept(true)
				{
					static_assert(offsetof(TESFile, m_file) == 0x58);
					static_assert(offsetof(TESFile, m_threadSafeFiles) == 0x10);
					static_assert(offsetof(TESFile, m_formUserData) == 0x60);
					static_assert(offsetof(TESFile, m_FileName) == 0x78);
					static_assert(offsetof(TESFile, m_header_chunk) == 0x2A4);
					static_assert(offsetof(TESFile, m_groups) == 0x2F8);
					static_assert(offsetof(TESFile, m_findData) == 0x310);
					static_assert(offsetof(TESFile, m_RecordFlags) == 0x33C);
					static_assert(offsetof(TESFile, m_dependArray) == 0x368);
					static_assert(offsetof(TESFile, m_versionControlID) == 0x3C8);
					static_assert(offsetof(TESFile, m_transientFormIDs) == 0x3E0);
					static_assert(offsetof(TESFile, m_authorName) == 0x380);
					static_assert(offsetof(TESFile, m_description) == 0x390);
				}
			public:
				[[nodiscard]] BSFile* GetFile() const noexcept(true) { return m_file; }
				[[nodiscard]] TESBitArrayFile* GetFormUserData() const noexcept(true) { return m_formUserData; }
				[[nodiscard]] TESBitArrayFile* GetFormVersionData() const noexcept(true) { return m_formVersionData; }
				[[nodiscard]] TESBitArrayFile* GetFormIDData() const noexcept(true) { return m_formIDData; }
				[[nodiscard]] std::uint32_t GetVersionControlID() const noexcept(true) { return m_versionControlID; }
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
				inline void SetMaster(bool Value) noexcept(true) { if (Value) m_RecordFlags |= FILE_RECORD_ESM; else m_RecordFlags &= ~FILE_RECORD_ESM; }
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
				bool IsActiveFileWarn() const noexcept(true);
			
				static bool ReadFirstChunk(const char* fileName, TESChunk& chunk) noexcept(true);
				static std::uint32_t GetTypeFile(const char* fileName) noexcept(true);
			};
			static_assert(sizeof(TESFile) == 0x8D8);
		}
	}
}