// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <windows.h>
#include <EditorAPI/B/BSString.h>
#include <EditorAPI/C/CheckInFormList.h>
#include <EditorAPI/B/BSFixedString.h>

namespace CKPE::Fallout4::EditorAPI
{
	class TESFile;
}

namespace CKPE::Fallout4::EditorAPI::Forms
{
	class TESQuest;
	class TESForm;
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

	public:
		// Copies another alias of the same type (checked through QType)
		virtual void CopyFrom(const BGSBaseAlias* Source) = 0; // 01
		[[nodiscard]] virtual bool IsDifferent(const BGSBaseAlias* Other) const = 0; // 02
		virtual void Save() const = 0; // 03 - writes the alias chunks
		virtual bool Load(TESFile* File) = 0; // 04
		virtual void InitItem() = 0; // 05
		[[nodiscard]] virtual const BSFixedString& QType() const = 0; // 06

	protected:
		virtual void sub_038() = 0; // 07 - location alias: tells the referenced forms to track the owner (virtual 75 of TESForm, one form without arguments)
		virtual void sub_040() = 0; // 08 - location alias: registers the owner on the referenced forms

	public:
		virtual void RemoveReferencesToForm(TESForm* Form) = 0; // 09
		[[nodiscard]] virtual bool CanCheckIn(const CheckInFormList* Forms, TESForm* ReferencingForm) const = 0; // 0A
		virtual void BuildCheckInDependencyMessage(const CheckInFormList* Forms, TESForm* ReferencingForm, BSString& Message) const = 0; // 0B
		// Dialog template resource id of the alias editor page (3364 for location aliases)
		[[nodiscard]] virtual std::int32_t GetDialogTemplateID() const = 0; // 0C

	protected:
		virtual void sub_068() = 0; // 0D - formats a text for the alias list by column id
		virtual void sub_070() = 0; // 0E - builds the alias summary text

	public:
		// Allocates a new alias of the same type and copies this one into it
		[[nodiscard]] virtual BGSBaseAlias* Clone() const = 0; // 0F

	protected:
		virtual void sub_080() = 0; // 10 - fills the alias editor dialog (also takes a second argument)

	public:
		virtual void UpdateDataFromDialogBox(HWND hDialogBox) = 0; // 11

	protected:
		virtual void sub_090() = 0; // 12 - releases the alias editor dialog data
		virtual void sub_098() = 0; // 13 - alias editor dialog message handler (WM_COMMAND etc.)

	public:
		// Appends the flag letters of the alias (R, S, ...) to the text
		virtual void BuildFlagsText(BSString& Text) const = 0; // 14

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
		virtual void CopyFrom(const BGSBaseAlias* Source) override;
		[[nodiscard]] virtual bool IsDifferent(const BGSBaseAlias* Other) const override;
		virtual void Save() const override;
		virtual bool Load(TESFile* File) override;
		virtual void InitItem() override;
		[[nodiscard]] virtual const BSFixedString& QType() const override;
		virtual void RemoveReferencesToForm(TESForm* Form) override;
		[[nodiscard]] virtual bool CanCheckIn(const CheckInFormList* Forms, TESForm* ReferencingForm) const override;
		virtual void BuildCheckInDependencyMessage(const CheckInFormList* Forms, TESForm* ReferencingForm, BSString& Message) const override;
		[[nodiscard]] virtual std::int32_t GetDialogTemplateID() const override;
		[[nodiscard]] virtual BGSBaseAlias* Clone() const override;
		virtual void UpdateDataFromDialogBox(HWND hDialogBox) override;
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
		virtual void CopyFrom(const BGSBaseAlias* Source) override;
		[[nodiscard]] virtual bool IsDifferent(const BGSBaseAlias* Other) const override;
		virtual void Save() const override;
		virtual bool Load(TESFile* File) override;
		virtual void InitItem() override;
		[[nodiscard]] virtual const BSFixedString& QType() const override;
		virtual void RemoveReferencesToForm(TESForm* Form) override;
		[[nodiscard]] virtual bool CanCheckIn(const CheckInFormList* Forms, TESForm* ReferencingForm) const override;
		virtual void BuildCheckInDependencyMessage(const CheckInFormList* Forms, TESForm* ReferencingForm, BSString& Message) const override;
		[[nodiscard]] virtual std::int32_t GetDialogTemplateID() const override;
		[[nodiscard]] virtual BGSBaseAlias* Clone() const override;
		virtual void UpdateDataFromDialogBox(HWND hDialogBox) override;
		virtual void BuildFlagsText(BSString& Text) const override;
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
		virtual void CopyFrom(const BGSBaseAlias* Source) override;
		[[nodiscard]] virtual bool IsDifferent(const BGSBaseAlias* Other) const override;
		virtual void Save() const override;
		virtual bool Load(TESFile* File) override;
		[[nodiscard]] virtual const BSFixedString& QType() const override;
		[[nodiscard]] virtual std::int32_t GetDialogTemplateID() const override;
		[[nodiscard]] virtual BGSBaseAlias* Clone() const override;
		virtual void UpdateDataFromDialogBox(HWND hDialogBox) override;
	public:
		/* 0x098 */ std::uint8_t maxInitialCount;
		/* 0x099 */ std::uint8_t pad99[0x7];
	};
	static_assert(sizeof(BGSRefCollectionAlias) == 0xA0);
	static_assert(offsetof(BGSRefCollectionAlias, maxInitialCount) == 0x98);
}
