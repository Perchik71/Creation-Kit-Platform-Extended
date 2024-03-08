// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once
#pragma pack(push, 1)

#include "NiAPI\NiTypes.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			class BSFile : public NiAPI::NiFile {
			protected:
				// members
				/*000*/ // NiFile
				/*150*/ uint64_t	unk150;
				/*158*/ uint64_t	unk158;
				/*160*/ uint32_t	unk160;					// init to -1
				/*164*/ uint32_t	unk164;
				/*168*/ uint32_t	unk168;
				/*16C*/ char		_FileName2[MAX_PATH];	// I don't know why, but this is once again the path to the file
				/*270*/ uint64_t	unk270;
				/*278*/ uint64_t	unk278;
				/*280*/ uint64_t	_Pos2;
				/*288*/ uint64_t	_FileSize;
				/*290*/ uint64_t	unk290;
				/*298*/ uint64_t	unk298;
				/*2A0*/ uint32_t	unk2A0;					// init to -1
				/*2A4*/ uint32_t	unk2A4;
				/*2A8*/ uint32_t	unk2A8;
				/*2AC*/ uint32_t	unk2AC;
			public:
				//__forceinline DWORD64 GetFileSize(VOID) const { return _FileSize; }
				static BSFile* CreateInstance(const char* fileName, uint32_t mode, uint64_t bufferSize, bool isTextFile);
				void DeleteInstance(bool ReleaseMemory = true);

				uint64_t GetFileSize() const;
				uint64_t Read(void* buffer, uint64_t size);
				uint64_t Write(const void* buffer, uint64_t size);
			public:
				inline static bool(*ICreateInstance)(BSFile*, const char*, uint32_t, uint64_t, bool);
				static bool HKCreateInstance(BSFile* This, const char* fileName, uint32_t mode, 
					uint64_t bufferSize, bool isTextFile);
			};
			static_assert(sizeof(BSFile) == 0x2B0, "BSFile class should be the size of 0x2B0");
		}
	}
}

#pragma pack(pop)