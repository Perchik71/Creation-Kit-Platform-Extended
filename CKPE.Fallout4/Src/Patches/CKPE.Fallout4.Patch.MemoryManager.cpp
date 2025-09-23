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
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.MemoryManager.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			constexpr auto MEM_THRESHOLD = 2;	// 2Gb

			class BSMemoryManager
			{
				// Не описываем конструкторы и деструкторы
				// Класс - это просто оболочка
			public:
				static void* Allocate(BSMemoryManager* manager, std::size_t size, std::uint32_t alignment, bool aligned)
				{
					auto ptr = Common::MemoryManager::GetSingleton()->MemAlloc(size, alignment, aligned, true);
					// _CKPE_TracerPush("MemoryManager", ptr, size);
					return ptr;
				}

				static void Deallocate(BSMemoryManager* manager, void* memory, bool aligned)
				{
					//_CKPE_TracerPop(memory);
					Common::MemoryManager::GetSingleton()->MemFree(memory);
				}

				static std::size_t Size(BSMemoryManager* manager, void* memory)
				{
					return Common::MemoryManager::GetSingleton()->MemSize(memory);
				}
			};

			class BSScrapHeap
			{
				// Не описываем конструкторы и деструкторы
				// Класс - это просто оболочка
			public:
				static void* Allocate(BSScrapHeap* manager, std::size_t size, std::uint32_t alignment)
				{
					auto ptr = Common::MemoryManager::GetSingleton()->MemAlloc(size, alignment, alignment != 0);
					//_CKPE_TracerPush("ScrapHeap", ptr, size);
					return ptr;
				}

				static void Deallocate(BSScrapHeap* manager, void* memory)
				{
					//_CKPE_TracerPop(memory);
					Common::MemoryManager::GetSingleton()->MemFree(memory);
				}
			};

			class bhkThreadMemorySource
			{
			private:
				char _pad0[0x8];
				CRITICAL_SECTION m_CritSec;
			public:
				CKPE_DECLARE_CONSTRUCTOR_HOOK(bhkThreadMemorySource);

				bhkThreadMemorySource();
				virtual ~bhkThreadMemorySource();
				virtual void* blockAlloc(std::size_t numBytes);
				virtual void blockFree(void* p, std::size_t numBytes);
				virtual void* bufAlloc(std::size_t& reqNumBytesInOut);
				virtual void bufFree(void* p, std::size_t numBytes);
				virtual void* bufRealloc(void* pold, std::size_t oldNumBytes, std::size_t& reqNumBytesInOut);
				virtual void blockAllocBatch(void** ptrsOut, std::size_t numPtrs, std::size_t blockSize);
				virtual void blockFreeBatch(void** ptrsIn, std::size_t numPtrs, std::size_t blockSize);
				virtual void getMemoryStatistics(class MemoryStatistics& u);
				virtual std::size_t getAllocatedSize(const void* obj, std::size_t nbytes);
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

			void* bhkThreadMemorySource::blockAlloc(std::size_t numBytes)
			{
				return BSMemoryManager::Allocate(nullptr, numBytes, 16, true);
			}

			void bhkThreadMemorySource::blockFree(void* p, std::size_t numBytes)
			{
				BSMemoryManager::Deallocate(nullptr, p, true);
			}

			void* bhkThreadMemorySource::bufAlloc(std::size_t& reqNumBytesInOut)
			{
				return blockAlloc(reqNumBytesInOut);
			}

			void bhkThreadMemorySource::bufFree(void* p, std::size_t numBytes)
			{
				return blockFree(p, numBytes);
			}

			void* bhkThreadMemorySource::bufRealloc(void* pold, std::size_t oldNumBytes, std::size_t& reqNumBytesInOut)
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
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool MemoryManager::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 2)
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
				Detours::DetourJump(__CKPE_OFFSET(3), (std::uintptr_t)&BSScrapHeap::Allocate);
				Detours::DetourJump(__CKPE_OFFSET(4), (std::uintptr_t)&BSScrapHeap::Deallocate);
				Detours::DetourJump(__CKPE_OFFSET(5), (std::uintptr_t)&bhkThreadMemorySource::__ctor__);
				
				SafeWrite::Write(__CKPE_OFFSET(6), { 0xC3 });
				SafeWrite::Write(__CKPE_OFFSET(7), { 0xC3 });
				SafeWrite::Write(__CKPE_OFFSET(8), { 0xC3 });
				SafeWrite::Write(__CKPE_OFFSET(9), { 0xC3 });

				return true;
			}

			bool MemoryManager::LowMemory() noexcept(true)
			{
				return HardwareInfo::OS::GetSharedMemory().GetFreeByUint() > MEM_THRESHOLD;
			}
		}
	}
}