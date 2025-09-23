// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Asserts.h>
#include <CKPE.Common.MemoryManager.h>
#include <EditorAPI/NiAPI/NiMemoryManager.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				void* NiMemoryManager::Alloc(const NiMemoryManager* lpManager, std::size_t dwSize, std::size_t dwAlignment)
				{
					return Common::MemoryManager::GetSingleton()->MemAlloc(dwSize, dwAlignment, dwAlignment != 0);
				}

				void NiMemoryManager::Free(const NiMemoryManager* lpManager, void* lpPointer)
				{
					Common::MemoryManager::GetSingleton()->MemFree(lpPointer);
				}

				std::size_t NiMemoryManager::Size(const NiMemoryManager* lpManager, void* lpPointer)
				{
					return Common::MemoryManager::GetSingleton()->MemSize(lpPointer);
				}
			}
		}
	}
}