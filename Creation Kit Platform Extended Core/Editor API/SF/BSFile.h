// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once
#pragma pack(push, 1)

#include <stdint.h>
#include "NiAPI/NiMemoryManager.h"
#include "BSEntryString.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Starfield
		{
			class BSFile {
			public:
				virtual ~BSFile() = default;

				struct FindData
				{
					uint64_t unk00;
					uint64_t unk08;
					uint64_t _FileSize;
					uint64_t unk18;
				};

				enum FileModes : uint32_t {
					fmReadOnly = 0,
					fmWriteOnly = 1,
					fmAppendOnly = 2,
				};

				inline bool Good() const { return _Good; }
				inline FileModes GetMode() const { return _Mode; }
				inline uint64_t GetPosition() const { return _Pos; }
				inline uint64_t GetFileSize() const { return _FileSize; }
				inline uint64_t GetAllocBufferSize() const { return _BufferAllocSize; }
				inline uint64_t GetBufferReadSize() const { return _BufferReadSize; }
				inline uint64_t GetBufferOffset() const { return _BufferOffset; }
				inline const char* GetFileName() const { return _FileName.Get<char>(true); }

				inline static bool(*ICreateInstance)(BSFile*, const char*, uint32_t, uint64_t, bool);
				inline static bool HKCreateInstance(BSFile* This, const char* fileName, uint32_t mode,
					uint64_t bufferSize, bool isTextFile)
				{
					if ((mode == FileModes::fmReadOnly) && (bufferSize < 0x40000))
						bufferSize = 0x40000;

					return ICreateInstance(This, fileName, mode, bufferSize, isTextFile);
				}
				inline static BSFile* CreateInstance(const char* fileName, uint32_t mode, uint64_t bufferSize, bool isTextFile)
				{
					BSFile* NewInstance = (BSFile*)NiAPI::NiMemoryManager::Alloc(nullptr, sizeof(BSFile));
					if (NewInstance && !ICreateInstance(NewInstance, fileName, mode, bufferSize, isTextFile))
					{
						NiAPI::NiMemoryManager::Free(nullptr, NewInstance);
						return nullptr;
					}

					return NewInstance;
				}
				inline void DeleteInstance(bool ReleaseMemory = true) const
				{
					thisVirtualCall<void>(0x0, this, ReleaseMemory);
				}

				inline uint64_t Read(void* buffer, uint64_t size) const
				{
					return thisVirtualCall<uint64_t>(0x28, this, buffer, size);
				}

				inline uint64_t Write(const void* buffer, uint64_t size) const
				{
					return thisVirtualCall<uint64_t>(0x30, this, buffer, size);
				}
			protected:
				// members
				/*008*/ uint64_t _Pos;				// init to 0
				/*010*/ uint64_t _BufferAllocSize;	// init to 0x800	(sent for reading/writing)
				/*018*/ uint64_t _BufferReadSize;	// init to 0		(answered by the function)
				/*020*/ uint64_t _BufferOffset;		// init to 0
				/*028*/ uint64_t _SizeBuffer;		// init to 0
				/*030*/ BSEntryString _FileName;	// 
				char strdata1[MAX_PATH - 0x14];
				/*148*/ FileModes _Mode;
				/*14C*/ uint32_t _Good;			
				/*150*/ uint32_t _Unk02;			
				/*154*/ BSEntryString _fileName2;	// Why?
				char strdata2[MAX_PATH - 0x10];
				/*270*/ char pad270[0x10];
				/*280*/ uint64_t _Pos2;
				/*288*/ uint64_t _FileSize;
				/*290*/ char pad290[0x10];
				/*2A0*/ FindData* _FindData;
				/*2A8*/ uint64_t  unk2A0;			// init to -1
				/*2B0*/ char pad2B0[0x10];			
			};
			static_assert(sizeof(BSFile::FindData) == 0x20, "BSFile::FindData class should be the size of 0x20");
			static_assert(sizeof(BSFile) == 0x2C0, "BSFile class should be the size of 0x2C0");
		}
	}
}

#pragma pack(pop)