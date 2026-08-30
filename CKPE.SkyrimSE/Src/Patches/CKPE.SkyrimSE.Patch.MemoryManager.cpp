// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Asserts.h>
#include <CKPE.HardwareInfo.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.MemoryManager.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.MemoryManager.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			constexpr auto MEM_THRESHOLD = 2;	// 2Gb

			class BSMemoryManager
			{
				// Не описываем конструкторы и деструкторы
				// Класс - это просто оболочка
			public:
				static void* Allocate([[maybe_unused]] BSMemoryManager* manager, std::size_t size, std::uint32_t alignment, bool aligned)
				{
					auto ptr = Common::MemoryManager::GetSingleton()->MemAlloc(size, alignment, aligned, true);
					// _CKPE_TracerPush("MemoryManager", ptr, size);
					return ptr;
				}

				static void Deallocate([[maybe_unused]] BSMemoryManager* manager, void* memory, [[maybe_unused]] bool aligned)
				{
					//_CKPE_TracerPop(memory);
					Common::MemoryManager::GetSingleton()->MemFree(memory);
				}

				static std::size_t Size([[maybe_unused]] BSMemoryManager* manager, void* memory)
				{
					return Common::MemoryManager::GetSingleton()->MemSize(memory);
				}
			};

			class BSScrapHeap
			{
				// Не описываем конструкторы и деструкторы
				// Класс - это просто оболочка
			public:
				static void* Allocate([[maybe_unused]] BSScrapHeap* manager, std::size_t size, std::uint32_t alignment)
				{
					auto ptr = Common::MemoryManager::GetSingleton()->MemAlloc(size, alignment, alignment != 0);
					//_CKPE_TracerPush("ScrapHeap", ptr, size);
					return ptr;
				}

				static void Deallocate([[maybe_unused]] BSScrapHeap* manager, void* memory)
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

			bool MemoryManager::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool MemoryManager::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool MemoryManager::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				auto base = Application::GetSingleton()->GetBase();
				auto Physical = HardwareInfo::OS::GetPhysicalMemory();
				auto Shared = HardwareInfo::OS::GetSharedMemory();
				_MESSAGE("\t\tPhysical Memory (Total: %.1f Gb, Available: %.1f Gb)"sv, Physical.Total, Physical.Free);
				_MESSAGE("\t\tMemory (Total: %.1f Gb, Available: %.1f Gb)"sv, Shared.Total, Shared.Free);

				// Программа очень любит думать, а винде это не нравиться, скажем винде, чтоб не обращала внимание.
				DisableProcessWindowsGhosting();

				auto patchIAT = [&base](const char* hmodule)
					{
						Detours::DetourIAT(base, hmodule, "malloc", (std::uintptr_t)malloc);
						Detours::DetourIAT(base, hmodule, "calloc", (std::uintptr_t)calloc);
						Detours::DetourIAT(base, hmodule, "realloc", (std::uintptr_t)realloc);
						Detours::DetourIAT(base, hmodule, "_recalloc", (std::uintptr_t)recalloc);
						Detours::DetourIAT(base, hmodule, "free", (std::uintptr_t)free);
						Detours::DetourIAT(base, hmodule, "_msize", (std::uintptr_t)msize);
						Detours::DetourIAT(base, hmodule, "_strdup", (std::uintptr_t)strdup);
						Detours::DetourIAT(base, hmodule, "memcpy_s", (std::uintptr_t)memcpy_s);
						Detours::DetourIAT(base, hmodule, "memmove_s", (std::uintptr_t)memmove_s);
						Detours::DetourIAT(base, hmodule, "memcmp", (std::uintptr_t)memcmp);
						Detours::DetourIAT(base, hmodule, "memcpy", (std::uintptr_t)memcpy);
						Detours::DetourIAT(base, hmodule, "memmove", (std::uintptr_t)memmove);
						Detours::DetourIAT(base, hmodule, "memset", (std::uintptr_t)memset);
					};

				patchIAT("API-MS-WIN-CRT-HEAP-L1-1-0.DLL");
				patchIAT("MSVCR110.DLL");

				// Принудительный вылет с сообщением для пользователя.
				CKPE_ASSERT_MSG(LowMemory(), "Not enough memory to run the program");

				Relocation(ID(105314)).WriteJump(&BSMemoryManager::Allocate);
				Relocation(ID(253947)).WriteJump(&BSMemoryManager::Deallocate);
				Relocation(ID(657554)).WriteJump(&BSMemoryManager::Size);
				Relocation(ID(317676)).WriteJump(&BSScrapHeap::Allocate);
				Relocation(ID{ 757576, 974040 }).WriteJump(&BSScrapHeap::Deallocate);
				Relocation(ID{ 758593, 948874 }).WriteJump(&bhkThreadMemorySource::__ctor__);	
				Relocation(ID(28172)).Write(RET);
				Relocation(ID{ 599362, 1025020 }).Write(RET);
				Relocation(ID{ 757575, 1016911 }).Write(RET);
				Relocation(ID(605622)).Write(RET);

				return true;
			}

			bool MemoryManager::LowMemory() noexcept(true)
			{
				return HardwareInfo::OS::GetSharedMemory().GetFreeByUint() > MEM_THRESHOLD;
			}
		}
	}
}