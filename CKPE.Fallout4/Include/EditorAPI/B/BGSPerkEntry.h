// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <windows.h>
#include <EditorAPI/B/BSString.h>
#include <EditorAPI/C/CheckInFormList.h>
#include <EditorAPI/B/BGSEntryPoint.h>
#include <EditorAPI/B/BGSEntryPointFunctionData.h>

namespace CKPE::Fallout4::EditorAPI
{
	class TESFile;
}

namespace CKPE::Fallout4::EditorAPI::Forms
{
	class TESQuest;
	class TESForm;
	class TESCondition;

	class BGSPerkEntry
	{
	protected:
		// 0x000-0x010: return 0 in the base and the quest/ability entries (the entry point entry overrides them); not identified
		virtual void sub_000() = 0;
		virtual void sub_008() = 0;
		virtual void sub_010() = 0;

	public:
		// The editor destructor is slot 3, not slot 0 as in the game.
		virtual ~BGSPerkEntry() = default;
		[[nodiscard]] virtual std::uint32_t GetType() const = 0; // 04

	protected:
		virtual void sub_028() = 0; // 05 - no-op in the base, quest and ability entries

	public:
		// Resolves the form ids read by Load into pointers through the owning perk (the quest entry looks the quest up and registers
		// the owner as a reference holder).
		virtual void ResolveReferences(TESForm* Owner) = 0; // 06
		// Allocates a new entry of the same type and copies this one into it
		[[nodiscard]] virtual BGSPerkEntry* Clone() const = 0; // 07
		// Copies the shared header: rank and priority
		virtual void CopyBaseData(const BGSPerkEntry* Source) = 0; // 08
		[[nodiscard]] virtual bool IsDifferent(const BGSPerkEntry* Other) const = 0; // 09
		virtual void Save() = 0; // 0A - writes the entry's chunks
		virtual bool Load(TESFile* File) = 0; // 0B

	protected:
		virtual void sub_060() = 0; // 0C
		virtual void sub_068() = 0; // 0D
		virtual void sub_070() = 0; // 0E - returns 0xFFFF in the base and quest entries
		virtual void sub_078() = 0; // 0F

	public:
		// Dialog box handling of the perk entry editor (the quest entry fills controls 2158 and 2446)
		virtual void UpdateDataInDialogBox(HWND hDialogBox) = 0; // 10
		virtual void UpdateDataFromDialogBox(HWND hDialogBox) = 0; // 11

	protected:
		virtual void sub_090() = 0; // 12 - no-op in the quest entry
		virtual void sub_098() = 0; // 13 - formats a value for the entry list according to a format id

	public:
		virtual void EnableDialogControls(HWND hDialogBox, bool bEnable) = 0; // 14

	protected:
		virtual void sub_0A8() = 0; // 15 - releases a reference counted object (count at +30)
		virtual void sub_0B0() = 0; // 16 - no-op in the base, quest and ability entries

	public:
		// The entry is acceptable for check-in when its referenced form (if any) passes TESForm::CanReferenceDuringCheckIn
		[[nodiscard]] virtual bool CanCheckIn(const CheckInFormList* Forms) const = 0; // 17
		virtual void RemoveReferencesToForm(TESForm* Form) = 0; // 18
		virtual void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const = 0; // 19

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
