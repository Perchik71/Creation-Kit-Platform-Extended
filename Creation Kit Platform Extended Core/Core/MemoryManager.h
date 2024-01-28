// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		class MemoryManager
		{
		public:
			MemoryManager();

			void* MemAlloc(size_t size, size_t alignment = 0, bool aligned = false, bool zeroed = false);
			void MemFree(void* block);
			size_t MemSize(void* block);
		private:
			MemoryManager(const MemoryManager&) = default;
			MemoryManager& operator=(const MemoryManager&) = default;
		};

		extern MemoryManager* GlobalMemoryManagerPtr;
	}
}