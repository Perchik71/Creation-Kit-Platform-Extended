// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <EditorAPI/B/BGSEntryPoint.h>
#include <EditorAPI/B/BGSEntryPointFunctionData.h>

namespace CKPE::Fallout4::EditorAPI
{
	class TESFile;
}

namespace CKPE::Fallout4::EditorAPI::Forms
{
	class TESQuest;
	class TESCondition;

	class BGSPerkEntry
	{
	protected:
		virtual void sub_000() = 0;
		virtual void sub_008() = 0;
		virtual void sub_010() = 0;

	public:
		// The editor destructor is slot 3, not slot 0 as in the game.
		virtual ~BGSPerkEntry() = default;
		[[nodiscard]] virtual std::uint32_t GetType() const = 0; // 04

	protected:
		virtual void sub_028() = 0;
		virtual void sub_030() = 0;
		virtual void sub_038() = 0;
		virtual void sub_040() = 0;
		virtual void sub_048() = 0;
		virtual void sub_050() = 0;

	public:
		virtual bool Load(TESFile* File) = 0; // 11

	protected:
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
		virtual void sub_0C0() = 0;
		virtual void sub_0C8() = 0;

	public:
		/* 0x008 */ std::uint8_t rank;
		/* 0x009 */ std::uint8_t priority;
		/* 0x00A */ std::uint8_t pad0A[0x6];
	};
	static_assert(sizeof(BGSPerkEntry) == 0x10);
	static_assert(offsetof(BGSPerkEntry, rank) == 0x8);
	static_assert(offsetof(BGSPerkEntry, priority) == 0x9);

	class BGSQuestPerkEntry : public BGSPerkEntry
	{
	public:
		/* 0x010 */ TESQuest* quest;
		/* 0x018 */ std::uint16_t stage;
		/* 0x01A */ std::uint8_t pad1A[0x6];
	};
	static_assert(sizeof(BGSQuestPerkEntry) == 0x20);
	static_assert(offsetof(BGSQuestPerkEntry, stage) == 0x18);

	class BGSAbilityPerkEntry : public BGSPerkEntry
	{
	public:
		/* 0x010 */ SpellItem* ability;
	};
	static_assert(sizeof(BGSAbilityPerkEntry) == 0x18);
	static_assert(offsetof(BGSAbilityPerkEntry, ability) == 0x10);

	class BGSEntryPointPerkEntry : public BGSPerkEntry
	{
	public:
		/* 0x010 */ BGSEntryPoint entryPoint;
		/* 0x011 */ std::uint8_t function;
		/* 0x012 */ std::uint8_t conditionCount;
		/* 0x013 */ std::uint8_t pad13[0x5];
		/* 0x018 */ BGSEntryPointFunctionData* functionData;
		/* 0x020 */ TESCondition* conditions;
		/* 0x028 */ std::uint64_t unk28;
		/* 0x030 */ void* unk30;
	};
	static_assert(sizeof(BGSEntryPointPerkEntry) == 0x38);
	static_assert(offsetof(BGSEntryPointPerkEntry, functionData) == 0x18);
	static_assert(offsetof(BGSEntryPointPerkEntry, conditions) == 0x20);
}
