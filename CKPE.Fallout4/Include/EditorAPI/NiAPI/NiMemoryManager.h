// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiMemoryManager 
				{
					NiMemoryManager(const NiMemoryManager&) = delete;
					NiMemoryManager& operator=(const NiMemoryManager&) = delete;
				public:
					NiMemoryManager() = default;
					~NiMemoryManager() = default;
				public:
					static void* Alloc(const NiMemoryManager* lpManager, std::size_t dwSize, std::size_t dwAlignment = 0);
					static void	Free(const NiMemoryManager* lpManager, void* lpPointer);
					static std::size_t Size(const NiMemoryManager* lpManager, void* lpPointer);
				};
			}
		}
	}
}