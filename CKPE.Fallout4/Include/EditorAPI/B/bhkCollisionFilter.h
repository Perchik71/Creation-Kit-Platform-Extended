// Copyright © 2020-2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/H/hkBaseObject.h>
#include <EditorAPI/H/hkpCollisionFilter.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hknpCollisionFilter : public hkReferencedObject
	{
	public:
		/* 04 */ virtual void UnkNPCollisionFilter04();
		/* 05 */ virtual void UnkNPCollisionFilter05();
		/* 06 */ virtual void UnkNPCollisionFilter06();
		/* 07 */ virtual void UnkNPCollisionFilter07();
		/* 08 */ virtual void UnkNPCollisionFilter08();

		/* 0x010 */ std::uint8_t unk010[0x8];
	};
	static_assert(sizeof(hknpCollisionFilter) == 0x18);

	class bhkCollisionFilter : public hknpCollisionFilter
	{
	public:
		/* 0x018 */ std::uint8_t unk018[0x188];
		/* 0x1A0 */ std::uint64_t layerMasks[64];
		/* 0x3A0 */ std::uint64_t layerFlagMasks[2];
		/* 0x3B0 */ BSFixedString layerNames[64];
		/* 0x5B0 */ std::uint8_t unk5B0[0x100];
		/* 0x6B0 */ BSFixedString bodyPartNames[32];
	};
	static_assert(sizeof(bhkCollisionFilter) == 0x7B0);
	static_assert(offsetof(bhkCollisionFilter, layerMasks) == 0x1A0);
	static_assert(offsetof(bhkCollisionFilter, layerFlagMasks) == 0x3A0);
	static_assert(offsetof(bhkCollisionFilter, layerNames) == 0x3B0);
	static_assert(offsetof(bhkCollisionFilter, bodyPartNames) == 0x6B0);

	class bhkMigrationCollisionFilter : public hkpCollisionFilter
	{
	public:
		/* 0x048 */ std::uint8_t unk048[0x190];
		/* 0x1D8 */ std::uint64_t layerMasks[64];
		/* 0x3D8 */ std::uint8_t unk3D8[0x210];
		/* 0x5E8 */ BSFixedString layerNames[64];
		/* 0x7E8 */ std::uint8_t unk7E8[0x100];
		/* 0x8E8 */ BSFixedString bodyPartNames[32];
	};
	static_assert(sizeof(bhkMigrationCollisionFilter) == 0x9E8);
	static_assert(offsetof(bhkMigrationCollisionFilter, layerMasks) == 0x1D8);
	static_assert(offsetof(bhkMigrationCollisionFilter, layerNames) == 0x5E8);
	static_assert(offsetof(bhkMigrationCollisionFilter, bodyPartNames) == 0x8E8);
}
