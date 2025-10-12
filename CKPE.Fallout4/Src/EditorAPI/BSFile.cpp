// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Utils.h>
#include <EditorAPI/BSFile.h>
#include <EditorAPI/NiAPI/NiMemoryManager.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			std::uintptr_t pointer_BSFile_sub = 0;

			BSFile* BSFile::CreateInstance(const char* fileName, std::uint32_t mode, std::uint64_t bufferSize, 
				bool isTextFile) noexcept(true)
			{
				BSFile* NewInstance = (BSFile*)NiAPI::NiMemoryManager::Alloc(nullptr, sizeof(BSFile));
				this_call<bool, const char*, std::uint32_t, std::uint64_t, bool>(pointer_BSFile_sub, NewInstance, fileName,
					mode, bufferSize, isTextFile);
				return NewInstance;
			}

			void BSFile::DeleteInstance(bool ReleaseMemory) 
			{
				vtbl_call<void>(0x0, this, ReleaseMemory);
			}

			std::uint64_t BSFile::GetFileSize() const noexcept(true)
			{
				return vtbl_call<std::uint64_t>(0x40, this);
			}

			std::uint64_t BSFile::Read(void* buffer, std::uint64_t size) noexcept(true)
			{
				return vtbl_call<std::uint64_t>(0x28, this, buffer, size);
			}

			std::uint64_t BSFile::Write(const void* buffer, std::uint64_t size) noexcept(true)
			{
				return vtbl_call<std::uint64_t>(0x30, this, buffer, size);
			}

			bool BSFile::HKCreateInstance(BSFile* This, const char* fileName, std::uint32_t mode,
				std::uint64_t bufferSize, bool isTextFile)
			{
				if (mode == FileModes::kFileMode_ReadOnly && bufferSize < 0x40000)
					bufferSize = 0x40000;

				return ICreateInstance(This, fileName, mode, bufferSize, isTextFile);
			}
		}
	}
}