// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <EditorAPI/B/BSFixedString.h>

namespace CKPE::Fallout4::EditorAPI
{
	class TESFile;
}

namespace CKPE::Fallout4::EditorAPI::Forms
{
	class TESQuest;
	class BGSLocation;
	class BGSKeyword;
	class TESCondition;

	class BGSBaseAlias
	{
	public:
		enum Flag : std::uint32_t
		{
			Optional = 1u << 1,
			AllowDisabled = 1u << 7,
			AllowReserved = 1u << 9
		};

		virtual ~BGSBaseAlias() = default; // 00

	protected:
		// Unresolved editor-only signatures must not be called through these slots.
		virtual void sub_008() = 0;
		virtual void sub_010() = 0;
		virtual void sub_018() = 0;

	public:
		virtual bool Load(TESFile* File) = 0; // 04
		virtual void InitItem() = 0; // 05
		[[nodiscard]] virtual const BSFixedString& QType() const = 0; // 06

	protected:
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

	public:
		[[nodiscard]] virtual TESQuest* QExternalQuest() const = 0; // 21
		virtual void ClearExternalQuest() = 0; // 22

		/* 0x008 */ BSFixedString aliasName;
		/* 0x010 */ TESQuest* owningQuest;
		/* 0x018 */ std::uint32_t aliasID;
		/* 0x01C */ std::uint32_t flags;
		/* 0x020 */ std::uint16_t fillType;
		/* 0x022 */ std::uint8_t pad22[0x6];
	};
	static_assert(sizeof(BGSBaseAlias) == 0x28);
	static_assert(offsetof(BGSBaseAlias, aliasName) == 0x8);
	static_assert(offsetof(BGSBaseAlias, aliasID) == 0x18);
	static_assert(offsetof(BGSBaseAlias, fillType) == 0x20);

	class BGSLocAlias : public BGSBaseAlias
	{
	public:
		enum class FillType : std::uint16_t
		{
			Conditions = 0,
			Forced = 1,
			FromAlias = 2,
			FromEvent = 3,
			FromExternal = 4
		};

		/* 0x028 */ BGSLocation* forcedLoc;
		/* 0x030 */ BGSKeyword* forcedFromAliasFilter;
		/* 0x038 */ TESQuest* externalQuest;
		/* 0x040 */ TESCondition* conditions;
		/* 0x048 */ std::uint32_t forcedFromAlias;
		/* 0x04C */ std::uint32_t forcedFromEvent;
		/* 0x050 */ std::uint32_t forcedEventData;
		/* 0x054 */ std::uint32_t externalAlias;
		/* 0x058 */ std::uint32_t closestToAlias;
		/* 0x05C */ std::uint8_t pad5C[0x4];
	};
	static_assert(sizeof(BGSLocAlias) == 0x60);
	static_assert(offsetof(BGSLocAlias, externalQuest) == 0x38);
	static_assert(offsetof(BGSLocAlias, conditions) == 0x40);

	class BGSRefAlias : public BGSBaseAlias
	{
	public:
		enum class FillType : std::uint16_t
		{
			Conditions = 0,
			Forced = 1,
			FromAlias = 2,
			FromEvent = 3,
			Created = 4,
			FromExternal = 5,
			UniqueActor = 6,
			NearAlias = 7
		};

		/* 0x028 */ std::uint64_t fillData[3];
		/* 0x040 */ TESCondition* conditions;
		/* 0x048 */ std::uint8_t unk48[0x50];
	};
	static_assert(sizeof(BGSRefAlias) == 0x98);
	static_assert(offsetof(BGSRefAlias, fillData) == 0x28);
	static_assert(offsetof(BGSRefAlias, conditions) == 0x40);

	class BGSRefCollectionAlias : public BGSRefAlias
	{
	public:
		/* 0x098 */ std::uint8_t maxInitialCount;
		/* 0x099 */ std::uint8_t pad99[0x7];
	};
	static_assert(sizeof(BGSRefCollectionAlias) == 0xA0);
	static_assert(offsetof(BGSRefCollectionAlias, maxInitialCount) == 0x98);
}
