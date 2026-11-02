// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/E/ExtraDataTypes.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE::Fallout4::EditorAPI
{
	class ExtraAction : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraAction;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraAction[0];

		static constexpr auto TYPE{ kAction };
		ExtraAction() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint8_t unk18;
		/* 0x019 */ std::uint8_t unk19[0x07];
		/* 0x020 */ std::uint64_t unk20;
	};
	static_assert(sizeof(ExtraAction) == 0x28);
	static_assert(offsetof(ExtraAction, unk18) == 0x18);

	class ExtraHavok : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraHavok;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraHavok[0];

		static constexpr auto TYPE{ kHavok };
		ExtraHavok() = delete;
		/* 0x018 */ std::uint64_t unk18;
		/* 0x020 */ std::uint64_t unk20;
	};
	static_assert(sizeof(ExtraHavok) == 0x28);
	static_assert(offsetof(ExtraHavok, unk18) == 0x18);

	class ExtraRagDollData : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraRagDollData;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraRagDollData[0];

		static constexpr auto TYPE{ kRagdollData };
		ExtraRagDollData() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint64_t ragDollData;
	};
	static_assert(sizeof(ExtraRagDollData) == 0x20);
	static_assert(offsetof(ExtraRagDollData, ragDollData) == 0x18);

	class ExtraGlobal : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraGlobal;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraGlobal[0];

		static constexpr auto TYPE{ kGlobal };
		ExtraGlobal() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint64_t global;
	};
	static_assert(sizeof(ExtraGlobal) == 0x20);
	static_assert(offsetof(ExtraGlobal, global) == 0x18);

	class ExtraRank : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraRank;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraRank[0];

		static constexpr auto TYPE{ kRank };
		ExtraRank() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint32_t rank;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraRank) == 0x20);
	static_assert(offsetof(ExtraRank, rank) == 0x18);

	class ExtraTimeLeft : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraTimeLeft;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraTimeLeft[0];

		static constexpr auto TYPE{ kTimeLeft };
		ExtraTimeLeft() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ float timeLeft;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraTimeLeft) == 0x20);
	static_assert(offsetof(ExtraTimeLeft, timeLeft) == 0x18);

	class ExtraCharge : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraCharge;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraCharge[0];

		static constexpr auto TYPE{ kCharge };
		ExtraCharge() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ float charge;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraCharge) == 0x20);
	static_assert(offsetof(ExtraCharge, charge) == 0x18);

	class ExtraSeed : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraSeed;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraSeed[0];

		static constexpr auto TYPE{ kSeed };
		ExtraSeed() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint8_t seed;
		/* 0x019 */ std::uint8_t unk19[0x07];
	};
	static_assert(sizeof(ExtraSeed) == 0x20);
	static_assert(offsetof(ExtraSeed, seed) == 0x18);

	class ExtraEnchantment : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraEnchantment;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraEnchantment[0];

		static constexpr auto TYPE{ kEnchantment };
		ExtraEnchantment() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint16_t charge;
		/* 0x01A */ std::uint8_t removeOnUnequip;
		/* 0x01B */ std::uint8_t unk1B[0x05];
		/* 0x020 */ std::uint64_t enchantment;
	};
	static_assert(sizeof(ExtraEnchantment) == 0x28);
	static_assert(offsetof(ExtraEnchantment, charge) == 0x18);

	class ExtraSoul : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraSoul;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraSoul[0];

		static constexpr auto TYPE{ kSoul };
		ExtraSoul() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint8_t soul;
		/* 0x019 */ std::uint8_t unk19[0x07];
	};
	static_assert(sizeof(ExtraSoul) == 0x20);
	static_assert(offsetof(ExtraSoul, soul) == 0x18);

	class ExtraPoison : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraPoison;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraPoison[0];

		static constexpr auto TYPE{ kPoison };
		ExtraPoison() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint32_t count;
		/* 0x01C */ std::uint8_t unk1C[0x04];
		/* 0x020 */ std::uint64_t poison;
	};
	static_assert(sizeof(ExtraPoison) == 0x28);
	static_assert(offsetof(ExtraPoison, count) == 0x18);

	class ExtraCannotWear : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraCannotWear;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraCannotWear[0];

		static constexpr auto TYPE{ kCannotWear };
		ExtraCannotWear() = delete;
	};
	static_assert(sizeof(ExtraCannotWear) == 0x18);

	class ExtraShouldWear : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraShouldWear;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraShouldWear[0];

		static constexpr auto TYPE{ kShouldWear };
		ExtraShouldWear() = delete;
	};
	static_assert(sizeof(ExtraShouldWear) == 0x18);

	class ExtraFlags : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraFlags;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraFlags[0];

		static constexpr auto TYPE{ kFlags };
		ExtraFlags() = delete;
		/* 0x018 */ std::uint32_t unk18;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraFlags) == 0x20);
	static_assert(offsetof(ExtraFlags, unk18) == 0x18);

	class ExtraLockList : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraLockList;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraLockList[0];

		static constexpr auto TYPE{ kLockList };
		ExtraLockList() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint64_t lockList;
	};
	static_assert(sizeof(ExtraLockList) == 0x20);
	static_assert(offsetof(ExtraLockList, lockList) == 0x18);

	class ExtraDroppedItemList : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraDroppedItemList;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraDroppedItemList[0];

		static constexpr auto TYPE{ kDroppedItemList };
		ExtraDroppedItemList() = delete;
		/* 0x018 */ std::uint32_t unk18;
		/* 0x01C */ std::uint8_t unk1C[0x04];
		/* 0x020 */ std::uint64_t unk20;
	};
	static_assert(sizeof(ExtraDroppedItemList) == 0x28);
	static_assert(offsetof(ExtraDroppedItemList, unk18) == 0x18);

	class ExtraItemDropper : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraItemDropper;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraItemDropper[0];

		static constexpr auto TYPE{ kItemDropper };
		ExtraItemDropper() = delete;
		/* 0x018 */ std::uint32_t unk18;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraItemDropper) == 0x20);
	static_assert(offsetof(ExtraItemDropper, unk18) == 0x18);

	class ExtraLeveledItem : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraLeveledItem;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraLeveledItem[0];

		static constexpr auto TYPE{ kLevelItem };
		ExtraLeveledItem() = delete;
		/* 0x018 */ std::uint32_t levItem;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraLeveledItem) == 0x20);
	static_assert(offsetof(ExtraLeveledItem, levItem) == 0x18);

	class ExtraLeveledItemBase : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraLeveledItemBase;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraLeveledItemBase[0];

		static constexpr auto TYPE{ kLeveledItemBase };
		ExtraLeveledItemBase() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint64_t leveledItemBase;
	};
	static_assert(sizeof(ExtraLeveledItemBase) == 0x20);
	static_assert(offsetof(ExtraLeveledItemBase, leveledItemBase) == 0x18);

	class ExtraObjectBreakable : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraObjectBreakable;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraObjectBreakable[0];

		static constexpr auto TYPE{ kObjectBreakable };
		ExtraObjectBreakable() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ BSTArray<std::byte> objectBreakable; // BSTArray header (buffer, capacity, size); element type unrecovered
	};
	static_assert(sizeof(ExtraObjectBreakable) == 0x30);
	static_assert(offsetof(ExtraObjectBreakable, objectBreakable) == 0x18);

	class ExtraInstanceData : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraInstanceData;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraInstanceData[0];

		static constexpr auto TYPE{ kInstanceData };
		ExtraInstanceData() = delete;
		/* 0x018 */ std::uint64_t unk18;
		/* 0x020 */ std::uint64_t unk20;
	};
	static_assert(sizeof(ExtraInstanceData) == 0x28);
	static_assert(offsetof(ExtraInstanceData, unk18) == 0x18);

	class BGSObjectInstanceExtra : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::BGSObjectInstanceExtra;
		constexpr static Common::ID VTABLE = IDs::VTABLE::BGSObjectInstanceExtra[0];

		static constexpr auto TYPE{ kObjectInstance };
		BGSObjectInstanceExtra() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x10 */ bool CompareForUI(const BSExtraData* rhs) const noexcept(true) override;
		/* 0x018 */ void* unk18; // pointer to a 0x10 byte heap object {buffer pointer, 32 bit count}; the destructor frees the buffer and the object
		/* 0x020 */ std::uint64_t unk20;
	};
	static_assert(sizeof(BGSObjectInstanceExtra) == 0x28);
	static_assert(offsetof(BGSObjectInstanceExtra, unk18) == 0x18);

	class ExtraAlphaCutoff : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraAlphaCutoff;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraAlphaCutoff[0];

		static constexpr auto TYPE{ kAlphaCutoff };
		ExtraAlphaCutoff() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint8_t unk18;
		/* 0x019 */ std::uint8_t unk19;
		/* 0x01A */ std::uint8_t unk1A[0x06];
	};
	static_assert(sizeof(ExtraAlphaCutoff) == 0x20);
	static_assert(offsetof(ExtraAlphaCutoff, unk18) == 0x18);

	class ExtraRefractionProperty : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraRefractionProperty;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraRefractionProperty[0];

		static constexpr auto TYPE{ kRefractionProperty };
		ExtraRefractionProperty() = delete;
		/* 0x018 */ float unk18;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraRefractionProperty) == 0x20);
	static_assert(offsetof(ExtraRefractionProperty, unk18) == 0x18);

	class ExtraModelSwap : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraModelSwap;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraModelSwap[0];

		static constexpr auto TYPE{ kModelSwap };
		ExtraModelSwap() = delete;
		/* 0x018 */ std::uint64_t unk18;
		/* 0x020 */ std::uint64_t unk20;
	};
	static_assert(sizeof(ExtraModelSwap) == 0x28);
	static_assert(offsetof(ExtraModelSwap, unk18) == 0x18);

	class ExtraLinkColorPair : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraLinkColorPair;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraLinkColorPair[0];

		static constexpr auto TYPE{ kLinkColorPair };
		ExtraLinkColorPair() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint32_t unk18;
		/* 0x01C */ std::uint32_t unk1C;
	};
	static_assert(sizeof(ExtraLinkColorPair) == 0x20);
	static_assert(offsetof(ExtraLinkColorPair, unk18) == 0x18);

	class ExtraRadioData : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraRadioData;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraRadioData[0];

		static constexpr auto TYPE{ kRadioData };
		ExtraRadioData() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint64_t unk18;
		/* 0x020 */ std::uint64_t unk20;
	};
	static_assert(sizeof(ExtraRadioData) == 0x28);
	static_assert(offsetof(ExtraRadioData, unk18) == 0x18);

	class ExtraSound : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraSound;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraSound[0];

		static constexpr auto TYPE{ kSound };
		ExtraSound() = delete;
		/* 0x018 */ std::uint64_t unk18;
	};
	static_assert(sizeof(ExtraSound) == 0x20);
	static_assert(offsetof(ExtraSound, unk18) == 0x18);

	class ExtraSoundOutputOverride : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraSoundOutputOverride;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraSoundOutputOverride[0];

		static constexpr auto TYPE{ kSoundOutput };
		ExtraSoundOutputOverride() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint64_t soundOutputOverride;
	};
	static_assert(sizeof(ExtraSoundOutputOverride) == 0x20);
	static_assert(offsetof(ExtraSoundOutputOverride, soundOutputOverride) == 0x18);

	class ExtraActivateLoopSound : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraActivateLoopSound;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraActivateLoopSound[0];

		static constexpr auto TYPE{ kActivateLoopSound };
		ExtraActivateLoopSound() = delete;
		/* 0x018 */ std::uint64_t unk18;
	};
	static_assert(sizeof(ExtraActivateLoopSound) == 0x20);
	static_assert(offsetof(ExtraActivateLoopSound, unk18) == 0x18);

	class ExtraCreatureMovementSound : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraCreatureMovementSound;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraCreatureMovementSound[0];

		static constexpr auto TYPE{ kCreatureMovementSound };
		ExtraCreatureMovementSound() = delete;
		/* 0x018 */ std::uint64_t unk18;
	};
	static_assert(sizeof(ExtraCreatureMovementSound) == 0x20);
	static_assert(offsetof(ExtraCreatureMovementSound, unk18) == 0x18);

	class ExtraCreatureAwakeSound : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraCreatureAwakeSound;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraCreatureAwakeSound[0];

		static constexpr auto TYPE{ kCreatureAwakeSound };
		ExtraCreatureAwakeSound() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint8_t unk18[0x0C];
		/* 0x024 */ float unk24;
		/* 0x028 */ std::uint32_t unk28;
		/* 0x02C */ std::uint8_t unk2C[0x04];
	};
	static_assert(sizeof(ExtraCreatureAwakeSound) == 0x30);
	static_assert(offsetof(ExtraCreatureAwakeSound, unk18) == 0x18);

	class ExtraLastFinishedSequence : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraLastFinishedSequence;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraLastFinishedSequence[0];

		static constexpr auto TYPE{ kLastFinishedSequence };
		ExtraLastFinishedSequence() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint64_t lastFinishedSequence;
	};
	static_assert(sizeof(ExtraLastFinishedSequence) == 0x20);
	static_assert(offsetof(ExtraLastFinishedSequence, lastFinishedSequence) == 0x18);

	class ExtraScriptedAnimDependence : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraScriptedAnimDependence;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraScriptedAnimDependence[0];

		static constexpr auto TYPE{ kScriptedAnimDependence };
		ExtraScriptedAnimDependence() = delete;
		/* 0x018 */ BSTArray<std::byte> scriptedAnimDependence; // BSTArray header (buffer, capacity, size); element type unrecovered
	};
	static_assert(sizeof(ExtraScriptedAnimDependence) == 0x30);
	static_assert(offsetof(ExtraScriptedAnimDependence, scriptedAnimDependence) == 0x18);

	class ExtraObjectSavedDynamicIdles : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraObjectSavedDynamicIdles;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraObjectSavedDynamicIdles[0];

		static constexpr auto TYPE{ kSavedDynamicIdles };
		ExtraObjectSavedDynamicIdles() = delete;
		/* 0x018 */ BSTArray<BSFixedString> idles; // destructor releases each 8-byte string, then frees the buffer
		/* 0x030 */ BSFixedString unk30; // released by the destructor
	};
	static_assert(sizeof(ExtraObjectSavedDynamicIdles) == 0x38);
	static_assert(offsetof(ExtraObjectSavedDynamicIdles, idles) == 0x18);
	static_assert(offsetof(ExtraObjectSavedDynamicIdles, unk30) == 0x30);

	class ExtraObjectSavedUnrecoverableSubgraphs : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraObjectSavedUnrecoverableSubgraphs;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraObjectSavedUnrecoverableSubgraphs[0];

		static constexpr auto TYPE{ kSavedUnrecoverableSubgraphs };
		ExtraObjectSavedUnrecoverableSubgraphs() = delete;
		/* 0x018 */ std::uint8_t unk18[0x118];
	};
	static_assert(sizeof(ExtraObjectSavedUnrecoverableSubgraphs) == 0x130);
	static_assert(offsetof(ExtraObjectSavedUnrecoverableSubgraphs, unk18) == 0x18);

	class ExtraCulledBone : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraCulledBone;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraCulledBone[0];

		static constexpr auto TYPE{ kCulledBone };
		ExtraCulledBone() = delete;
		/* 0x018 */ std::uint8_t unk18[0x18];
	};
	static_assert(sizeof(ExtraCulledBone) == 0x30);
	static_assert(offsetof(ExtraCulledBone, unk18) == 0x18);

	class ExtraInputEnableLayer : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraInputEnableLayer;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraInputEnableLayer[0];

		static constexpr auto TYPE{ kInputEnableLayer };
		ExtraInputEnableLayer() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint64_t inputEnableLayer;
	};
	static_assert(sizeof(ExtraInputEnableLayer) == 0x20);
	static_assert(offsetof(ExtraInputEnableLayer, inputEnableLayer) == 0x18);

	class ExtraDirectAtTarget : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraDirectAtTarget;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraDirectAtTarget[0];

		static constexpr auto TYPE{ kDirectAtTarget };
		ExtraDirectAtTarget() = delete;
		/* 0x018 */ std::uint32_t unk18;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraDirectAtTarget) == 0x20);
	static_assert(offsetof(ExtraDirectAtTarget, unk18) == 0x18);

	namespace Workshop
	{
		class ExtraData : public BSExtraData
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::Workshop__ExtraData;
			constexpr static Common::ID VTABLE = IDs::VTABLE::Workshop__ExtraData[0];

			static constexpr auto TYPE{ kWorkshop };
			ExtraData() = delete;
			/* 0x018 */ std::uint8_t unk18[0x40];
		};
		static_assert(sizeof(ExtraData) == 0x58);
		static_assert(offsetof(ExtraData, unk18) == 0x18);
	}
}
