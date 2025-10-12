// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "NiAPI\NiTypes.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSFile : public NiAPI::NiFile 
			{
			protected:
				// members
				/*000*/ // NiFile
				/*150*/ std::uint64_t	unk150;
				/*158*/ std::uint64_t	unk158;
				/*160*/ std::uint32_t	unk160;					// init to -1
				/*164*/ std::uint32_t	unk164;
				/*168*/ std::uint32_t	unk168;
				/*16C*/ char		_FileName2[MAX_PATH];	// I don't know why, but this is once again the path to the file
				/*270*/ std::uint64_t	unk270;
				/*278*/ std::uint64_t	unk278;
				/*280*/ std::uint64_t	_Pos2;
				/*288*/ std::uint64_t	_FileSize;
				/*290*/ std::uint64_t	unk290;
				/*298*/ std::uint64_t	unk298;
				/*2A0*/ std::uint32_t	unk2A0;					// init to -1
				/*2A4*/ std::uint32_t	unk2A4;
				/*2A8*/ std::uint32_t	unk2A8;
				/*2AC*/ std::uint32_t	unk2AC;
			public:
				//__forceinline DWORD64 GetFileSize(VOID) const { return _FileSize; }
				static BSFile* CreateInstance(const char* fileName, std::uint32_t mode, std::uint64_t bufferSize, 
					bool isTextFile) noexcept(true);
				void DeleteInstance(bool ReleaseMemory = true);

				std::uint64_t GetFileSize() const noexcept(true);
				std::uint64_t Read(void* buffer, std::uint64_t size) noexcept(true);
				std::uint64_t Write(const void* buffer, std::uint64_t size) noexcept(true);
			public:
				inline static bool(*ICreateInstance)(BSFile*, const char*, std::uint32_t, std::uint64_t, bool);
				static bool HKCreateInstance(BSFile* This, const char* fileName, std::uint32_t mode,
					std::uint64_t bufferSize, bool isTextFile);
			};
			static_assert(sizeof(BSFile) == 0x2B0, "BSFile class should be the size of 0x2B0");
		}
	}
}