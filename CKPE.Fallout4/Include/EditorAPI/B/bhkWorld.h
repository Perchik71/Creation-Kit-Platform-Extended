// Copyright © 2020-2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <EditorAPI/B/bhkRefObject.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hknpBSWorld;
	class hclBSWorld;
	class hknpWorldCinfo;

	class bhkWorld : public NiAPI::NiObject
	{
	public:
		/* 2A */ virtual bool Update(std::uint32_t UpdateFlags);
		/* 2B */ virtual void Init(const hknpWorldCinfo& Info);

		/* 0x010 */ std::uint8_t unk010[0x40];
		/* 0x050 */ alignas(16) float origin[4];
		/* 0x060 */ hknpBSWorld* worldNP;
		/* 0x068 */ std::uint8_t unk068[0xF0];
		/* 0x158 */ hclBSWorld* clothWorld;
		/* 0x160 */ void* clothStepBuffer;
		/* 0x168 */ std::int32_t clothStepBufferSize;
		/* 0x16C */ std::uint32_t unk16C;
		/* 0x170 */ std::uint8_t unk170[0x20];
	};
	static_assert(sizeof(bhkWorld) == 0x190);
	static_assert(offsetof(bhkWorld, origin) == 0x50);
	static_assert(offsetof(bhkWorld, worldNP) == 0x60);
	static_assert(offsetof(bhkWorld, clothWorld) == 0x158);
	static_assert(offsetof(bhkWorld, clothStepBuffer) == 0x160);
	static_assert(offsetof(bhkWorld, clothStepBufferSize) == 0x168);

	class bhkMigrationWorld : public bhkRefObject
	{
	public:
		/* 38 */ virtual void UnkMigrationWorld38();
		/* 39 */ virtual void UnkMigrationWorld39();

		/* 0x020 */ alignas(16) std::uint8_t unk020[0xC5A0];
	};
	static_assert(sizeof(bhkMigrationWorld) == 0xC5C0);
}
