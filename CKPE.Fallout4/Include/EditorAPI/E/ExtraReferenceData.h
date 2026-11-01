// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/E/ExtraDataTypes.h>
#include <EditorAPI/T/TESFullName.h>

namespace CKPE::Fallout4::EditorAPI
{
	struct MapMarkerData
	{
		/* 0x000 */ TESFullName FullName;
		/* 0x018 */ std::uint8_t unk18[0x08];
	};
	static_assert(sizeof(MapMarkerData) == 0x20);
	static_assert(offsetof(MapMarkerData, unk18) == 0x18);

	struct alignas(8) REFR_LOCK
	{
		/* 0x000 */ std::uint8_t unk00[0x18];
	};
	static_assert(sizeof(REFR_LOCK) == 0x18);

	struct alignas(8) DoorTeleportData
	{
		/* 0x000 */ std::uint8_t unk00[0x20];
	};
	static_assert(sizeof(DoorTeleportData) == 0x20);

	class ExtraLock : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kLock };
		ExtraLock() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ REFR_LOCK* Lock;
	};
	static_assert(sizeof(ExtraLock) == 0x20);
	static_assert(offsetof(ExtraLock, Lock) == 0x18);

	class ExtraTeleport : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kTeleport };
		ExtraTeleport() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ DoorTeleportData* TeleportData;
	};
	static_assert(sizeof(ExtraTeleport) == 0x20);
	static_assert(offsetof(ExtraTeleport, TeleportData) == 0x18);

	class ExtraMapMarker : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kMapMarker };
		ExtraMapMarker() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ MapMarkerData* MarkerData;
	};
	static_assert(sizeof(ExtraMapMarker) == 0x20);
	static_assert(offsetof(ExtraMapMarker, MarkerData) == 0x18);

	class ExtraOwnership : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kOwnership };
		ExtraOwnership() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint64_t unk18;
		/* 0x020 */ std::uint32_t unk20;
		/* 0x024 */ std::uint8_t unk24;
		/* 0x025 */ std::uint8_t unk25[0x03];
	};
	static_assert(sizeof(ExtraOwnership) == 0x28);
	static_assert(offsetof(ExtraOwnership, unk18) == 0x18);

	class ExtraCount : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kCount };
		ExtraCount() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint16_t Count;
	};
	static_assert(sizeof(ExtraCount) == 0x20);
	static_assert(offsetof(ExtraCount, Count) == 0x18);

	class ExtraHealth : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kHealth };
		ExtraHealth() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ float Health;
	};
	static_assert(sizeof(ExtraHealth) == 0x20);
	static_assert(offsetof(ExtraHealth, Health) == 0x18);

	class ExtraPersistentCell : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kPersistentCell };
		ExtraPersistentCell() = delete;
		/* 0x018 */ Forms::TESForm* Cell;
	};
	static_assert(sizeof(ExtraPersistentCell) == 0x20);
	static_assert(offsetof(ExtraPersistentCell, Cell) == 0x18);

	class ExtraEncounterZone : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kEncounterZone };
		ExtraEncounterZone() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ Forms::TESForm* EncounterZone;
	};
	static_assert(sizeof(ExtraEncounterZone) == 0x20);
	static_assert(offsetof(ExtraEncounterZone, EncounterZone) == 0x18);

	class ExtraEmittanceSource : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kEmittanceSource };
		ExtraEmittanceSource() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ Forms::TESForm* EmittanceSource;
	};
	static_assert(sizeof(ExtraEmittanceSource) == 0x20);
	static_assert(offsetof(ExtraEmittanceSource, EmittanceSource) == 0x18);

	class ExtraLocation : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kLocation };
		ExtraLocation() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ Forms::TESForm* Location;
	};
	static_assert(sizeof(ExtraLocation) == 0x20);
	static_assert(offsetof(ExtraLocation, Location) == 0x18);

	class ExtraLocationRefType : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kLocationRefType };
		ExtraLocationRefType() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint64_t unk18;
		/* 0x020 */ std::uint8_t unk20[0x08];
		/* 0x028 */ std::uint32_t unk28;
		/* 0x02C */ std::uint8_t unk2C[0x04];
	};
	static_assert(sizeof(ExtraLocationRefType) == 0x30);
	static_assert(offsetof(ExtraLocationRefType, unk18) == 0x18);

	class ExtraActivateRef : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kActivateRef };
		ExtraActivateRef() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint8_t unk18;
		/* 0x019 */ std::uint8_t unk19[0x07];
		/* 0x020 */ std::uint64_t unk20;
		/* 0x028 */ std::uint64_t unk28;
	};
	static_assert(sizeof(ExtraActivateRef) == 0x30);
	static_assert(offsetof(ExtraActivateRef, unk18) == 0x18);

	class ExtraActivateRefChildren : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kActivateRefChildren };
		ExtraActivateRefChildren() = delete;
		/* 0x018 */ std::uint64_t unk18;
		/* 0x020 */ std::uint64_t unk20;
		/* 0x028 */ std::uint32_t unk28;
		/* 0x02C */ std::uint8_t unk2C[0x04];
	};
	static_assert(sizeof(ExtraActivateRefChildren) == 0x30);
	static_assert(offsetof(ExtraActivateRefChildren, unk18) == 0x18);

	class ExtraEnableStateChildren : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kEnableStateChildren };
		ExtraEnableStateChildren() = delete;
		/* 0x018 */ std::uint32_t unk18;
		/* 0x01C */ std::uint8_t unk1C[0x04];
		/* 0x020 */ std::uint64_t unk20;
	};
	static_assert(sizeof(ExtraEnableStateChildren) == 0x28);
	static_assert(offsetof(ExtraEnableStateChildren, unk18) == 0x18);

	class ExtraEditorTestRadius : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kEditorTestRadius };
		ExtraEditorTestRadius() = delete;
		/* 0x018 */ float unk18;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraEditorTestRadius) == 0x20);
	static_assert(offsetof(ExtraEditorTestRadius, unk18) == 0x18);

	class ExtraEditorFlags : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kEditorFlags };
		ExtraEditorFlags() = delete;
		/* 0x018 */ std::uint32_t unk18;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraEditorFlags) == 0x20);
	static_assert(offsetof(ExtraEditorFlags, unk18) == 0x18);

	class ExtraOriginalReference : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kOriginalReference };
		ExtraOriginalReference() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint32_t unk18;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraOriginalReference) == 0x20);
	static_assert(offsetof(ExtraOriginalReference, unk18) == 0x18);

	class ExtraReferenceGroup : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kReferenceGroup };
		ExtraReferenceGroup() = delete;
		/* 0x018 */ std::uint64_t unk18;
	};
	static_assert(sizeof(ExtraReferenceGroup) == 0x20);
	static_assert(offsetof(ExtraReferenceGroup, unk18) == 0x18);

	class ExtraStartingWorldOrCell : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kStartingWorldOrCell };
		ExtraStartingWorldOrCell() = delete;
		/* 0x018 */ std::uint64_t unk18;
	};
	static_assert(sizeof(ExtraStartingWorldOrCell) == 0x20);
	static_assert(offsetof(ExtraStartingWorldOrCell, unk18) == 0x18);

	class ExtraMasterFileCell : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kMasterFileCell };
		ExtraMasterFileCell() = delete;
		/* 0x018 */ std::uint64_t unk18;
	};
	static_assert(sizeof(ExtraMasterFileCell) == 0x20);
	static_assert(offsetof(ExtraMasterFileCell, unk18) == 0x18);

	class ExtraMasterLocation : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kMasterLocation };
		ExtraMasterLocation() = delete;
		/* 0x018 */ std::uint64_t unk18;
	};
	static_assert(sizeof(ExtraMasterLocation) == 0x20);
	static_assert(offsetof(ExtraMasterLocation, unk18) == 0x18);

	class ExtraAttachedArrows3D : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kAttachArrows3D };
		ExtraAttachedArrows3D() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint64_t unk18;
		/* 0x020 */ std::uint8_t unk20[0x08];
		/* 0x028 */ std::uint32_t unk28;
		/* 0x02C */ std::uint8_t unk2C[0x0C];
	};
	static_assert(sizeof(ExtraAttachedArrows3D) == 0x38);
	static_assert(offsetof(ExtraAttachedArrows3D, unk18) == 0x18);

	class ExtraAttachRef : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kAttachRef };
		ExtraAttachRef() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint32_t unk18;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraAttachRef) == 0x20);
	static_assert(offsetof(ExtraAttachRef, unk18) == 0x18);

	class ExtraAttachRefChildren : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kAttachRefChildren };
		ExtraAttachRefChildren() = delete;
		/* 0x018 */ std::uint8_t unk18[0x48];
		/* 0x060 */ std::uint8_t unk60[0x10]; // a BSSimpleList {item, next}, the only member the destructor releases
	};
	static_assert(sizeof(ExtraAttachRefChildren) == 0x70);
	static_assert(offsetof(ExtraAttachRefChildren, unk18) == 0x18);

	class ExtraOpenCloseActivateRef : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kOpenCloseActivateRef };
		ExtraOpenCloseActivateRef() = delete;
		/* 0x018 */ std::uint32_t unk18;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraOpenCloseActivateRef) == 0x20);
	static_assert(offsetof(ExtraOpenCloseActivateRef, unk18) == 0x18);

	class ExtraForcedLandingMarker : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kForcedLandingMarker };
		ExtraForcedLandingMarker() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint32_t unk18;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraForcedLandingMarker) == 0x20);
	static_assert(offsetof(ExtraForcedLandingMarker, unk18) == 0x18);

	class ExtraForcedTarget : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kForcedTarget };
		ExtraForcedTarget() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint32_t unk18;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraForcedTarget) == 0x20);
	static_assert(offsetof(ExtraForcedTarget, unk18) == 0x18);

	class ExtraTeleportName : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kTeleportName };
		ExtraTeleportName() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint64_t unk18;
	};
	static_assert(sizeof(ExtraTeleportName) == 0x20);
	static_assert(offsetof(ExtraTeleportName, unk18) == 0x18);

	class ExtraRandomTeleportMarker : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kTeleportMarker };
		ExtraRandomTeleportMarker() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ std::uint64_t unk18;
	};
	static_assert(sizeof(ExtraRandomTeleportMarker) == 0x20);
	static_assert(offsetof(ExtraRandomTeleportMarker, unk18) == 0x18);

	class ExtraRangedDistOverride : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kRangeDistOverride };
		ExtraRangedDistOverride() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ float unk18;
		/* 0x01C */ float unk1C;
	};
	static_assert(sizeof(ExtraRangedDistOverride) == 0x20);
	static_assert(offsetof(ExtraRangedDistOverride, unk18) == 0x18);

	class ExtraScale : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kScale };
		ExtraScale() = delete;
		/* 0x08 */ bool CompareImpl(const BSExtraData& rhs) const noexcept(true) override;
		/* 0x018 */ float unk18;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraScale) == 0x20);
	static_assert(offsetof(ExtraScale, unk18) == 0x18);

	class ExtraCachedScale : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kCachedScale };
		ExtraCachedScale() = delete;
		/* 0x018 */ float unk18;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraCachedScale) == 0x20);
	static_assert(offsetof(ExtraCachedScale, unk18) == 0x18);

	class ExtraObjectHealth : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kObjectHealth };
		ExtraObjectHealth() = delete;
		/* 0x018 */ float unk18;
		/* 0x01C */ std::uint8_t unk1C[0x04];
	};
	static_assert(sizeof(ExtraObjectHealth) == 0x20);
	static_assert(offsetof(ExtraObjectHealth, unk18) == 0x18);

	class ExtraBadPosition : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kBadPosition };
		ExtraBadPosition() = delete;
	};
	static_assert(sizeof(ExtraBadPosition) == 0x18);
}
