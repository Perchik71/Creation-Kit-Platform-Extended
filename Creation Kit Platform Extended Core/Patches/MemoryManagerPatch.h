// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Module.h"
#include "Core/Relocator.h"
#include "Core/RelocationDatabase.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		using namespace CreationKitPlatformExtended::Core;

		class MemoryManagerPatch : public Module
		{
		public:
			MemoryManagerPatch();

			virtual bool HasOption() const;
			virtual bool HasCanRuntimeDisabled() const;
			virtual const char* GetOptionName() const;
			virtual const char* GetName() const;
			virtual bool HasDependencies() const;
			virtual Array<String> GetDependencies() const;
		public:
			static void* HkCalloc(size_t count, size_t size);
			static void* HkMalloc(size_t size);
			static void* HkAlignedMalloc(size_t size, size_t alignment);
			static void* HkRealloc(void* memory, size_t size);
			static void* HkRecalloc(void* memory, size_t count, size_t size);
			static void HkFree(void* block);
			static void HkAlignedFree(void* block);
			static size_t HkMemSize(void* block);
			static char* HkStrDup(const char* str);
			static void* MemAlloc(size_t size, size_t alignment = 0, bool aligned = false, bool zeroed = false);
		protected:
			virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const;
			virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
		private:
			MemoryManagerPatch(const MemoryManagerPatch&) = default;
			MemoryManagerPatch& operator=(const MemoryManagerPatch&) = default; 

			inline bool LowPhysicalMemory();
		};
	}
}
