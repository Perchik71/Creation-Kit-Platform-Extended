// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once
#pragma pack(push, 1)

#include "NiAPI/NiMemoryManager.h"
#include "NiAPI/NiTypes.h"
#include <CKPE.Utils.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			// `class BSFile`: VTable [0x000000000920E230, 0x0000000000000000 offset, 20 functions] `.?AVBSFile@@`
			class BSFile : public NiAPI::NiFile
			{
				struct FindData
				{
					std::uint64_t unk00;
					std::uint64_t unk08;
					std::uint64_t _FileSize;
					std::uint64_t unk18;
				};
			public:
				virtual ~BSFile() = default;

				[[nodiscard]] inline const char* GetFileName2() const { return _FileName2; }
				[[nodiscard]] inline std::uint64_t GetFileSize() const { return _FileSize; }

				inline static bool(*ICreateInstance)(BSFile*, const char*, FileModes, std::uint64_t, bool);
				inline static bool HKCreateInstance(BSFile* This, const char* fileName, FileModes mode,
					std::uint64_t bufferSize, bool isTextFile)
				{
					if ((mode == FileModes::kFileMode_ReadOnly) && (bufferSize < 0x40000))
						bufferSize = 0x40000;

					return ICreateInstance(This, fileName, mode, bufferSize, isTextFile);
				}
				inline static BSFile* CreateInstance(const char* fileName, FileModes mode, std::uint64_t bufferSize, 
					bool isTextFile)
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
					vtbl_call<void>(0x0, this, ReleaseMemory);
				}

				inline std::uint64_t Read(void* buffer, std::uint64_t size) const
				{
					return vtbl_call<std::uint64_t>(0x28, this, buffer, size);
				}

				inline std::uint64_t Write(const void* buffer, std::uint64_t size) const
				{
					return vtbl_call<std::uint64_t>(0x30, this, buffer, size);
				}
			protected:
				/*150*/ char pad150[0x1C];
				/*16C*/ char _FileName2[260];
				/*270*/ char pad270[0x10];
				/*280*/ std::uint64_t _Pos2;
				/*288*/ std::uint64_t _FileSize;
				/*290*/ char pad290[0x10];
				/*2A0*/ FindData* _FindData;
				/*2A8*/ std::uint64_t _unk2A0;			// init to -1
				/*2B0*/ char pad2B0[0x10];
			};
			//static_assert(sizeof(BSFile::FindData) == 0x20, "BSFile::FindData class should be the size of 0x20");
			static_assert(sizeof(BSFile) == 0x2C0, "BSFile class should be the size of 0x2C0");
		}
	}
}

#pragma pack(pop)