// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Enum.h>
#include <CKPE.EnumSet.h>
#include <CKPE.Common.h>
#include <EditorAPI/NiAPI/NiTypes.h>
#include <EditorAPI/BGSLocalizedString.h>
#include <EditorAPI/TESFullName.h>
#include <EditorAPI/BSSpinLock.h> 
#include "TESForm.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESObjectLAND;
				class TESObjectREFR;

				// class upd... no longer suitable for 1.5.3, 1.5.73
				// size 0x88 - now 0x90
				// func 101
				class TESObjectCELL : public TESForm, public TESFullName
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftCell;
					// 10
					struct Item
					{
						TESObjectREFR* Refr;
						void* Unk;
					};
					// 08
					struct CellCoordinates { std::int32_t X, Y; };
					// 8C
					struct LightingData
					{
						/*00*/ NiAPI::NiRGBA	ambient;
						/*04*/ NiAPI::NiRGBA	directional;
						/*08*/ NiAPI::NiRGBA	fogNearColor;
						/*0C*/ float			fogNear;
						/*10*/ float			fogFar;
						/*14*/ std::uint32_t	Unk14;
						/*18*/ std::uint32_t	Unk18;
						/*1C*/ std::uint32_t	Unk1C;
						/*20*/ std::uint32_t	Unk20;
						/*24*/ float			fogPow;

						// 18
						struct DirectionalAmbientLightingData
						{
							/*00*/ NiAPI::NiRGBA Xplus;
							/*04*/ NiAPI::NiRGBA Xminus;
							/*08*/ NiAPI::NiRGBA Yplus;
							/*0C*/ NiAPI::NiRGBA Yminus;
							/*10*/ NiAPI::NiRGBA Zplus;
							/*14*/ NiAPI::NiRGBA Zminus;
						};

						/*28*/ DirectionalAmbientLightingData DirectionalAmbientLighting;

						/*40*/ std::uint32_t	Unk40;
						/*44*/ float			Unk44;
						/*48*/ NiAPI::NiRGBA	fogFarColor;
						/*4C*/ float			fogMax;
						/*50*/ std::uint32_t	Unk50;
						/*54*/ std::uint32_t	Unk54;
						/*58*/ std::uint32_t	Unk58;
						/*5C*/ float			fogNearHeightMid;
						/*60*/ float			fogNearHeightRange;
						/*64*/ NiAPI::NiRGBA	fogHighNearColor;
						/*68*/ NiAPI::NiRGBA	fogHighFarColor;
						/*6C*/ float			fogHighDensityScale;
						/*70*/ float			fogScaleNear;
						/*74*/ float			fogScaleFar;
						/*78*/ float			fogScaleHighNear;
						/*7C*/ float			fogScaleHighFar;
						/*80*/ float			fogFarHeightMid;
						/*84*/ float			fogFarHeightRange;
					};
					// 08
					union CellData
					{
						/*00*/ CellCoordinates* Grid;					// if exterior
						/*00*/ LightingData* Lighting;					// if interior
					};
				private:
					enum class CellFlags : std::uint16_t
					{
						kNone = 0,
						kIsInteriorCell = 1 << 0,
						kHasWater = 1 << 1,
						kInvertCanTravelFromHere = 1 << 2,
						kNoLODWater = 1 << 3,
						kHasTempData = 1 << 4,
						kPublicArea = 1 << 5,
						kHandChanged = 1 << 6,
						kShowSky = 1 << 7,
						kUseSkyLighting = 1 << 8,
						kWarnToLeave = 1 << 9
					};

					enum class CellProcessLevels : std::uint8_t
					{
						cplNotLoaded,									// default value
						cplUnloading,
						cplLoadingData,
						cplLoading,
						cplLoaded,										// loaded cells that are not processed
						cplDetaching,
						cplAttachQueued,
						cplAttaching,
						cplAttached										// current interior cell, or exterior cells within fixed radius of current exterior cell						
					};
				public:
					struct RecordFlags
					{
						enum RecordFlag : std::uint32_t
						{
							kDeleted = 1 << 5,
							kPersistent = 1 << 10,
							kIgnored = 1 << 12,
							kOffLimits = 1 << 17,
							kCompressed = 1 << 18,
							kCantWait = 1 << 19
						};
					};

					virtual ~TESObjectCELL() = default;

					[[nodiscard]] inline bool IsAttached() const noexcept(true) { return cellProcessLevels == CellProcessLevels::cplAttached; }
					[[nodiscard]] inline bool IsLoaded() const noexcept(true) { return cellProcessLevels == CellProcessLevels::cplLoaded; }
					[[nodiscard]] inline bool HasFastTravel() const noexcept(true)
					{
						return HasInterior() ? cellFlags.any(CellFlags::kInvertCanTravelFromHere) : cellFlags.none(CellFlags::kInvertCanTravelFromHere);
					}
					[[nodiscard]] inline bool HasInterior() const noexcept(true) { return cellFlags.any(CellFlags::kIsInteriorCell); }
					[[nodiscard]] inline bool HasWater() const noexcept(true) { return cellFlags.any(CellFlags::kHasWater); }
					[[nodiscard]] inline bool HasHandChanged() const noexcept(true) { return cellFlags.any(CellFlags::kHandChanged); }
					[[nodiscard]] inline std::int32_t GetGridX() const noexcept(true)
					{
						if (HasInterior()) return 0;
						auto Grid = (VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_1_5_73) ?
							difference.v1_5._CellData.Grid : difference.v1_6._CellData.Grid;
						return Grid ? Grid->X : 0;
					}
					[[nodiscard]] inline std::int32_t GetGridY() const noexcept(true)
					{
						if (HasInterior()) return 0;
						auto Grid = (VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_1_5_73) ?
							difference.v1_5._CellData.Grid : difference.v1_6._CellData.Grid;
						return Grid ? Grid->Y : 0;
					}
					[[nodiscard]] inline TESFormArray* GetNavMeshes() const noexcept(true) 
					{
						return (VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_1_5_73) ?
							difference.v1_5._NavMeshes : difference.v1_6._NavMeshes;
					}
					[[nodiscard]] inline std::uint32_t GetNavMeshesCount() const  noexcept(true) 
					{
						return (VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_1_5_73) ?
							difference.v1_5._NavMeshes->size() : difference.v1_6._NavMeshes->size();
					}
					[[nodiscard]] inline TESObjectLAND* GetLandspace() const noexcept(true) 
					{
						return (VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_1_5_73) ?
							difference.v1_5._Landspace : difference.v1_6._Landspace;
					}
					[[nodiscard]] inline LightingData* GetLighting() const noexcept(true) 
					{
						if (!HasInterior()) return nullptr;
						auto Lighting = (VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_1_5_73) ?
							difference.v1_5._CellData.Lighting : difference.v1_6._CellData.Lighting;
						return Lighting;
					}
					[[nodiscard]] inline Item* GetItems() const noexcept(true) 
					{
						return (VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_1_5_73) ?
							(Item*)difference.v1_5._AddrRefrs : (Item*)difference.v1_6._AddrRefrs;
					}
					[[nodiscard]] inline std::uint32_t GetItemCount() const noexcept(true) 
					{ 
						return (VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_1_5_73) ? 
							difference.v1_5._SizeRefrs : difference.v1_6._SizeRefrs;
					}
				public:
					CKPE_READ_PROPERTY(GetFullName) const char* FullName;
					CKPE_READ_PROPERTY(GetLandspace) TESObjectLAND* Landspace;
					CKPE_READ_PROPERTY(GetGridX) std::int32_t GridX;
					CKPE_READ_PROPERTY(GetGridY) std::int32_t GridY;
					CKPE_READ_PROPERTY(GetLighting) LightingData* Lighting;
				private:
					// members
					mutable BSSpinLock grassCreateLock;
					mutable BSSpinLock grassTaskLock;
					TEnumSet<CellFlags, std::uint16_t> cellFlags;
					TEnum<CellProcessLevels, std::uint8_t> cellProcessLevels;
					bool autoWaterLoaded;
					std::uint8_t pad054;
					bool cellDetached;
					std::uint8_t pad056[2];
#define RUNTIME_DATA_CONTENT	\
					void* _ExtraData;					\
					TESForm* Imagespace;				\
					CellData _CellData;					\
					TESObjectLAND* _Landspace;			\
					float waterHeight;					\
					TESFormArray* _NavMeshes;			\
					char _pad90[0xC];					\
					std::uint32_t _SizeRefrs;			\
					char _padA0[0x4];					\
					std::uint32_t _TotalRefrs;			\
					char _padA8[0x8];					\
					std::uint32_t _AllocateSizeRefrs;	\
					char _padB4[0xC];					\
					Item* _AddrRefrs;					\

					union
					{
						struct _v1_5
						{
							RUNTIME_DATA_CONTENT
						} v1_5;
						struct _v1_6
						{
							char _pad58[0x8];	// 0x8 added with 1.6.1130 (maybe ExtraData without ref)
							RUNTIME_DATA_CONTENT
						} v1_6;
					} difference;

#undef RUNTIME_DATA_CONTENT
				};
				static_assert(sizeof(TESObjectCELL) == 0xC8);
			}
		}
	}
}