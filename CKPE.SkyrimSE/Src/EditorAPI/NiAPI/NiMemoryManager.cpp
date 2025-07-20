// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Asserts.h>
#include <CKPE.Common.MemoryManager.h>
#include <EditorAPI/NiAPI/NiMemoryManager.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				void* NiMemoryManager::Alloc(const NiMemoryManager* lpManager, std::size_t dwSize, std::size_t dwAlignment)
				{
					auto p = aligned_malloc(dwSize, dwAlignment);
					if (p) memset(p, 0, dwSize);
					return p;
				}

				void NiMemoryManager::Free(const NiMemoryManager* lpManager, void* lpPointer)
				{
					aligned_free(lpPointer);
				}

				std::size_t NiMemoryManager::Size(const NiMemoryManager* lpManager, void* lpPointer)
				{
					// only own memmgr
					return msize(lpPointer);
				}
			}
		}
	}
}