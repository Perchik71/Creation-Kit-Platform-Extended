// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/E/ExtraDataTypes.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE::Fallout4::EditorAPI
{
	class ExtraActorCause : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraActorCause;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraActorCause[0];

		static constexpr auto TYPE{ kActorCause };
		ExtraActorCause() = delete;
		/* 0x018 */ std::uint64_t unk18;
	};
	static_assert(sizeof(ExtraActorCause) == 0x20);
	static_assert(offsetof(ExtraActorCause, unk18) == 0x18);

	class ExtraAliasInstanceArray : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraAliasInstanceArray;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraAliasInstanceArray[0];

		static constexpr auto TYPE{ kAliasInstanceArray };
		ExtraAliasInstanceArray() = delete;
		/* 0x018 */ std::uint64_t unk18;
		/* 0x020 */ std::uint32_t unk20;
		/* 0x024 */ std::uint8_t unk24[0x04];
		/* 0x028 */ std::uint32_t unk28;
		/* 0x02C */ std::uint8_t unk2C[0x0C];
	};
	static_assert(sizeof(ExtraAliasInstanceArray) == 0x38);
	static_assert(offsetof(ExtraAliasInstanceArray, unk18) == 0x18);

	class ExtraAmmo : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraAmmo;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraAmmo[0];

		static constexpr auto TYPE{ kAmmo };
		ExtraAmmo() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint32_t count;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraAmmo) == 0x20);
	static_assert(offsetof(ExtraAmmo, count) == 0x18);

	class ExtraAnimGraphManager : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraAnimGraphManager;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraAnimGraphManager[0];

		static constexpr auto TYPE{ kHavokAnim };
		ExtraAnimGraphManager() = delete;
		/* 0x018 */ std::uint64_t unk18;
		/* 0x020 */ std::uint32_t unk20;
		/* 0x024 */ std::uint8_t unk24;
		/* 0x025 */ std::uint8_t unk25[0x03];
	};
	static_assert(sizeof(ExtraAnimGraphManager) == 0x28);
	static_assert(offsetof(ExtraAnimGraphManager, unk18) == 0x18);

	class ExtraAshPileRef : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraAshPileRef;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraAshPileRef[0];

		static constexpr auto TYPE{ kAshpileRef };
		ExtraAshPileRef() = delete;
		/* 0x018 */ std::uint32_t unk18;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraAshPileRef) == 0x20);
	static_assert(offsetof(ExtraAshPileRef, unk18) == 0x18);

	class ExtraBiped : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraBiped;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraBiped[0];

		static constexpr auto TYPE{ kBiped };
		ExtraBiped() = delete;
		/* 0x018 */ std::uint64_t unk18;
	};
	static_assert(sizeof(ExtraBiped) == 0x20);
	static_assert(offsetof(ExtraBiped, unk18) == 0x18);

	class ExtraCombatStyle : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraCombatStyle;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraCombatStyle[0];

		static constexpr auto TYPE{ kCombatStyle };
		ExtraCombatStyle() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint64_t combatStyle;
	};
	static_assert(sizeof(ExtraCombatStyle) == 0x20);
	static_assert(offsetof(ExtraCombatStyle, combatStyle) == 0x18);

	class ExtraDismemberedLimbs : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraDismemberedLimbs;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraDismemberedLimbs[0];

		static constexpr auto TYPE{ kDismemberedLimbs };
		ExtraDismemberedLimbs() = delete;
		/* 0x018 */ std::uint32_t unk18;
		/* 0x01C */ std::uint8_t unk1C;
		/* 0x01D */ std::uint8_t unk1D[0x03];
		/* 0x020 */ BSTArray<std::uint64_t> dismemberedLimbs; // 8 byte elements (stride found in the indexing code)
		/* 0x038 */ std::uint64_t unk38;
		/* 0x040 */ std::uint64_t unk40;
	};
	static_assert(sizeof(ExtraDismemberedLimbs) == 0x48);
	static_assert(offsetof(ExtraDismemberedLimbs, unk18) == 0x18);

	class ExtraEssentialProtected : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraEssentialProtected;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraEssentialProtected[0];

		static constexpr auto TYPE{ kEssentialProtected };
		ExtraEssentialProtected() = delete;
		/* 0x018 */ std::uint16_t unk18;
		/* 0x01A */ std::uint8_t unk1A[0x06];
	};
	static_assert(sizeof(ExtraEssentialProtected) == 0x20);
	static_assert(offsetof(ExtraEssentialProtected, unk18) == 0x18);

	class ExtraFXPickNodes : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraFXPickNodes;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraFXPickNodes[0];

		static constexpr auto TYPE{ kActorFXPickNodes };
		ExtraFXPickNodes() = delete;
		/* 0x018 */ BSTArray<std::byte> fxPickNodes0; // BSTArray header (buffer, capacity, size); element type unrecovered
		/* 0x030 */ BSTArray<std::byte> fxPickNodes1; // BSTArray header (buffer, capacity, size); element type unrecovered
		/* 0x048 */ BSTArray<std::byte> fxPickNodes2; // BSTArray header (buffer, capacity, size); element type unrecovered
		/* 0x060 */ BSTArray<std::byte> fxPickNodes3; // BSTArray header (buffer, capacity, size); element type unrecovered
	};
	static_assert(sizeof(ExtraFXPickNodes) == 0x78);
	static_assert(offsetof(ExtraFXPickNodes, fxPickNodes0) == 0x18);

	class ExtraFactionChanges : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraFactionChanges;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraFactionChanges[0];

		static constexpr auto TYPE{ kFactionChanges };
		ExtraFactionChanges() = delete;
		/* 0x018 */ std::uint8_t removeCrimeFaction;
		/* 0x019 */ std::uint8_t unk19[0x1F];
		/* 0x038 */ std::uint64_t crimeFaction;
	};
	static_assert(sizeof(ExtraFactionChanges) == 0x40);
	static_assert(offsetof(ExtraFactionChanges, removeCrimeFaction) == 0x18);

	class ExtraFavorCost : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraFavorCost;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraFavorCost[0];

		static constexpr auto TYPE{ kFavorCost };
		ExtraFavorCost() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ float favorCost;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraFavorCost) == 0x20);
	static_assert(offsetof(ExtraFavorCost, favorCost) == 0x18);

	class ExtraFollowerSwimBreadcrumbs : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraFollowerSwimBreadcrumbs;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraFollowerSwimBreadcrumbs[0];

		static constexpr auto TYPE{ kFollowerSwimBreadcrumbs };
		ExtraFollowerSwimBreadcrumbs() = delete;
		/* 0x018 */ std::uint32_t unk18;
		/* 0x01C */ std::uint32_t unk1C;
		/* 0x020 */ std::uint32_t unk20;
		/* 0x024 */ std::uint32_t unk24;
		/* 0x028 */ std::uint32_t unk28;
		/* 0x02C */ std::uint8_t unk2C[0x04];
		/* 0x030 */ std::uint64_t unk30;
		/* 0x038 */ std::uint64_t unk38;
	};
	static_assert(sizeof(ExtraFollowerSwimBreadcrumbs) == 0x40);
	static_assert(offsetof(ExtraFollowerSwimBreadcrumbs, unk18) == 0x18);

	class ExtraFromAlias : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraFromAlias;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraFromAlias[0];

		static constexpr auto TYPE{ kFromAlias };
		ExtraFromAlias() = delete;
		/* 0x018 */ std::uint8_t unk18[0x10];
	};
	static_assert(sizeof(ExtraFromAlias) == 0x28);
	static_assert(offsetof(ExtraFromAlias, unk18) == 0x18);

	class ExtraGhost : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraGhost;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraGhost[0];

		static constexpr auto TYPE{ kGhost };
		ExtraGhost() = delete;
		/* 0x018 */ std::uint8_t unk18;
		/* 0x019 */ std::uint8_t unk19[0x07];
	};
	static_assert(sizeof(ExtraGhost) == 0x20);
	static_assert(offsetof(ExtraGhost, unk18) == 0x18);

	class ExtraGuardedRefData : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraGuardedRefData;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraGuardedRefData[0];

		static constexpr auto TYPE{ kGuardedRefData };
		ExtraGuardedRefData() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ BSTArray<std::byte> guardedRefData; // 24 byte elements (stride found in the indexing code); element layout unrecovered
	};
	static_assert(sizeof(ExtraGuardedRefData) == 0x30);
	static_assert(offsetof(ExtraGuardedRefData, guardedRefData) == 0x18);

	class ExtraHasNoRumors : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraHasNoRumors;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraHasNoRumors[0];

		static constexpr auto TYPE{ kNoRumors };
		ExtraHasNoRumors() = delete;
		/* 0x018 */ std::uint8_t unk18;
		/* 0x019 */ std::uint8_t unk19[0x07];
	};
	static_assert(sizeof(ExtraHasNoRumors) == 0x20);
	static_assert(offsetof(ExtraHasNoRumors, unk18) == 0x18);

	class ExtraHeadTrackingWeight : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraHeadTrackingWeight;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraHeadTrackingWeight[0];

		static constexpr auto TYPE{ kHeadTrackingWeight };
		ExtraHeadTrackingWeight() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ float headTrackingWeight;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraHeadTrackingWeight) == 0x20);
	static_assert(offsetof(ExtraHeadTrackingWeight, headTrackingWeight) == 0x18);

	class ExtraHeadingTarget : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraHeadingTarget;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraHeadingTarget[0];

		static constexpr auto TYPE{ kHeadTrackTarget };
		ExtraHeadingTarget() = delete;
		/* 0x018 */ std::uint32_t unk18;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraHeadingTarget) == 0x20);
	static_assert(offsetof(ExtraHeadingTarget, unk18) == 0x18);

	class ExtraHorse : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraHorse;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraHorse[0];

		static constexpr auto TYPE{ kHorse };
		ExtraHorse() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint32_t horse;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraHorse) == 0x20);
	static_assert(offsetof(ExtraHorse, horse) == 0x18);

	class ExtraIgnoredAttractKeywords : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraIgnoredAttractKeywords;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraIgnoredAttractKeywords[0];

		static constexpr auto TYPE{ kIgnoredAttractKeywords };
		ExtraIgnoredAttractKeywords() = delete;
		/* 0x018 */ BSTArray<std::byte> ignoredAttractKeywords; // BSTArray header (buffer, capacity, size); element type unrecovered
	};
	static_assert(sizeof(ExtraIgnoredAttractKeywords) == 0x30);
	static_assert(offsetof(ExtraIgnoredAttractKeywords, ignoredAttractKeywords) == 0x18);

	class ExtraIgnoredBySandbox : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraIgnoredBySandbox;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraIgnoredBySandbox[0];

		static constexpr auto TYPE{ kIgnoredBySandbox };
		ExtraIgnoredBySandbox() = delete;
	};
	static_assert(sizeof(ExtraIgnoredBySandbox) == 0x18);

	class ExtraInvestedGold : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraInvestedGold;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraInvestedGold[0];

		static constexpr auto TYPE{ kRefInvestedGold };
		ExtraInvestedGold() = delete;
		/* 0x018 */ std::uint32_t unk18;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraInvestedGold) == 0x20);
	static_assert(offsetof(ExtraInvestedGold, unk18) == 0x18);

	class ExtraLevCreaModifier : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraLevCreaModifier;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraLevCreaModifier[0];

		static constexpr auto TYPE{ kLevCreaMod };
		ExtraLevCreaModifier() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint32_t levCreaModifier;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraLevCreaModifier) == 0x20);
	static_assert(offsetof(ExtraLevCreaModifier, levCreaModifier) == 0x18);

	class ExtraLeveledCreature : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraLeveledCreature;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraLeveledCreature[0];

		static constexpr auto TYPE{ kLeveledCreature };
		ExtraLeveledCreature() = delete;
		/* 0x018 */ std::uint64_t originalBase;
		/* 0x020 */ std::uint8_t unk20[0x68];
	};
	static_assert(sizeof(ExtraLeveledCreature) == 0x88);
	static_assert(offsetof(ExtraLeveledCreature, originalBase) == 0x18);

	class ExtraOutfitItem : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraOutfitItem;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraOutfitItem[0];

		static constexpr auto TYPE{ kOutfitItem };
		ExtraOutfitItem() = delete;
		/* 0x018 */ std::uint32_t outfit;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraOutfitItem) == 0x20);
	static_assert(offsetof(ExtraOutfitItem, outfit) == 0x18);

	class ExtraPackage : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraPackage;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraPackage[0];

		static constexpr auto TYPE{ kPackage };
		ExtraPackage() = delete;
		/* 0x018 */ std::uint64_t unk18;
		/* 0x020 */ std::uint32_t unk20;
		/* 0x024 */ std::uint32_t unk24;
		/* 0x028 */ std::uint8_t unk28;
		/* 0x029 */ std::uint8_t unk29;
		/* 0x02A */ std::uint8_t unk2A;
		/* 0x02B */ std::uint8_t unk2B[0x05];
	};
	static_assert(sizeof(ExtraPackage) == 0x30);
	static_assert(offsetof(ExtraPackage, unk18) == 0x18);

	class ExtraPackageStartLocation : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraPackageStartLocation;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraPackageStartLocation[0];

		static constexpr auto TYPE{ kPackagesStartLoc };
		ExtraPackageStartLocation() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint64_t unk18;
		/* 0x020 */ std::uint32_t unk20;
		/* 0x024 */ std::uint32_t unk24;
		/* 0x028 */ std::uint32_t unk28;
		/* 0x02C */ std::uint32_t unk2C;
	};
	static_assert(sizeof(ExtraPackageStartLocation) == 0x30);
	static_assert(offsetof(ExtraPackageStartLocation, unk18) == 0x18);

	class ExtraPatrolRefData : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraPatrolRefData;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraPatrolRefData[0];

		static constexpr auto TYPE{ kPatrolRefData };
		ExtraPatrolRefData() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint64_t patrolRefData;
	};
	static_assert(sizeof(ExtraPatrolRefData) == 0x20);
	static_assert(offsetof(ExtraPatrolRefData, patrolRefData) == 0x18);

	class ExtraPatrolRefInUseData : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraPatrolRefInUseData;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraPatrolRefInUseData[0];

		static constexpr auto TYPE{ kPatrolRefInUseData };
		ExtraPatrolRefInUseData() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint32_t patrolRefInUseData;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraPatrolRefInUseData) == 0x20);
	static_assert(offsetof(ExtraPatrolRefInUseData, patrolRefInUseData) == 0x18);

	class ExtraPlayerCrimeList : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraPlayerCrimeList;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraPlayerCrimeList[0];

		static constexpr auto TYPE{ kPlayerCrimeList };
		ExtraPlayerCrimeList() = delete;
		/* 0x018 */ std::uint64_t unk18;
	};
	static_assert(sizeof(ExtraPlayerCrimeList) == 0x20);
	static_assert(offsetof(ExtraPlayerCrimeList, unk18) == 0x18);

	class ExtraPromotedRef : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraPromotedRef;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraPromotedRef[0];

		static constexpr auto TYPE{ kPromotedRef };
		ExtraPromotedRef() = delete;
		/* 0x018 */ BSTArray<std::byte> promotedRef; // BSTArray header (buffer, capacity, size); element type unrecovered
	};
	static_assert(sizeof(ExtraPromotedRef) == 0x30);
	static_assert(offsetof(ExtraPromotedRef, promotedRef) == 0x18);

	class ExtraRunOncePacks : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraRunOncePacks;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraRunOncePacks[0];

		static constexpr auto TYPE{ kRunOncePackages };
		ExtraRunOncePacks() = delete;
		/* 0x018 */ std::uint64_t unk18;
	};
	static_assert(sizeof(ExtraRunOncePacks) == 0x20);
	static_assert(offsetof(ExtraRunOncePacks, unk18) == 0x18);

	class ExtraSayTopicInfoOnceADay : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraSayTopicInfoOnceADay;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraSayTopicInfoOnceADay[0];

		static constexpr auto TYPE{ kSayOnceADayTopicInfo };
		ExtraSayTopicInfoOnceADay() = delete;
		/* 0x018 */ std::uint64_t unk18;
	};
	static_assert(sizeof(ExtraSayTopicInfoOnceADay) == 0x20);
	static_assert(offsetof(ExtraSayTopicInfoOnceADay, unk18) == 0x18);

	class ExtraSceneData : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraSceneData;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraSceneData[0];

		static constexpr auto TYPE{ kScene };
		ExtraSceneData() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint64_t sceneData;
	};
	static_assert(sizeof(ExtraSceneData) == 0x20);
	static_assert(offsetof(ExtraSceneData, sceneData) == 0x18);

	class ExtraSpawnContainer : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraSpawnContainer;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraSpawnContainer[0];

		static constexpr auto TYPE{ kSpawnCount };
		ExtraSpawnContainer() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint32_t unk18;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraSpawnContainer) == 0x20);
	static_assert(offsetof(ExtraSpawnContainer, unk18) == 0x18);

	class ExtraVoiceType : public BSExtraData
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::ExtraVoiceType;
		constexpr static Common::ID VTABLE = IDs::VTABLE::ExtraVoiceType[0];

		static constexpr auto TYPE{ kVoiceType };
		ExtraVoiceType() = delete;
		/* 0x018 */ std::uint64_t unk18;
	};
	static_assert(sizeof(ExtraVoiceType) == 0x20);
	static_assert(offsetof(ExtraVoiceType, unk18) == 0x18);
}
