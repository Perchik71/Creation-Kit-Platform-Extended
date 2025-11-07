// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.h>
#include <EditorAPI/NiAPI/NiFlags.h>
#include "TESObjectLAND.h"
#include "TESObjectREFR.h"
#include <EditorAPI/TESFullName.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESObjectCELL : public TESForm, public TESFullName
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftCell;
					// 08
					struct CellCoordinates { std::int32_t X, Y; };
					// 8C
					struct LightingData 
					{
						/*00*/ NiAPI::NiRGBA		ambient;
						/*04*/ NiAPI::NiRGBA		directional;
						/*08*/ NiAPI::NiRGBA		fogNearColor;
						/*0C*/ float				fogNear;
						/*10*/ float				fogFar;
						/*14*/ std::uint32_t		Unk14;
						/*18*/ std::uint32_t		Unk18;
						/*1C*/ std::uint32_t		Unk1C;
						/*20*/ std::uint32_t		Unk20;
						/*24*/ float				fogPow;

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

						/*40*/ std::uint32_t		Unk40;
						/*44*/ float				Unk44;
						/*48*/ NiAPI::NiRGBA		fogFarColor;
						/*4C*/ float				fogMax;
						/*50*/ std::uint32_t		Unk50;
						/*54*/ std::uint32_t		Unk54;
						/*58*/ std::uint32_t		Unk58;
						/*5C*/ float				fogNearHeightMid;
						/*60*/ float				fogNearHeightRange;
						/*64*/ NiAPI::NiRGBA		fogHighNearColor;
						/*68*/ NiAPI::NiRGBA		fogHighFarColor;
						/*6C*/ float				fogHighDensityScale;
						/*70*/ float				fogScaleNear;
						/*74*/ float				fogScaleFar;
						/*78*/ float				fogScaleHighNear;
						/*7C*/ float				fogScaleHighFar;
						/*80*/ float				fogFarHeightMid;
						/*84*/ float				fogFarHeightRange;
					};
					// 08
					union CellData 
					{
						/*00*/ CellCoordinates* Grid;					// if exterior
						/*00*/ LightingData* Lighting;					// if interior
					};
				private:
					enum CellFlags : std::uint16_t
					{
						cfInterior = 0x1,
						cfWater = 0x2,
						cfInvertFastTravel = 0x4,						// interiors: can travel, exteriors: cannot travel
						cfFragment = 0x40,								// (exteriors only) Exists Ext01, ... ExtXX
					};
					enum CellProcessLevels : std::uint16_t
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
				private:
					char _pad1[0x10];
					NiAPI::NiTFlags<CellFlags, TESObjectCELL> _cell_flags;
					CellProcessLevels _cell_process_level;
					char Unk54[0x1];									// set 0x1 when unloaded cell
					char _pad2[0x3];
					void* _extradata;
					CellData _celldata;
					TESObjectLAND* _landspace;
					char _pad3[0x8];
					TESFormArray* _navmeshes;
					BSTArray<TESObjectREFR*> _items;
				public:
					[[nodiscard]] inline const TESObjectREFR** GetItemsConst() const noexcept(true) { return (const TESObjectREFR**)_items.data(); }
					[[nodiscard]] inline TESObjectREFR** GetItems() noexcept(true) { return (TESObjectREFR**)_items.data(); }
					[[nodiscard]] inline std::uint32_t GetItemCount() const noexcept(true) { return (std::uint32_t)_items.size(); }
					[[nodiscard]] inline const TESObjectLAND* GetLandspaceConst() const noexcept(true) { return _landspace; }
					[[nodiscard]] inline TESObjectLAND* GetLandspace() noexcept(true) { return _landspace; }
					[[nodiscard]] inline bool IsNavMesh() const noexcept(true) { return _navmeshes != nullptr; }
					[[nodiscard]] inline std::uint32_t GetNavMeshesCount() const noexcept(true) { return (std::uint32_t)_navmeshes->size(); }
					[[nodiscard]] inline const TESForm** GetNavMeshesConst() const noexcept(true) { return (const TESForm**)_navmeshes->data(); }
					[[nodiscard]] inline TESForm** GetNavMeshes() noexcept(true) { return (TESForm**)_navmeshes->data(); }
					[[nodiscard]] inline std::int32_t GetGridX() const noexcept(true) { return (!HasInterior()) ? _celldata.Grid->X : 0; } // 0xDB3470
					[[nodiscard]] inline std::int32_t GetGridY() const noexcept(true) { return (!HasInterior()) ? _celldata.Grid->Y : 0; } // 0xDB3490
					[[nodiscard]] inline LightingData* GetLighting() const noexcept(true) { return (HasInterior()) ? _celldata.Lighting : nullptr; }
					[[nodiscard]] inline bool IsAttached() const noexcept(true) { return _cell_process_level == CellProcessLevels::cplAttached; }
					[[nodiscard]] inline bool IsLoaded() const noexcept(true) { return _cell_process_level == CellProcessLevels::cplLoaded; }
					[[nodiscard]] inline bool HasFastTravel() const noexcept(true)
					{
						return HasInterior() ? _cell_flags.Has(cfInvertFastTravel) : (!_cell_flags.Has(cfInvertFastTravel));
					}
					[[nodiscard]] inline bool HasInterior() const noexcept(true) { return _cell_flags.Has(cfInterior); }
					[[nodiscard]] inline bool HasWater() const noexcept(true) { return _cell_flags.Has(cfWater); }
					[[nodiscard]] inline bool HasFragment() const noexcept(true) { return _cell_flags.Has(cfFragment); }
				public:
					CKPE_READ_PROPERTY(GetNavMeshes) TESForm** NavMeshes;
					CKPE_READ_PROPERTY(GetNavMeshesConst) const TESForm** NavMeshesConst;
					CKPE_READ_PROPERTY(GetNavMeshesCount) std::uint32_t NavMeshesCount;
					CKPE_READ_PROPERTY(GetItems) TESObjectREFR** Items;
					CKPE_READ_PROPERTY(GetItemsConst) const TESObjectREFR** ItemsConst;
					CKPE_READ_PROPERTY(GetItemCount) std::uint32_t ItemCount;
					CKPE_READ_PROPERTY(GetLandspaceConst) const TESObjectLAND* LandspaceConst;
					CKPE_READ_PROPERTY(GetLandspace) TESObjectLAND* Landspace;
					CKPE_READ_PROPERTY(GetGridX) std::int32_t GridX;
					CKPE_READ_PROPERTY(GetGridY) std::int32_t GridY;
					CKPE_READ_PROPERTY(GetLighting) LightingData* Lighting;
				public:
					virtual ~TESObjectCELL() = default;
				};
			}
		}
	}
}