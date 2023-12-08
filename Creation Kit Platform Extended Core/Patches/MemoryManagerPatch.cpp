// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "MemoryManagerPatch.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		constexpr auto MEM_THRESHOLD = 2147483648;
		constexpr auto MEM_GB = 1073741824;

		class MemoryManager
		{
			// Не описываем конструкторы и деструкторы
			// Класс - это просто оболочка
		public:
			static void* Allocate(MemoryManager* manager, size_t size, uint32_t alignment, bool aligned)
			{
				return MemoryManagerPatch::MemAlloc(size, alignment, aligned, true);
			}

			static void Deallocate(MemoryManager* manager, void* memory, bool aligned)
			{
				voltek::scalable_free(memory);
			}

			static size_t Size(MemoryManager* manager, void* memory)
			{
				return voltek::scalable_msize(memory);
			}
		};

		class ScrapHeap
		{
			// Не описываем конструкторы и деструкторы
			// Класс - это просто оболочка
		public:
			static void* Allocate(ScrapHeap* manager, size_t size, uint32_t alignment)
			{
				return MemoryManagerPatch::MemAlloc(size, alignment, alignment != 0);
			}

			static void Deallocate(ScrapHeap* manager, void* memory)
			{
				voltek::scalable_free(memory);
			}
		};

		namespace Skyrim
		{
			class bhkThreadMemorySource
			{
			private:
				char _pad0[0x8];
				CRITICAL_SECTION m_CritSec;
			public:
				DECLARE_CONSTRUCTOR_HOOK(bhkThreadMemorySource);

				bhkThreadMemorySource();
				virtual ~bhkThreadMemorySource();
				virtual void* blockAlloc(size_t numBytes);
				virtual void blockFree(void* p, size_t numBytes);
				virtual void* bufAlloc(size_t& reqNumBytesInOut);
				virtual void bufFree(void* p, size_t numBytes);
				virtual void* bufRealloc(void* pold, size_t oldNumBytes, size_t& reqNumBytesInOut);
				virtual void blockAllocBatch(void** ptrsOut, size_t numPtrs, size_t blockSize);
				virtual void blockFreeBatch(void** ptrsIn, size_t numPtrs, size_t blockSize);
				virtual void getMemoryStatistics(class MemoryStatistics& u);
				virtual size_t getAllocatedSize(const void* obj, size_t nbytes);
				virtual void resetPeakMemoryStatistics();
			};

			bhkThreadMemorySource::bhkThreadMemorySource()
			{
				InitializeCriticalSection(&m_CritSec);
			}

			bhkThreadMemorySource::~bhkThreadMemorySource()
			{
				DeleteCriticalSection(&m_CritSec);
			}

			void* bhkThreadMemorySource::blockAlloc(size_t numBytes)
			{
				return MemoryManager::Allocate(nullptr, numBytes, 16, true);
			}

			void bhkThreadMemorySource::blockFree(void* p, size_t numBytes)
			{
				MemoryManager::Deallocate(nullptr, p, true);
			}

			void* bhkThreadMemorySource::bufAlloc(size_t& reqNumBytesInOut)
			{
				return blockAlloc(reqNumBytesInOut);
			}

			void bhkThreadMemorySource::bufFree(void* p, size_t numBytes)
			{
				return blockFree(p, numBytes);
			}

			void* bhkThreadMemorySource::bufRealloc(void* pold, size_t oldNumBytes, size_t& reqNumBytesInOut)
			{
				void* p = blockAlloc(reqNumBytesInOut);
				memcpy(p, pold, oldNumBytes);
				blockFree(pold, oldNumBytes);

				return p;
			}

			void bhkThreadMemorySource::blockAllocBatch(void** ptrsOut, size_t numPtrs, size_t blockSize)
			{
				for (long i = 0; i < numPtrs; i++)
					ptrsOut[i] = blockAlloc(blockSize);
			}

			void bhkThreadMemorySource::blockFreeBatch(void** ptrsIn, size_t numPtrs, size_t blockSize)
			{
				for (long i = 0; i < numPtrs; i++)
					blockFree(ptrsIn[i], blockSize);
			}

			void bhkThreadMemorySource::getMemoryStatistics(class MemoryStatistics& u)
			{
				// Ничего
			}

			size_t bhkThreadMemorySource::getAllocatedSize(const void* obj, size_t nbytes)
			{
				Assert(false);
				return 0;
			}

			void bhkThreadMemorySource::resetPeakMemoryStatistics()
			{
				// Ничего
			}
		}

		namespace Fallout4
		{
			class bhkThreadMemorySource
			{
			private:
				char _pad0[0x8];
				CRITICAL_SECTION m_CritSec;
			public:
				DECLARE_CONSTRUCTOR_HOOK(bhkThreadMemorySource);

				bhkThreadMemorySource();
				virtual ~bhkThreadMemorySource();
				virtual void* blockAlloc(size_t numBytes);
				virtual void blockFree(void* p, size_t numBytes);
				virtual void* bufAlloc(size_t& reqNumBytesInOut);
				virtual void bufFree(void* p, size_t numBytes);
				virtual void* bufRealloc(void* pold, size_t oldNumBytes, size_t& reqNumBytesInOut);
				virtual void blockAllocBatch(void** ptrsOut, size_t numPtrs, size_t blockSize);
				virtual void blockFreeBatch(void** ptrsIn, size_t numPtrs, size_t blockSize);
				virtual void getMemoryStatistics(class MemoryStatistics& u);
				virtual size_t getAllocatedSize(const void* obj, size_t nbytes);
				virtual void resetPeakMemoryStatistics();
				virtual void* getExtendedInterface();
			};

			bhkThreadMemorySource::bhkThreadMemorySource()
			{
				InitializeCriticalSection(&m_CritSec);
			}

			bhkThreadMemorySource::~bhkThreadMemorySource()
			{
				DeleteCriticalSection(&m_CritSec);
			}

			void* bhkThreadMemorySource::blockAlloc(size_t numBytes)
			{
				return MemoryManager::Allocate(nullptr, numBytes, 16, true);
			}

			void bhkThreadMemorySource::blockFree(void* p, size_t numBytes)
			{
				MemoryManager::Deallocate(nullptr, p, true);
			}

			void* bhkThreadMemorySource::bufAlloc(size_t& reqNumBytesInOut)
			{
				return blockAlloc(reqNumBytesInOut);
			}

			void bhkThreadMemorySource::bufFree(void* p, size_t numBytes)
			{
				return blockFree(p, numBytes);
			}

			void* bhkThreadMemorySource::bufRealloc(void* pold, size_t oldNumBytes, size_t& reqNumBytesInOut)
			{
				void* p = blockAlloc(reqNumBytesInOut);
				memcpy(p, pold, oldNumBytes);
				blockFree(pold, oldNumBytes);

				return p;
			}

			void bhkThreadMemorySource::blockAllocBatch(void** ptrsOut, size_t numPtrs, size_t blockSize)
			{
				for (long i = 0; i < numPtrs; i++)
					ptrsOut[i] = blockAlloc(blockSize);
			}

			void bhkThreadMemorySource::blockFreeBatch(void** ptrsIn, size_t numPtrs, size_t blockSize)
			{
				for (long i = 0; i < numPtrs; i++)
					blockFree(ptrsIn[i], blockSize);
			}

			void bhkThreadMemorySource::getMemoryStatistics(class MemoryStatistics& u)
			{
				// Ничего
			}

			size_t bhkThreadMemorySource::getAllocatedSize(const void* obj, size_t nbytes)
			{
				Assert(FALSE);
				return 0;
			}

			void bhkThreadMemorySource::resetPeakMemoryStatistics()
			{
				// Ничего
			}

			void* bhkThreadMemorySource::getExtendedInterface()
			{
				return nullptr;
			}
		}

		MemoryManagerPatch::MemoryManagerPatch() : Module(GlobalEnginePtr)
		{}

		bool MemoryManagerPatch::HasOption() const
		{
			return false;
		}

		bool MemoryManagerPatch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* MemoryManagerPatch::GetOptionName() const
		{
			return nullptr;
		}

		const char* MemoryManagerPatch::GetName() const
		{
			return "Memory Manager";
		}

		void* MemoryManagerPatch::HkCalloc(size_t count, size_t size)
		{
			return MemAlloc(count * size, 0, false, true);
		}

		void* MemoryManagerPatch::HkMalloc(size_t size)
		{
			return MemAlloc(size);
		}

		void* MemoryManagerPatch::HkAlignedMalloc(size_t size, size_t alignment)
		{
			return MemAlloc(size, alignment, true);
		}

		void* MemoryManagerPatch::HkRealloc(void* memory, size_t size)
		{
			void* newMemory = nullptr;

			if (size > 0)
			{
				// Recalloc behaves like calloc if there's no existing allocation. Realloc doesn't. Zero it either way.
				newMemory = MemAlloc(size, 0, false, true);

				if (memory)
					memcpy(newMemory, memory, std::min(size, voltek::scalable_msize(memory)));
			}

			voltek::scalable_free(memory);
			return newMemory;
		}

		void* MemoryManagerPatch::HkRecalloc(void* memory, size_t count, size_t size)
		{
			return HkRealloc(memory, count * size);
		}

		void MemoryManagerPatch::HkFree(void* block)
		{
			voltek::scalable_free(block);
		}

		void MemoryManagerPatch::HkAlignedFree(void* block)
		{
			voltek::scalable_free(block);
		}

		size_t MemoryManagerPatch::HkMemSize(void* block)
		{
			return voltek::scalable_msize(block);
		}

		char* MemoryManagerPatch::HkStrDup(const char* str)
		{
			return CreationKitPlatformExtended::Utils::StrDub(str);
		}

		void* MemoryManagerPatch::MemAlloc(size_t size, size_t alignment, bool aligned, bool zeroed)
		{
			// Если не задано, то будет 4
			if (!aligned)
				alignment = 4;

			if (!size)
			{
				size = 1;
				alignment = 4;
			}

			AssertMsgVa(alignment != 0 && alignment % 2 == 0, "Alignment is fucked: %llu", alignment);

			// Должно быть в степени 2, округлить его, если необходимо
			if ((alignment & (alignment - 1)) != 0)
			{
				alignment--;
				alignment |= alignment >> 1;
				alignment |= alignment >> 2;
				alignment |= alignment >> 4;
				alignment |= alignment >> 8;
				alignment |= alignment >> 16;
				alignment++;
			}

			// Размер должен быть кратен выравниванию с округлением до ближайшего
			if ((size % alignment) != 0)
				size = ((size + alignment - 1) / alignment) * alignment;

			void* ptr = voltek::scalable_alloc(size);
			if (ptr && zeroed) memset(ptr, 0, size);

			if (!ptr && size <= (128 * 1024 * 1024))
				AssertMsgVa(false, "A memory allocation failed. This is due to memory leaks in the Creation Kit or not"
					" having enough free RAM.\n\nRequested chunk size: %llu bytes.", size);

			return ptr;
		}

		bool MemoryManagerPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return true;
		}

		bool MemoryManagerPatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			// Принудительный вылет с сообщением для пользователя.
			AssertMsg(LowPhysicalMemory(), "Not enough memory to run the program");

			auto TotalGB = (double)(Utils::GetTotalPhysicalMemory()) / MEM_GB;
			auto AvailableTotalGB = (double)(Utils::GetAvailableTotalPhysicalMemory()) / MEM_GB;

			_MESSAGE("Memory (Total: %.1f Gb, Available: %.1f Gb)", TotalGB, AvailableTotalGB);

			auto patchIAT = [](const char* module)
			{
				PatchIAT(HkCalloc, module, "calloc");
				PatchIAT(HkMalloc, module, "malloc");
				PatchIAT(HkAlignedMalloc, module, "_aligned_malloc");
				PatchIAT(HkRealloc, module, "realloc");
				PatchIAT(HkRecalloc, module, "_recalloc");
				PatchIAT(HkFree, module, "free");
				PatchIAT(HkAlignedFree, module, "_aligned_free");
				PatchIAT(HkMemSize, module, "_msize");
				PatchIAT(HkStrDup, module, "_strdup");
				PatchIAT(memcpy_s, module, "memcpy_s");
				PatchIAT(memcpy_s, module, "memmove_s");
				PatchIAT(memcmp, module, "memcmp");
				PatchIAT(memcpy, module, "memcpy");
				PatchIAT(memmove, module, "memmove");
				PatchIAT(memset, module, "memset");
			};

			patchIAT("API-MS-WIN-CRT-HEAP-L1-1-0.DLL");
			patchIAT("MSVCR110.DLL");

			if (lpRelocationDatabaseItem->Version() == 1)
			{
				lpRelocator->DetourJump(lpRelocationDatabaseItem->At(0), (uintptr_t)&MemoryManager::Allocate);
				lpRelocator->DetourJump(lpRelocationDatabaseItem->At(1), (uintptr_t)&MemoryManager::Deallocate);
				lpRelocator->DetourJump(lpRelocationDatabaseItem->At(2), (uintptr_t)&MemoryManager::Size);
				lpRelocator->DetourJump(lpRelocationDatabaseItem->At(3), (uintptr_t)&ScrapHeap::Allocate);
				lpRelocator->DetourJump(lpRelocationDatabaseItem->At(4), (uintptr_t)&ScrapHeap::Deallocate);

				lpRelocator->DetourFunctionClass(lpRelocationDatabaseItem->At(5), 
					(uintptr_t)&Skyrim::bhkThreadMemorySource::__ctor__);

				{
					ScopeRelocator SectionTextProtectionRemove;

					lpRelocator->Patch(lpRelocationDatabaseItem->At(6), { 0xC3 });
					lpRelocator->Patch(lpRelocationDatabaseItem->At(7), { 0xC3 });
					lpRelocator->Patch(lpRelocationDatabaseItem->At(8), { 0xC3 });
					lpRelocator->Patch(lpRelocationDatabaseItem->At(9), { 0xC3 });
				}

				// Программа очень любит думать, а винде это не нравиться, скажем винде, чтоб не обращала внимание.
				DisableProcessWindowsGhosting();

				return true;
			}

			return false;
		}

		bool MemoryManagerPatch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}

		bool MemoryManagerPatch::LowPhysicalMemory()
		{
			return Utils::GetAvailableTotalPhysicalMemory() > MEM_THRESHOLD;
		}
	}
}