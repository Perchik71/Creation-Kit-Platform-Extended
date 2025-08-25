// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Asserts.h>
#include <CKPE.HardwareInfo.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.MemoryManager.h>
#include <CKPE.Starfield.VersionLists.h>
#include <Patches/CKPE.Starfield.Patch.MemoryManager.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			constexpr auto MEM_THRESHOLD = 20;	// 20Gb

			class BSMemoryManager
			{
				// Не описываем конструкторы и деструкторы
				// Класс - это просто оболочка
			public:
				[[nodiscard]] inline static std::size_t GetAlignSize(std::size_t size, std::size_t alignment)
				{
					alignment = ((alignment + 15) & ~15);
					return (alignment + size) & ~alignment;
				}

				[[nodiscard]] inline static void* Allocate(BSMemoryManager* manager, std::size_t size, 
					std::uint32_t alignment, bool aligned)
				{
					auto ptr = Common::MemoryManager::GetSingleton()->MemAlloc(GetAlignSize(size, alignment), 16, true);
					return ptr;
				}

				inline static void Deallocate(BSMemoryManager* manager, void* memory, bool aligned)
				{
					Common::MemoryManager::GetSingleton()->MemFree(memory);
				}

				[[nodiscard]] inline static std::size_t Size(BSMemoryManager* manager, void* memory)
				{
					return Common::MemoryManager::GetSingleton()->MemSize(memory);
				}
			};

			class HeapAllocator
			{
				// Не описываем конструкторы и деструкторы
				// Класс - это просто оболочка
			public:
				[[nodiscard]] inline static void* Allocate(HeapAllocator* manager, std::size_t size, std::uint32_t alignment)
				{
					return Common::MemoryManager::GetSingleton()->MemAlloc(
						BSMemoryManager::GetAlignSize(size, alignment), 16, true);
				}

				inline static void Deallocate(HeapAllocator* manager, void* memory)
				{
					Common::MemoryManager::GetSingleton()->MemFree(memory);
				}

				[[nodiscard]] inline static std::size_t Size(HeapAllocator* manager, void* memory)
				{
					return Common::MemoryManager::GetSingleton()->MemSize(memory);
				}

				[[nodiscard]] static std::size_t BlockSize(HeapAllocator* manager, void* memory)
				{
					return BSMemoryManager::GetAlignSize(Size(nullptr, memory), 16) + 16;
				}
			};

			class bhkThreadMemorySource
			{
			private:
				char _pad0[0x8]{};
				char _nameClass[0x20]{};
			public:
				inline static bhkThreadMemorySource** Instance;
				static bhkThreadMemorySource* init(bhkThreadMemorySource* newInstance);

				bhkThreadMemorySource();
				virtual ~bhkThreadMemorySource() = default;
				virtual void* blockAlloc(std::size_t numBytes);
				virtual void blockFree(void* p, std::size_t numBytes);
				virtual void* blockRealloc(void* pold, std::size_t oldNumBytes, std::size_t& reqNumBytesInOut);
				virtual void blockAllocBatch(void** ptrsOut, std::size_t numPtrs, std::size_t blockSize);
				virtual void blockFreeBatch(void** ptrsIn, std::size_t numPtrs, std::size_t blockSize);
				virtual void getMemoryStatistics(class MemoryStatistics& u);
				virtual std::size_t getAllocatedSize(const void* obj, std::size_t nbytes);
				virtual void resetPeakMemoryStatistics();
				virtual void unk40();
				virtual void unk48();
				virtual void unk50();
				virtual void unk58();
				virtual void* getExtendedInterface();
			};

			bhkThreadMemorySource* bhkThreadMemorySource::init(bhkThreadMemorySource* newInstance)
			{
				*Instance = new(newInstance) bhkThreadMemorySource();
				return newInstance;
			}

			bhkThreadMemorySource::bhkThreadMemorySource()
			{
				strcpy_s(_nameClass, "bhkThreadMemorySource");
			}

			void* bhkThreadMemorySource::blockAlloc(std::size_t numBytes)
			{
				return BSMemoryManager::Allocate(nullptr, numBytes, 16, true);
			}

			void bhkThreadMemorySource::blockFree(void* p, std::size_t numBytes)
			{
				BSMemoryManager::Deallocate(nullptr, p, true);
			}

			void* bhkThreadMemorySource::blockRealloc(void* pold, std::size_t oldNumBytes, std::size_t& reqNumBytesInOut)
			{
				void* p = blockAlloc(reqNumBytesInOut);
				memcpy(p, pold, oldNumBytes);
				blockFree(pold, oldNumBytes);

				return p;
			}

			void bhkThreadMemorySource::blockAllocBatch(void** ptrsOut, std::size_t numPtrs, std::size_t blockSize)
			{
				for (long i = 0; i < numPtrs; i++)
					ptrsOut[i] = blockAlloc(blockSize);
			}

			void bhkThreadMemorySource::blockFreeBatch(void** ptrsIn, std::size_t numPtrs, std::size_t blockSize)
			{
				for (long i = 0; i < numPtrs; i++)
					blockFree(ptrsIn[i], blockSize);
			}

			void bhkThreadMemorySource::getMemoryStatistics(class MemoryStatistics& u)
			{
				// Ничего
			}

			std::size_t bhkThreadMemorySource::getAllocatedSize(const void* obj, std::size_t nbytes)
			{
				CKPE_ASSERT(false);
				return 0;
			}

			void bhkThreadMemorySource::resetPeakMemoryStatistics()
			{
				// Ничего
			}

			void bhkThreadMemorySource::unk40()
			{
				// Ничего
			}

			void bhkThreadMemorySource::unk48()
			{
				// Ничего
			}

			void bhkThreadMemorySource::unk50()
			{
				// Ничего
			}

			void bhkThreadMemorySource::unk58()
			{
				// Ничего
			}

			void* bhkThreadMemorySource::getExtendedInterface()
			{
				return nullptr;
			}

			MemoryManager::MemoryManager() : Common::Patch()
			{
				SetName("Memory Manager");
			}

			bool MemoryManager::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* MemoryManager::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool MemoryManager::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> MemoryManager::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool MemoryManager::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_STARFIELD_LAST;
			}

			bool MemoryManager::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 3)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				auto Physical = HardwareInfo::OS::GetPhysicalMemory();
				auto Shared = HardwareInfo::OS::GetSharedMemory();
				_MESSAGE("\t\tPhysical Memory (Total: %.1f Gb, Available: %.1f Gb)", Physical.Total, Physical.Free);
				_MESSAGE("\t\tMemory (Total: %.1f Gb, Available: %.1f Gb)", Shared.Total, Shared.Free);

				// Программа очень любит думать, а винде это не нравиться, скажем винде, чтоб не обращала внимание.
				DisableProcessWindowsGhosting();

				auto patchIAT = [&base](const char* module)
					{
						Detours::DetourIAT(base, module, "malloc", (std::uintptr_t)malloc);
						Detours::DetourIAT(base, module, "calloc", (std::uintptr_t)calloc);
						Detours::DetourIAT(base, module, "realloc", (std::uintptr_t)realloc);
						Detours::DetourIAT(base, module, "_recalloc", (std::uintptr_t)recalloc);
						Detours::DetourIAT(base, module, "free", (std::uintptr_t)free);
						Detours::DetourIAT(base, module, "_msize", (std::uintptr_t)msize);
						Detours::DetourIAT(base, module, "_strdup", (std::uintptr_t)strdup);
						Detours::DetourIAT(base, module, "memcpy_s", (std::uintptr_t)memcpy_s);
						Detours::DetourIAT(base, module, "memmove_s", (std::uintptr_t)memmove_s);
						Detours::DetourIAT(base, module, "memcmp", (std::uintptr_t)memcmp);
						Detours::DetourIAT(base, module, "memcpy", (std::uintptr_t)memcpy);
						Detours::DetourIAT(base, module, "memmove", (std::uintptr_t)memmove);
						Detours::DetourIAT(base, module, "memset", (std::uintptr_t)memset);
					};

				patchIAT("API-MS-WIN-CRT-HEAP-L1-1-0.DLL");
				patchIAT("MSVCR110.DLL");

				// Принудительный вылет с сообщением для пользователя.
				CKPE_ASSERT_MSG(LowMemory(), "Not enough memory to run the program");

				Detours::DetourJump(__CKPE_OFFSET(0), (std::uintptr_t)&BSMemoryManager::Allocate);
				Detours::DetourJump(__CKPE_OFFSET(1), (std::uintptr_t)&BSMemoryManager::Deallocate);
				Detours::DetourJump(__CKPE_OFFSET(2), (std::uintptr_t)&BSMemoryManager::Size);

				// They added a static variable, surprisingly, they preferred setting this in the class constructor,
				// they clearly did not read Alen I. Holub... (as can't write in C/C++)
				// https://www.amazon.com/Enough-Rope-Shoot-Yourself-Foot/dp/0070296898
				bhkThreadMemorySource::Instance = (bhkThreadMemorySource**)(__CKPE_OFFSET(3));
				Detours::DetourJump(__CKPE_OFFSET(4), (std::uintptr_t)&bhkThreadMemorySource::init);

				// Reducing performance, it looks like Bethesda has created something wonderful this time
				// 
				// ScrapHeap, HeapAllocator, SharedHeapAllocator and etc.
				//Detours::DetourJump(__CKPE_OFFSET(5), (std::uintptr_t)&HeapAllocator::Allocate);
				//Detours::DetourJump(__CKPE_OFFSET(6), (std::uintptr_t)&HeapAllocator::Deallocate);
				//Detours::DetourJump(__CKPE_OFFSET(7), (std::uintptr_t)&HeapAllocator::Size);
				//Detours::DetourJump(__CKPE_OFFSET(8), (std::uintptr_t)&HeapAllocator::BlockSize);
				// 
				//SafeWrite::Write(__CKPE_OFFSET(9), { 0xC3 });
				// This condition causes a leak, as it blocks the release of memory, perhaps this is due to interception.
				// Solution: delete the condition.
				//SafeWrite::WriteNop(__CKPE_OFFSET(10), 2);

				return true;
			}

			bool MemoryManager::LowMemory() noexcept(true)
			{
				return HardwareInfo::OS::GetSharedMemory().GetFreeByUint() > MEM_THRESHOLD;
			}
		}
	}
}