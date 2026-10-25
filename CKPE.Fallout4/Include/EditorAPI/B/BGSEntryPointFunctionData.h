// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <windows.h>

namespace CKPE::Fallout4::EditorAPI
{
	class TESFile;
}

namespace CKPE::Fallout4::EditorAPI::Forms
{
	class ActorValueInfo;
	class TESLevItem;
	class SpellItem;

	class BGSEntryPointFunctionData
	{
	public:
		enum class Type : std::uint32_t
		{
			OneValue = 1,
			TwoValue = 2,
			LeveledList = 3,
			ActivateChoice = 4,
			SpellItem = 5,
			BooleanGraphVariable = 6,
			Text = 7,
			AVAndValue = 8
		};

		virtual ~BGSEntryPointFunctionData() = default; // 00
		[[nodiscard]] virtual Type GetType() const = 0; // 01

	protected:
		virtual void sub_010() = 0; // 02 - no-op for plain values; the form based types (leveled list, spell, actor value) override it
		virtual void sub_018() = 0; // 03 - no-op in most types
		virtual void sub_020() = 0; // 04 - returns 0 in most types

	public:
		// Dialog box handling of the perk entry editor (the one value type sets up a numeric edit control)
		virtual void InitDialogControls(HWND hDialogBox) = 0; // 05
		virtual void UpdateDataFromDialogBox(HWND hDialogBox) = 0; // 06
		// Formats the value for a list view; returns false when Format is not handled (the one value type handles format 5 as "%.02f")
		virtual bool FormatValueText(std::int32_t Format, char* Buffer) = 0; // 07

	protected:
		virtual void sub_040() = 0; // 08 - returns true for plain values
		virtual void sub_048() = 0; // 09 - no-op in most types
		virtual void sub_050() = 0; // 0A - no-op in most types

	public:
		// Allocates a new data object of the same type and copies this one into it
		[[nodiscard]] virtual BGSEntryPointFunctionData* Clone() const = 0; // 0B
		[[nodiscard]] virtual bool IsDifferent(const BGSEntryPointFunctionData* Other) const = 0; // 0C
		virtual void Save() const = 0; // 0D - writes the EPFD chunk
		virtual bool Load(TESFile* File) = 0; // 0E - reads the EPFD chunk, false when its size does not match the type

	public:
		/* 0x008 */ std::uint8_t unk08[0x10];
	};
	static_assert(sizeof(BGSEntryPointFunctionData) == 0x18);

	class BGSEntryPointFunctionDataOneValue : public BGSEntryPointFunctionData
	{
	public:
		/* 0x018 */ float value;
		/* 0x01C */ std::uint8_t pad1C[0x4];
	};
	static_assert(sizeof(BGSEntryPointFunctionDataOneValue) == 0x20);
	static_assert(offsetof(BGSEntryPointFunctionDataOneValue, value) == 0x18);

	class BGSEntryPointFunctionDataTwoValue : public BGSEntryPointFunctionData
	{
	public:
		/* 0x018 */ float values[2];
	};
	static_assert(sizeof(BGSEntryPointFunctionDataTwoValue) == 0x20);
	static_assert(offsetof(BGSEntryPointFunctionDataTwoValue, values) == 0x18);

	class BGSEntryPointFunctionDataAVAndValue : public BGSEntryPointFunctionData
	{
	public:
		/* 0x018 */ ActorValueInfo* actorValue;
		/* 0x020 */ float value;
		/* 0x024 */ std::uint8_t pad24[0x4];
	};
	static_assert(sizeof(BGSEntryPointFunctionDataAVAndValue) == 0x28);
	static_assert(offsetof(BGSEntryPointFunctionDataAVAndValue, value) == 0x20);

	class BGSEntryPointFunctionDataLeveledList : public BGSEntryPointFunctionData
	{
	public:
		/* 0x018 */ TESLevItem* leveledList;
	};
	static_assert(sizeof(BGSEntryPointFunctionDataLeveledList) == 0x20);
	static_assert(offsetof(BGSEntryPointFunctionDataLeveledList, leveledList) == 0x18);

	class BGSEntryPointFunctionDataSpellItem : public BGSEntryPointFunctionData
	{
	public:
		/* 0x018 */ SpellItem* spell;
	};
	static_assert(sizeof(BGSEntryPointFunctionDataSpellItem) == 0x20);
	static_assert(offsetof(BGSEntryPointFunctionDataSpellItem, spell) == 0x18);

	class BGSEntryPointFunctionDataBooleanGraphVariable : public BGSEntryPointFunctionData
	{
	public:
		/* 0x018 */ char* variableName;
		/* 0x020 */ std::uint8_t unk20[0x8];
	};
	static_assert(sizeof(BGSEntryPointFunctionDataBooleanGraphVariable) == 0x28);
	static_assert(offsetof(BGSEntryPointFunctionDataBooleanGraphVariable, variableName) == 0x18);

	class BGSEntryPointFunctionDataText : public BGSEntryPointFunctionData
	{
	public:
		/* 0x018 */ std::uint8_t unk18[0x10];
	};
	static_assert(sizeof(BGSEntryPointFunctionDataText) == 0x28);

	class BGSEntryPointFunctionDataActivateChoice : public BGSEntryPointFunctionData
	{
	public:
		/* 0x018 */ std::uint8_t unk18[0x10];
		/* 0x028 */ SpellItem* spell;
		/* 0x030 */ std::uint16_t flags;
		/* 0x032 */ std::uint8_t pad32[0x6];
	};
	static_assert(sizeof(BGSEntryPointFunctionDataActivateChoice) == 0x38);
	static_assert(offsetof(BGSEntryPointFunctionDataActivateChoice, spell) == 0x28);
	static_assert(offsetof(BGSEntryPointFunctionDataActivateChoice, flags) == 0x30);
}
