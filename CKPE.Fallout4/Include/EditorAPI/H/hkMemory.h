// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>

namespace CKPE::Fallout4::EditorAPI
{
	class hkMemoryAllocator
	{
	public:
		class MemoryStatistics;
		class ExtendedInterface;

		virtual ~hkMemoryAllocator();
		virtual void* BlockAlloc(std::int32_t numBytes);
		virtual void BlockFree(void* block, std::int32_t numBytes);
		virtual void* BufAlloc(std::int32_t& numBytes);
		virtual void BufFree(void* block, std::int32_t numBytes);
		virtual void* BufRealloc(void* block, std::int32_t oldSize,
			std::int32_t& numBytes);
		virtual void BlockAllocBatch(void** blocks, std::int32_t count,
			std::int32_t blockSize);
		virtual void BlockFreeBatch(void** blocks, std::int32_t count,
			std::int32_t blockSize);
		virtual void GetMemoryStatistics(MemoryStatistics& statistics) const;
		virtual std::int32_t GetAllocatedSize(const void* block,
			std::int32_t numBytes) const;
		virtual void ResetPeakMemoryStatistics();
		virtual ExtendedInterface* GetExtendedInterface();
	};
	static_assert(sizeof(hkMemoryAllocator) == 0x8);

	class hkMemorySystem
	{
	public:
		virtual ~hkMemorySystem();
		virtual void Unk01();
		virtual void Unk02();
		virtual void Unk03();
		virtual void Unk04();
		virtual void Unk05();
		virtual void Unk06();
		virtual void Unk07();
		virtual void Unk08();
		virtual void Unk09();
		virtual void Unk0A();
		virtual void Unk0B();
		virtual void Unk0C();
		virtual void Unk0D();
		virtual void Unk0E();
		virtual void Unk0F();
		virtual void Unk10();
		virtual void Unk11();
		virtual void Unk12();
		virtual void Unk13();
		virtual void Unk14();
	};
	static_assert(sizeof(hkMemorySystem) == 0x8);

	class hkLifoAllocator : public hkMemoryAllocator
	{
	public:
		/* 0x008 */ std::byte pad008[0x8];
		/* 0x010 */ std::int32_t m_slabSize;
		/* 0x014 */ std::uint32_t pad014;
		/* 0x018 */ void* m_cur;
		/* 0x020 */ void* m_end;
		/* 0x028 */ std::byte pad028[0x28];
	};
	static_assert(sizeof(hkLifoAllocator) == 0x50);
	static_assert(offsetof(hkLifoAllocator, m_slabSize) == 0x10);
	static_assert(offsetof(hkLifoAllocator, m_cur) == 0x18);
	static_assert(offsetof(hkLifoAllocator, m_end) == 0x20);

	class hkThreadMemory : public hkMemoryAllocator
	{
	public:
		virtual void hkThreadMemory_Unk0C();

		/* 0x008 */ std::byte pad008[0x1B0];
	};
	static_assert(sizeof(hkThreadMemory) == 0x1B8);

	class hkMemoryRouter
	{
	public:
		/* 0x000 */ hkLifoAllocator m_stack;
		/* 0x050 */ std::byte pad050[0x8];
		/* 0x058 */ hkMemoryAllocator* m_heap;
		/* 0x060 */ std::byte pad060[0x20];
	};
	static_assert(sizeof(hkMemoryRouter) == 0x80);
	static_assert(offsetof(hkMemoryRouter, m_heap) == 0x58);

	class hkContainerHeapAllocator
	{
	public:
		class Allocator : public hkMemoryAllocator {};
		static_assert(sizeof(Allocator) == 0x8);
	};

	class hkContainerTempAllocator
	{
	public:
		class Allocator : public hkMemoryAllocator {};
		static_assert(sizeof(Allocator) == 0x8);
	};

	class hkContainerDebugAllocator
	{
	public:
		class Allocator : public hkMemoryAllocator {};
		static_assert(sizeof(Allocator) == 0x8);
	};

	class hkFreeListAllocator;
}
