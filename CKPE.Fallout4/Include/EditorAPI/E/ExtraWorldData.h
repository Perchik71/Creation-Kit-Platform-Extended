// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/E/ExtraDataTypes.h>

namespace CKPE::Fallout4::EditorAPI
{
	class ExtraCell3D : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kCell3D };
		ExtraCell3D() = delete;
		/* 0x018 */ std::uint8_t unk18[0x08];
	};
	static_assert(sizeof(ExtraCell3D) == 0x20);
	static_assert(offsetof(ExtraCell3D, unk18) == 0x18);

	class ExtraRegionList : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kRegionList };
		ExtraRegionList() = delete;
		/* 0x018 */ std::uint8_t unk18[0x08];
	};
	static_assert(sizeof(ExtraRegionList) == 0x20);
	static_assert(offsetof(ExtraRegionList, unk18) == 0x18);

	class ExtraCellMusicType : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kCellMusicType };
		ExtraCellMusicType() = delete;
		/* 0x018 */ Forms::TESForm* MusicType;
	};
	static_assert(sizeof(ExtraCellMusicType) == 0x20);
	static_assert(offsetof(ExtraCellMusicType, MusicType) == 0x18);

	class ExtraCellSkyRegion : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kSkyRegion };
		ExtraCellSkyRegion() = delete;
		/* 0x018 */ Forms::TESForm* SkyRegion;
	};
	static_assert(sizeof(ExtraCellSkyRegion) == 0x20);
	static_assert(offsetof(ExtraCellSkyRegion, SkyRegion) == 0x18);

	class ExtraCellAcousticSpace : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kCellAcousticSpace };
		ExtraCellAcousticSpace() = delete;
		/* 0x018 */ Forms::TESForm* AcousticSpace;
	};
	static_assert(sizeof(ExtraCellAcousticSpace) == 0x20);
	static_assert(offsetof(ExtraCellAcousticSpace, AcousticSpace) == 0x18);

	class ExtraCellImageSpace : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kImageSpace };
		ExtraCellImageSpace() = delete;
		/* 0x018 */ Forms::TESForm* ImageSpace;
	};
	static_assert(sizeof(ExtraCellImageSpace) == 0x20);
	static_assert(offsetof(ExtraCellImageSpace, ImageSpace) == 0x18);

	class ExtraCellGodRays : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kGodRays };
		ExtraCellGodRays() = delete;
		/* 0x018 */ Forms::TESForm* GodRays;
	};
	static_assert(sizeof(ExtraCellGodRays) == 0x20);
	static_assert(offsetof(ExtraCellGodRays, GodRays) == 0x18);

	class ExtraDetachTime : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kCellDetachTime };
		ExtraDetachTime() = delete;
		/* 0x018 */ std::uint8_t unk18[0x08];
	};
	static_assert(sizeof(ExtraDetachTime) == 0x20);
	static_assert(offsetof(ExtraDetachTime, unk18) == 0x18);

	class ExtraDistantData : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kDistantData };
		ExtraDistantData() = delete;
		/* 0x018 */ std::uint8_t unk18[0x10];
	};
	static_assert(sizeof(ExtraDistantData) == 0x28);
	static_assert(offsetof(ExtraDistantData, unk18) == 0x18);

	class ExtraCellGrassData : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kCellGrassData };
		ExtraCellGrassData() = delete;
		/* 0x018 */ std::uint8_t unk18[0x18];
	};
	static_assert(sizeof(ExtraCellGrassData) == 0x30);
	static_assert(offsetof(ExtraCellGrassData, unk18) == 0x18);

	class ExtraCellWaterEnvMap : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kCellWaterEnvMap };
		ExtraCellWaterEnvMap() = delete;
		/* 0x018 */ std::uint8_t unk18[0x28];
	};
	static_assert(sizeof(ExtraCellWaterEnvMap) == 0x40);
	static_assert(offsetof(ExtraCellWaterEnvMap, unk18) == 0x18);

	class ExtraWaterData : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kWaterData };
		ExtraWaterData() = delete;
		/* 0x018 */ std::uint8_t unk18[0x18];
	};
	static_assert(sizeof(ExtraWaterData) == 0x30);
	static_assert(offsetof(ExtraWaterData, unk18) == 0x18);

	class ExtraWaterCurrentZoneData : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kWaterCurrentZoneData };
		ExtraWaterCurrentZoneData() = delete;
		/* 0x018 */ std::uint8_t unk18[0x60];
	};
	static_assert(sizeof(ExtraWaterCurrentZoneData) == 0x78);
	static_assert(offsetof(ExtraWaterCurrentZoneData, unk18) == 0x18);

	class ExtraWaterLightRefs : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kWaterLightRefs };
		ExtraWaterLightRefs() = delete;
		/* 0x018 */ std::uint8_t unk18[0x10];
	};
	static_assert(sizeof(ExtraWaterLightRefs) == 0x28);
	static_assert(offsetof(ExtraWaterLightRefs, unk18) == 0x18);

	class ExtraLitWaterRefs : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kLitWaterRefs };
		ExtraLitWaterRefs() = delete;
		/* 0x018 */ std::uint8_t unk18[0x10];
	};
	static_assert(sizeof(ExtraLitWaterRefs) == 0x28);
	static_assert(offsetof(ExtraLitWaterRefs, unk18) == 0x18);

	class ExtraMultiBound : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kMultibound };
		ExtraMultiBound() = delete;
		/* 0x018 */ std::uint8_t unk18[0x08];
	};
	static_assert(sizeof(ExtraMultiBound) == 0x20);
	static_assert(offsetof(ExtraMultiBound, unk18) == 0x18);

	class ExtraMultiBoundData : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kMultiboundData };
		ExtraMultiBoundData() = delete;
		/* 0x018 */ std::uint8_t unk18[0x08];
	};
	static_assert(sizeof(ExtraMultiBoundData) == 0x20);
	static_assert(offsetof(ExtraMultiBoundData, unk18) == 0x18);

	class ExtraMultiBoundRef : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kMultiboundRef };
		ExtraMultiBoundRef() = delete;
		/* 0x018 */ std::uint8_t unk18[0x08];
	};
	static_assert(sizeof(ExtraMultiBoundRef) == 0x20);
	static_assert(offsetof(ExtraMultiBoundRef, unk18) == 0x18);

	class ExtraOcclusionShape : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kOcclusionShape };
		ExtraOcclusionShape() = delete;
		/* 0x018 */ std::uint8_t unk18[0x08];
	};
	static_assert(sizeof(ExtraOcclusionShape) == 0x20);
	static_assert(offsetof(ExtraOcclusionShape, unk18) == 0x18);

	class ExtraOcclusionPlaneRefData : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kOcclusionPlaneRefData };
		ExtraOcclusionPlaneRefData() = delete;
		/* 0x018 */ std::uint8_t unk18[0x08];
	};
	static_assert(sizeof(ExtraOcclusionPlaneRefData) == 0x20);
	static_assert(offsetof(ExtraOcclusionPlaneRefData, unk18) == 0x18);

	class ExtraPortalRefData : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kPortalRefData };
		ExtraPortalRefData() = delete;
		/* 0x018 */ std::uint8_t unk18[0x08];
	};
	static_assert(sizeof(ExtraPortalRefData) == 0x20);
	static_assert(offsetof(ExtraPortalRefData, unk18) == 0x18);

	class ExtraPortal : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kPortal };
		ExtraPortal() = delete;
		/* 0x018 */ std::uint8_t unk18[0x08];
	};
	static_assert(sizeof(ExtraPortal) == 0x20);
	static_assert(offsetof(ExtraPortal, unk18) == 0x18);

	class ExtraRoom : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kRoom };
		ExtraRoom() = delete;
		/* 0x018 */ std::uint8_t unk18[0x08];
	};
	static_assert(sizeof(ExtraRoom) == 0x20);
	static_assert(offsetof(ExtraRoom, unk18) == 0x18);

	class ExtraRoomRefData : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kRoomRefData };
		ExtraRoomRefData() = delete;
		/* 0x018 */ std::uint8_t unk18[0x08];
	};
	static_assert(sizeof(ExtraRoomRefData) == 0x20);
	static_assert(offsetof(ExtraRoomRefData, unk18) == 0x18);

	class ExtraNavMeshPortal : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kNavmeshPortal };
		ExtraNavMeshPortal() = delete;
		/* 0x018 */ std::uint8_t unk18[0x10];
	};
	static_assert(sizeof(ExtraNavMeshPortal) == 0x28);
	static_assert(offsetof(ExtraNavMeshPortal, unk18) == 0x18);

	class ExtraCollisionData : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kCollisionData };
		ExtraCollisionData() = delete;
		/* 0x018 */ std::uint8_t unk18[0x08];
	};
	static_assert(sizeof(ExtraCollisionData) == 0x20);
	static_assert(offsetof(ExtraCollisionData, unk18) == 0x18);

	class ExtraPreVisRefs : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kCellPrevisRefs };
		ExtraPreVisRefs() = delete;
		/* 0x018 */ std::uint8_t unk18[0x38];
	};
	static_assert(sizeof(ExtraPreVisRefs) == 0x50);
	static_assert(offsetof(ExtraPreVisRefs, unk18) == 0x18);

	class ExtraCombinedRefs : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kCellCombinedRefs };
		ExtraCombinedRefs() = delete;
		/* 0x018 */ std::uint8_t unk18[0x50];
	};
	static_assert(sizeof(ExtraCombinedRefs) == 0x68);
	static_assert(offsetof(ExtraCombinedRefs, unk18) == 0x18);

	class ExtraInteriorLODWorldspace : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kInteriorLODWorldspace };
		ExtraInteriorLODWorldspace() = delete;
		/* 0x018 */ std::uint8_t unk18[0x18];
	};
	static_assert(sizeof(ExtraInteriorLODWorldspace) == 0x30);
	static_assert(offsetof(ExtraInteriorLODWorldspace, unk18) == 0x18);

	class ExtraReflectedRefs : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kReflectedRefs };
		ExtraReflectedRefs() = delete;
		/* 0x018 */ std::uint8_t unk18[0x18];
	};
	static_assert(sizeof(ExtraReflectedRefs) == 0x30);
	static_assert(offsetof(ExtraReflectedRefs, unk18) == 0x18);

	class ExtraReflectorRefs : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kReflectorRefs };
		ExtraReflectorRefs() = delete;
		/* 0x018 */ std::uint8_t unk18[0x18];
	};
	static_assert(sizeof(ExtraReflectorRefs) == 0x30);
	static_assert(offsetof(ExtraReflectorRefs, unk18) == 0x18);

	class ExtraBendableSplineParams : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kBendableSplineParams };
		ExtraBendableSplineParams() = delete;
		/* 0x018 */ std::uint8_t unk18[0x18];
	};
	static_assert(sizeof(ExtraBendableSplineParams) == 0x30);
	static_assert(offsetof(ExtraBendableSplineParams, unk18) == 0x18);

	class ExtraGroupConstraint : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kGroupConstraint };
		ExtraGroupConstraint() = delete;
		/* 0x018 */ std::uint8_t unk18[0x38];
	};
	static_assert(sizeof(ExtraGroupConstraint) == 0x50);
	static_assert(offsetof(ExtraGroupConstraint, unk18) == 0x18);

	class ExtraAcousticParent : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kAcousticParent };
		ExtraAcousticParent() = delete;
		/* 0x018 */ std::uint8_t unk18[0x08];
	};
	static_assert(sizeof(ExtraAcousticParent) == 0x20);
	static_assert(offsetof(ExtraAcousticParent, unk18) == 0x18);

	class ExtraProjectedDecalData : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kProjectedDecalRef };
		ExtraProjectedDecalData() = delete;
		/* 0x018 */ std::uint8_t unk18[0x08];
	};
	static_assert(sizeof(ExtraProjectedDecalData) == 0x20);
	static_assert(offsetof(ExtraProjectedDecalData, unk18) == 0x18);

	class ExtraDecalGroup : public BSExtraData
	{
	public:
		static constexpr auto TYPE{ kDecalGroup };
		ExtraDecalGroup() = delete;
		/* 0x018 */ std::uint8_t unk18[0x08];
	};
	static_assert(sizeof(ExtraDecalGroup) == 0x20);
	static_assert(offsetof(ExtraDecalGroup, unk18) == 0x18);
}
