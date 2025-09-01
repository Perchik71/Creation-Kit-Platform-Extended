// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <windows.h>
#include <EditorAPI/BSString.h>
#include <EditorAPI/BSTArray.h>
#include <EditorAPI/BSEntryString.h>

namespace CKPE
{
	namespace Starfield
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

			// There are a lot of them, for the plugin, actual class TESPackedFile.
			// TESPackedFile, TESFormFile, TESRemappedFile, TESLooseFile, TESVersionedFile.
			class TESFile
			{
			private:
				TESFile() = default;
				virtual ~TESFile() = default;
			public:
				enum : std::uint32_t
				{
					FILE_RECORD_ESM = 0x1,			// Master plugin
					FILE_RECORD_CHECKED = 0x4,		// Pending load/loaded
					FILE_RECORD_ACTIVE = 0x8,		// Save target
					FILE_RECORD_LOCALIZED = 0x80,	// Strings removed
					FILE_RECORD_LIGHT = 0x100,		// Master plugin (Light)
					// 0x200 ESL (unused ???)
					FILE_RECORD_MID = 0x400,		// Master plugin (Mid)
				};
			//public:
			private:
				/* 008 */ char _pad0[0x38];
				/* 040 */ char m_FileName[MAX_PATH];
				char _pad1[0x6C];
				bool m_headerRead;
				char _pad2[0x3];
				struct FileHeaderInfo
				{
					/*00*/ float fileVersion;
					/*04*/ std::uint32_t numRecords;		// number of record blocks in file
					/*08*/ std::uint32_t nextFormID;		// including file index in highest byte
				};
				/* 1B4 */ FileHeaderInfo m_fileHeader;
				/* 1C0 */ std::uint32_t m_Flags;
				std::uint32_t Unk1C4;

				/* 1C8 */ BSTArray<BSEntryString> m_DependName;
				/* 1D8 */ BSTArray<TESFile*> m_DependFile;
				/* 1E8 */ BSTArray<TESFile*> m_DependFile2;

				char _pad3[0x28];
				std::uint8_t m_fileIndex;
				char _pad4[0x3];

				/* 224 */ BSString m_authorName;
				/* 234 */ BSString m_description;

				char _pad5[0x2FC];									// Fill 0x00.. omg
				/* 540 */ BSTArray<std::uint32_t>* m_ArrFormID;		// OwnerID's ??
				char _pad6[0x170];									// Fill 0x00....
				/* 6B8 */ BSTArray<std::uint32_t>* m_ArrFormID2;	// OwnerID's ??
				char _pad7[0x290];									// Fill 0x00... need kill programmer
				/* 950 */ char m_FilePath[MAX_PATH];
				struct WIN32_FIND_DATA_UNKNOWN
				{
					char _pad0[0x1C];
					FILETIME ftCreationTime;
					FILETIME ftLastWriteTime;
					std::uint32_t nFileSizeHigh;
					std::uint32_t nFileSizeLow;
				};
				/* A54 */ WIN32_FIND_DATA_UNKNOWN m_findData;
				std::uint32_t UnkA88;
				/* A8C */ std::uint32_t m_fileSize;			// Supported only 4GB
				char _pad8[0x10];
				/* AA0 */ std::uint32_t m_bufferSize;		// buffer size used when opening BSFile. init to 0x800
				std::uint32_t UnkAA4;
			public:
				const char* GetAuthorName() const noexcept(true);
				const char* GetDescription() const noexcept(true);
				inline const char* GetFileName() const noexcept(true) { return m_FileName ? m_FileName : ""; }
				inline const char* GetFilePath() const noexcept(true) { return m_FilePath ? m_FilePath : ""; }
				inline TESFile** GetDependArray() const noexcept(true) { return const_cast<TESFile**>(m_DependFile.data()); }
				inline std::uint32_t GetDependCount() const noexcept(true) { return m_DependFile.size(); }
				inline std::uint8_t GetFileLoadIndex() const noexcept(true) { return m_fileIndex; }
				inline std::uint32_t GetFileSize() const noexcept(true) { return m_fileSize; }
				inline std::uint32_t GetFlags() const noexcept(true) { return m_Flags; }
				inline FileHeaderInfo GetHeaderInfo() const noexcept(true) { return m_fileHeader; }
				inline bool IsLoaded() const noexcept(true) { return m_fileIndex != 0xFF; }
				inline bool IsMaster() const noexcept(true) { return m_Flags & FILE_RECORD_ESM; }
				inline bool IsSelected() const noexcept(true) { return m_Flags & FILE_RECORD_CHECKED; }
				inline bool IsActive() const noexcept(true) { return m_Flags & FILE_RECORD_ACTIVE; }
				inline bool IsLocalized() const noexcept(true) { return m_Flags & FILE_RECORD_LOCALIZED; }
				inline bool IsSmallMaster() const noexcept(true) { return m_Flags & FILE_RECORD_LIGHT; }
				inline bool IsMediumMaster() const noexcept(true) { return m_Flags & FILE_RECORD_MID; }
				SYSTEMTIME GetCreationTime() const noexcept(true);
				SYSTEMTIME GetLastWriteTime() const noexcept(true);
				static bool ReadFirstChunk(const char* fileName, TESChunk& chunk) noexcept(true);
				static std::uint32_t GetTypeFile(const char* fileName) noexcept(true);
			public:
				inline static std::int32_t(*LoadTESInfo)(TESFile*);
				inline static std::int64_t(*WriteTESInfo)(TESFile*);
				std::int32_t hk_LoadTESInfo() noexcept(true);
				std::int64_t hk_WriteTESInfo() noexcept(true);
				static void hk_ConversionPlugin(void* __This, std::uint32_t Type) noexcept(true);
				bool IsMasterFileToBlacklist() const noexcept(true);
			public:
				inline static bool AllowSaveESM;
				inline static bool AllowMasterESP;
			public:
				CKPE_READ_PROPERTY(GetAuthorName) const char* AuthorName;
				CKPE_READ_PROPERTY(GetDescription) const char* Description;
				CKPE_READ_PROPERTY(GetFileName) const char* FileName;
				CKPE_READ_PROPERTY(GetFilePath) const char* FilePath;
				CKPE_READ_PROPERTY(GetFlags) std::uint32_t Flags;
				CKPE_READ_PROPERTY(GetFileLoadIndex) std::uint8_t FileLoadIndex;
				CKPE_READ_PROPERTY(GetDependArray) TESFile** DependArray;
				CKPE_READ_PROPERTY(GetDependCount) std::uint32_t DependCount;
				CKPE_READ_PROPERTY(GetFileSize) std::uint32_t FileSize;
				CKPE_READ_PROPERTY(GetCreationTime) SYSTEMTIME CreationTime;
				CKPE_READ_PROPERTY(GetLastWriteTime) SYSTEMTIME LastWriteTime;
			};
			//static_assert(offsetof(TESFile, m_FilePath) == 0x950);
			static_assert(sizeof(TESFile) == 0xAA8);
		}
	}
}