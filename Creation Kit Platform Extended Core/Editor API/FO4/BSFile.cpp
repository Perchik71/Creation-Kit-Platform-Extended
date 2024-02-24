// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "BSFile.h"
#include "NiAPI\NiMemoryManager.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			using namespace CreationKitPlatformExtended::Core;

			uintptr_t pointer_BSFile_sub = 0;

			BSFile* BSFile::CreateInstance(const char* fileName, uint32_t mode, uint64_t bufferSize, bool isTextFile) 
			{
				BSFile* NewInstance = (BSFile*)NiAPI::NiMemoryManager::Alloc(nullptr, sizeof(BSFile));
				thisCall<bool, const char*, uint32_t, uint64_t, bool>(pointer_BSFile_sub, NewInstance, fileName,
					mode, bufferSize, isTextFile);
				return NewInstance;
			}

			void BSFile::DeleteInstance(bool ReleaseMemory) 
			{
				thisVirtualCall<void>(0x0, this, ReleaseMemory);
			}

			uint64_t BSFile::GetFileSize() const
			{
				return thisVirtualCall<uint64_t>(0x40, this);
			}

			uint64_t BSFile::Read(void* buffer, uint64_t size) 
			{
				return thisVirtualCall<uint64_t>(0x28, this, buffer, size);
			}

			uint64_t BSFile::Write(const void* buffer, uint64_t size) 
			{
				return thisVirtualCall<uint64_t>(0x30, this, buffer, size);
			}

			bool BSFile::HKCreateInstance(BSFile* This, const char* fileName, uint32_t mode,
				uint64_t bufferSize, bool isTextFile)
			{
				if (mode == FileModes::kFileMode_ReadOnly && bufferSize < 0x40000)
					bufferSize = 0x40000;

				// Files larger than 256 MB open synchronously
				if (((mode & FILE_FLAG_NO_BUFFERING) == FILE_FLAG_NO_BUFFERING) &&
					(std::filesystem::file_size(fileName) >= 0x10000000))
					mode &= ~FILE_FLAG_NO_BUFFERING;

				return ICreateInstance(This, fileName, mode, bufferSize, isTextFile);
			}
		}
	}
}