// Copyright © 2022-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <windows.h>
#include "BSTList.h"
#include "BSString.h"

namespace CKPE
{
	namespace SkyrimSE
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
				/* 000 */ char _pad0[0x58];
				/* 058 */ char m_FileName[MAX_PATH];
				/* 15C */ char m_FilePath[MAX_PATH];
				/* 260 */ char _pad1[0x8];
				/* 268 */ std::uint32_t m_bufsize;
				/* 26C */ char _pad2[0x38];
				/* 2A4 */ std::uint32_t m_fileSize;
				/* 2A8 */ char _pad3[0x48];

				struct WIN32_FIND_DATA_UNKNOWN 
				{
					FILETIME ftCreationTime;
					char _pad0[0x8];
					FILETIME ftLastWriteTime;
					std::uint32_t nFileSizeHigh;
					std::uint32_t nFileSizeLow;
				};

				/* 2F0 */ WIN32_FIND_DATA_UNKNOWN m_findData;
				/* 310 */ char _pad4[0x11C];

				struct FileHeaderInfo 
				{
					float fileVersion;
					std::uint32_t numRecords;		// number of record blocks in file
					std::uint32_t nextFormID;		// including file index in highest byte
				};

				/* 42C */ FileHeaderInfo m_fileHeader;
				/* 438 */ std::uint32_t m_RecordFlags;

				// Data for Depend files - compared against size in findData of masters to check if they have changed since last edit
				// 08
				struct DependFileData 
				{
					/*00*/ std::uint32_t nFileSizeHigh;
					/*04*/ std::uint32_t nFileSizeLow;
				};
				typedef BSSimpleList<DependFileData*> DependDataList;
				typedef BSSimpleList<const char*> DependNameList;

				/* 43C */ char _pad5[0x4];
				/* 440 */ DependNameList m_dependNames;
				/* 450 */ DependDataList m_dependData;
				/* 460 */ std::uint32_t m_dependCount;
				/* 464 */ char _pad6[0x4];
				/* 468 */ TESFile** m_dependArray;
				/* 470 */ char _pad7[0x8];
				/* 478 */ std::uint8_t m_fileIndex;			// index of this file in load order (or 0xFF if not loaded)
				/* 479 */ char _pad8[0x7];
				/* 480 */ BSString m_authorName;
				/* 490 */ BSString m_description;
				/* 4A0 */ char _pad9[0x20];
				/* 4C0 */ TESFile* m_parentFile;
				/* 4C8 */ char _pad10[0x8];
				/* 4D0 */ std::uint32_t* m_ownedIds;
				/* 4D8 */ std::uint64_t m_countOwned;
			public:
				[[nodiscard]] inline BSString GetAuthorName() const noexcept(true) { return *m_authorName ? m_authorName : "Bethesda Software"; }
				[[nodiscard]] inline BSString GetDescription() const noexcept(true) { return *m_description ? m_description : ""; }
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
				[[nodiscard]] SYSTEMTIME GetCreationTime() const noexcept(true);
				[[nodiscard]] SYSTEMTIME GetLastWriteTime() const noexcept(true);

				inline static int (*LoadTESInfo)(TESFile*);
				inline static __int64 (*WriteTESInfo)(TESFile*);
				inline static bool AllowSaveESM;
				inline static bool AllowMasterESP;

				int hk_LoadTESInfo();
				__int64 hk_WriteTESInfo();
				bool IsActiveFileBlacklist() const noexcept(true);
			public:
				static bool ReadFirstChunk(const char* fileName, TESChunk& chunk) noexcept(true);
				static std::uint32_t GetTypeFile(const char* fileName) noexcept(true);
			};
			static_assert(sizeof(TESFile) == 0x4E0);
		}
	}
}