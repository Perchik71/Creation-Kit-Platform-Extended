// Copyright © 2020-2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <windows.h>
#include <EditorAPI/H/hkMemory.h>
#include <EditorAPI/I/IMemoryHeap.h>

namespace CKPE::Fallout4::EditorAPI
{
	class bhkThreadMemorySource : public hkMemoryAllocator, public IMemoryHeap
	{
	public:
		/* 0x010 */ CRITICAL_SECTION criticalSection;
		/* 0x038 */ std::uint8_t unk038[0x48];
		/* 0x080 */ void* reservedAddress;
		/* 0x088 */ void* alignedAddress;
		/* 0x090 */ std::uint8_t unk090[0x8];
		/* 0x098 */ void* committedBegin;
		/* 0x0A0 */ void* committedEnd;
		/* 0x0A8 */ std::uint8_t unk0A8[0x8];
		/* 0x0B0 */ void* reservedEnd;
		/* 0x0B8 */ std::uint8_t unk0B8[0x10];
		/* 0x0C8 */ std::uint32_t reservedSize;
		/* 0x0CC */ std::uint8_t unk0CC[0x1C];

		[[nodiscard]] inline CRITICAL_SECTION* GetCriticalSection() noexcept(true)
		{
			return &criticalSection;
		}
	};
	static_assert(sizeof(bhkThreadMemorySource) == 0xE8);
	static_assert(offsetof(bhkThreadMemorySource, criticalSection) == 0x10);
	static_assert(offsetof(bhkThreadMemorySource, reservedAddress) == 0x80);
	static_assert(offsetof(bhkThreadMemorySource, alignedAddress) == 0x88);
	static_assert(offsetof(bhkThreadMemorySource, committedBegin) == 0x98);
	static_assert(offsetof(bhkThreadMemorySource, committedEnd) == 0xA0);
	static_assert(offsetof(bhkThreadMemorySource, reservedEnd) == 0xB0);
	static_assert(offsetof(bhkThreadMemorySource, reservedSize) == 0xC8);

	class bhkMemorySystem : public hkMemorySystem
	{
	public:
		/* 15 */ virtual void UnkMemorySystem15();

		/* 0x008 */ std::uint8_t unk008[0x8];
		/* 0x010 */ alignas(bhkThreadMemorySource)
			std::uint8_t threadMemorySourceStorage[sizeof(bhkThreadMemorySource)];
		/* 0x0F8 */ std::uint8_t unk0F8[0x478];
		/* 0x570 */ hkThreadMemory threadMemory;
		/* 0x728 */ std::uint8_t pad728[0x8];
		/* 0x730 */ hkMemoryRouter mainRouter;
		/* 0x7B0 */ std::int32_t solverBufferSize;
		/* 0x7B4 */ std::uint8_t unk7B4[0xC];

		[[nodiscard]] inline bhkThreadMemorySource* GetThreadMemorySource() noexcept(true)
		{
			return reinterpret_cast<bhkThreadMemorySource*>(threadMemorySourceStorage);
		}
	};
	static_assert(sizeof(bhkMemorySystem) == 0x7C0);
	static_assert(offsetof(bhkMemorySystem, threadMemorySourceStorage) == 0x10);
	static_assert(offsetof(bhkMemorySystem, threadMemory) == 0x570);
	static_assert(offsetof(bhkMemorySystem, mainRouter) == 0x730);
	static_assert(offsetof(bhkMemorySystem, solverBufferSize) == 0x7B0);
}
