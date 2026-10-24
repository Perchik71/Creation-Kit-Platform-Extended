// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <EditorAPI/N/NiPoint.h>

namespace CKPE::Fallout4::EditorAPI
{
	namespace Forms
	{
		class TESObjectCELL;
		class TESWorldSpace;
	}

	class IAIWorldLocation
	{
	public:
		virtual ~IAIWorldLocation() = default;

	protected:
		virtual void sub_008() = 0;
		virtual void sub_010() = 0;
		virtual void sub_018() = 0;
		virtual void sub_020() = 0;
		virtual void sub_028() = 0;
		virtual void sub_030() = 0;
		virtual void sub_038() = 0;
		virtual void sub_040() = 0;
		virtual void sub_048() = 0;
		virtual void sub_050() = 0;
		virtual void sub_058() = 0;
		virtual void sub_060() = 0;
		virtual void sub_068() = 0;
		virtual void sub_070() = 0;
		virtual void sub_078() = 0;
		virtual void sub_080() = 0;
		virtual void sub_088() = 0;
		virtual void sub_090() = 0;
		virtual void sub_098() = 0;
		virtual void sub_0A0() = 0;
		virtual void sub_0A8() = 0;
		virtual void sub_0B0() = 0;
		virtual void sub_0B8() = 0;
	};
	static_assert(sizeof(IAIWorldLocation) == 0x8);

	class BGSAIWorldLocation : public IAIWorldLocation
	{};
	static_assert(sizeof(BGSAIWorldLocation) == 0x8);

	class BGSAIWorldLocationInteriorCell : public BGSAIWorldLocation
	{
	public:
		/* 0x008 */ Forms::TESObjectCELL* cell;
		/* 0x010 */ NiAPI::NiPoint3 position;
		/* 0x01C */ std::uint8_t pad1C[0x4];
	};
	static_assert(sizeof(BGSAIWorldLocationInteriorCell) == 0x20);
	static_assert(offsetof(BGSAIWorldLocationInteriorCell, position) == 0x10);

	class BGSAIWorldLocationPointRadius : public BGSAIWorldLocation
	{
	public:
		/* 0x008 */ NiAPI::NiPoint3 position;
		/* 0x014 */ std::uint8_t pad14[0x4];
		/* 0x018 */ Forms::TESObjectCELL* cell;
		/* 0x020 */ Forms::TESWorldSpace* worldSpace;
		/* 0x028 */ float radius;
		/* 0x02C */ float unk2C;
		/* 0x030 */ bool unk30;
		/* 0x031 */ bool unk31;
		/* 0x032 */ std::uint8_t pad32[0x6];
	};
	static_assert(sizeof(BGSAIWorldLocationPointRadius) == 0x38);
	static_assert(offsetof(BGSAIWorldLocationPointRadius, radius) == 0x28);

	class BGSAIWorldLocationRefRadius : public BGSAIWorldLocation
	{
	public:
		/* 0x008 */ std::uint32_t reference;
		/* 0x00C */ float radius;
		/* 0x010 */ std::uint32_t unk10;
		/* 0x014 */ float unk14;
		/* 0x018 */ bool unk18;
		/* 0x019 */ std::uint8_t pad19[0x7];
	};
	static_assert(sizeof(BGSAIWorldLocationRefRadius) == 0x20);
	static_assert(offsetof(BGSAIWorldLocationRefRadius, radius) == 0xC);

	class BGSAIWorldLocationPrimitive : public BGSAIWorldLocation
	{
	public:
		/* 0x008 */ std::uint32_t reference;
		/* 0x00C */ std::uint8_t unk0C[0xC];
		/* 0x018 */ NiAPI::NiPoint3 position;
		/* 0x024 */ std::uint8_t unk24[0x5C];
	};
	static_assert(sizeof(BGSAIWorldLocationPrimitive) == 0x80);
	static_assert(offsetof(BGSAIWorldLocationPrimitive, position) == 0x18);
}
