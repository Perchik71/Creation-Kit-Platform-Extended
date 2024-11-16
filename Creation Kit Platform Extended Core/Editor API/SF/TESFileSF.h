// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once
#pragma pack(push, 1)

#include "Editor API/BSString.h"
#include "Editor API/BSTArray.h"
#include "Editor API/SF/BSEntryString.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Starfield
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

			// There are a lot of them, for the plugin, actual class TESPackedFile.
			// TESPackedFile, TESFormFile, TESRemappedFile, TESLooseFile, TESVersionedFile.
			class TESFile
			{
			private:
				TESFile() = default;
				virtual ~TESFile() = default;
			public:
				enum : uint32_t
				{
					FILE_RECORD_ESM = 0x1,			// Master plugin
					FILE_RECORD_CHECKED = 0x4,		// Pending load/loaded
					FILE_RECORD_ACTIVE = 0x8,		// Save target
					FILE_RECORD_LOCALIZED = 0x80,	// Strings removed
					FILE_RECORD_LIGHT = 0x100,		// Master plugin (Light)
					// 0x200 ESL (unused ???)
					FILE_RECORD_MID = 0x400,		// Master plugin (Mid)
				};
			private:
				/* 008 */ char _pad0[0x38];
				/* 040 */ char m_FileName[MAX_PATH];
				char _pad1[0x6C];
				bool m_headerRead;
				char _pad2[0x3];
				struct FileHeaderInfo {
					/*00*/ float fileVersion;
					/*04*/ uint32_t numRecords;		// number of record blocks in file
					/*08*/ uint32_t nextFormID;		// including file index in highest byte
				};
				/* 1B4 */ FileHeaderInfo m_fileHeader;
				/* 1C0 */ uint32_t m_Flags;
				uint32_t Unk1C4;

				/* 1C8 */ BSTArrayEx<BSEntryString> m_DependName;
				/* 1D8 */ BSTArrayEx<TESFile*> m_DependFile;
				/* 1E8 */ BSTArrayEx<TESFile*> m_DependFile2;

				char _pad3[0x28];
				uint8_t m_fileIndex;
				char _pad4[0x3];

				/* 224 */ BSStringEx m_authorName;
				/* 234 */ BSStringEx m_description;

				char _pad5[0x2FC];								// Fill 0x00.. omg
				/* 540 */ BSTArrayEx<uint32_t>* m_ArrFormID;	// OwnerID's ??
				char _pad6[0x170];								// Fill 0x00....
				/* 6B8 */ BSTArrayEx<uint32_t>* m_ArrFormID2;	// OwnerID's ??
				char _pad7[0x290];								// Fill 0x00... need kill programmer
				/* 950 */ char m_FilePath[MAX_PATH];
				struct WIN32_FIND_DATA_UNKNOWN {
					char _pad0[0x1C];
					FILETIME ftCreationTime;
					FILETIME ftLastWriteTime;
					uint32_t nFileSizeHigh;
					uint32_t nFileSizeLow;
				};
				/* A54 */ WIN32_FIND_DATA_UNKNOWN m_findData;
				uint32_t UnkA88;
				/* A8C */ uint32_t m_fileSize;			// Supported only 4GB
				char _pad8[0x10];
				/* AA0 */ uint32_t m_bufferSize;		// buffer size used when opening BSFile. init to 0x800
				uint32_t UnkAA4;
				char _pad9[0x8];
			public:
				BSString GetAuthorName() const;				// Important: Sometimes the text is not written as a pointer. (Unsafe)
				BSString GetDescription() const;			// Important: Sometimes the text is not written as a pointer. (Unsafe)
				inline BSString GetFileName() const { return m_FileName ? m_FileName : ""; }
				inline BSString GetFilePath() const { return m_FilePath ? m_FilePath : ""; }
				inline TESFile** GetDependArray() const { return m_DependFile.QBuffer(); }
				inline uint32_t GetDependCount() const { return m_DependFile.QSize(); }
				inline uint8_t GetFileLoadIndex() const { return m_fileIndex; }
				inline uint32_t GetFileSize() const { return m_fileSize; }
				inline uint32_t GetFlags() const { return m_Flags; }
				inline FileHeaderInfo GetHeaderInfo() const { return m_fileHeader; }
				inline bool IsLoaded() const { return m_fileIndex != 0xFF; }
				inline bool IsMaster() const { return m_Flags & FILE_RECORD_ESM; }
				inline bool IsSelected() const { return m_Flags & FILE_RECORD_CHECKED; }
				inline bool IsActive() const { return m_Flags & FILE_RECORD_ACTIVE; }
				inline bool IsLocalized() const { return m_Flags & FILE_RECORD_LOCALIZED; }
				inline bool IsSmallMaster() const { return m_Flags & FILE_RECORD_LIGHT; }
				inline bool IsMediumMaster() const { return m_Flags & FILE_RECORD_MID; }
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
				READ_PROPERTY(GetFileLoadIndex) uint8_t FileLoadIndex;
				READ_PROPERTY(GetDependArray) TESFile** DependArray;
				READ_PROPERTY(GetDependCount) uint32_t DependCount;
				READ_PROPERTY(GetFileSize) uint32_t FileSize;
				READ_PROPERTY(GetCreationTime) SYSTEMTIME CreationTime;
				READ_PROPERTY(GetLastWriteTime) SYSTEMTIME LastWriteTime;
			};
			static_assert(sizeof(TESFile) == 0xAB0);
		}
	}
}

#pragma pack(pop)
