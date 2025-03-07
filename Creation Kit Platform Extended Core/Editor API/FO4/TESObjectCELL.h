//////////////////////////////////////////
/*
* Copyright (c) 2020-2022 Perchik71 <email:perchik71@outlook.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this
* software and associated documentation files (the "Software"), to deal in the Software
* without restriction, including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

#pragma once
#pragma pack(push, 1)

#include "TESObjectREFR.h"
#include "NiAPI\NiTypes.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			class TESObjectLAND;
			class TESObjectCELL : public TESForm {
			public:
				constexpr static uint8_t TYPE_ID = ftCell;
				// 08
				struct CellCoordinates { int32_t X, Y; };
				// 8C
				struct LightingData {
					/*00*/ NiRGBA		ambient;
					/*04*/ NiRGBA		directional;
					/*08*/ NiRGBA		fogNearColor;
					/*0C*/ float		fogNear;
					/*10*/ float		fogFar;
					/*14*/ uint32_t		Unk14;
					/*18*/ uint32_t		Unk18;
					/*1C*/ uint32_t		Unk1C;
					/*20*/ uint32_t		Unk20;
					/*24*/ float		fogPow;

					// 18
					struct DirectionalAmbientLightingData {
						/*00*/ NiRGBA Xplus;
						/*04*/ NiRGBA Xminus;
						/*08*/ NiRGBA Yplus;
						/*0C*/ NiRGBA Yminus;
						/*10*/ NiRGBA Zplus;
						/*14*/ NiRGBA Zminus;
					};

					/*28*/ DirectionalAmbientLightingData DirectionalAmbientLighting;

					/*40*/ uint32_t		Unk40;
					/*44*/ float		Unk44;
					/*48*/ NiRGBA		fogFarColor;
					/*4C*/ float		fogMax;
					/*50*/ uint32_t		Unk50;
					/*54*/ uint32_t		Unk54;
					/*58*/ uint32_t		Unk58;
					/*5C*/ float		fogNearHeightMid;
					/*60*/ float		fogNearHeightRange;
					/*64*/ NiRGBA		fogHighNearColor;
					/*68*/ NiRGBA		fogHighFarColor;
					/*6C*/ float		fogHighDensityScale;
					/*70*/ float		fogScaleNear;
					/*74*/ float		fogScaleFar;
					/*78*/ float		fogScaleHighNear;
					/*7C*/ float		fogScaleHighFar;
					/*80*/ float		fogFarHeightMid;
					/*84*/ float		fogFarHeightRange;
				};
				// 08
				union CellData {
					/*00*/ CellCoordinates* Grid;					// if exterior
					/*00*/ LightingData* Lighting;					// if interior
				};
			private:
				enum CellFlags : uint16_t {
					cfInterior = 0x1,
					cfExterior = 0x2,
					cfInvertFastTravel = 0x4,						// interiors: can travel, exteriors: cannot travel
					cfFragment = 0x40,								// (exteriors only) Exists Ext01, ... ExtXX
				};
				enum CellProcessLevels : uint16_t {
					cplNone = 0x0,									// default value
					cplLoaded = 0x4,								// loaded cells that are not processed
					cplActive = 0x8,								// current interior cell, or exterior cells within fixed radius of current exterior cell
				};
			private:
				TESFullName _fullName;
				char _pad1[0x10];
				CellFlags _cell_flags;
				CellProcessLevels _cell_process_level;
				char Unk54[0x1];									// set 0x1 when unloaded cell
				char _pad2[0x3];
				void* _extradata;
				CellData _celldata;
				TESObjectLAND* _landspace;
				char _pad3[0x8];
				TESFormArray* _navmeshes;
				BSTArray<TESObjectREFR_Original*> _items;
			public:
				inline const TESObjectREFR_Original** GetItemsConst() const { return (const TESObjectREFR_Original**)_items.QBuffer(); }
				inline TESObjectREFR_Original** GetItems() { return (TESObjectREFR_Original**)_items.QBuffer(); }
				inline uint32_t GetItemsCount() const { return (uint32_t)_items.QSize(); }
				inline const TESObjectLAND* GetLandspaceConst() const { return _landspace; }
				inline TESObjectLAND* GetLandspace() { return _landspace; }
				inline bool IsNavMesh() const { return _navmeshes != NULL; }
				inline uint32_t GetNavMeshesCount() const { return (uint32_t)_navmeshes->QSize(); }
				inline const TESForm** GetNavMeshesConst() const { return (const TESForm**)_navmeshes->QBuffer(); }
				inline TESForm** GetNavMeshes() { return (TESForm**)_navmeshes->QBuffer(); }
				inline int32_t GetGridX() const { return (IsExterior()) ? _celldata.Grid->X : 0; } // 0xDB3470
				inline int32_t GetGridY() const { return (IsExterior()) ? _celldata.Grid->Y : 0; } // 0xDB3490
				inline LightingData* GetLighting() { return (IsInterior()) ? _celldata.Lighting : NULL; }
				inline bool IsActive() const { return _cell_process_level == CellProcessLevels::cplActive; }
				inline bool IsLoaded() const { return _cell_process_level == CellProcessLevels::cplLoaded; }
				inline bool IsFastTravelEnabled() const {
					return IsInterior() ? (CellFlags::cfInvertFastTravel & _cell_flags) :
						((CellFlags::cfInvertFastTravel & _cell_flags) != CellFlags::cfInvertFastTravel);
				}
				inline bool IsInterior() const { return (_cell_flags & CellFlags::cfInterior) == CellFlags::cfInterior; }
				inline bool IsExterior() const { return (_cell_flags & CellFlags::cfExterior) == CellFlags::cfExterior; }
				inline bool IsFragment() const { return (_cell_flags & CellFlags::cfFragment) == CellFlags::cfFragment; }
			public:
				READ_PROPERTY(GetNavMeshes) TESForm** NavMeshes;
				READ_PROPERTY(GetNavMeshesConst) const TESForm** NavMeshesConst;
				READ_PROPERTY(GetNavMeshesCount) uint32_t NavMeshesCount;
				READ_PROPERTY(GetItems) TESObjectREFR_Original** Items;
				READ_PROPERTY(GetItemsConst) const TESObjectREFR_Original** ItemsConst;
				READ_PROPERTY(GetItemsCount) uint32_t ItemsCount;
				READ_PROPERTY(GetLandspaceConst) const TESObjectLAND* LandspaceConst;
				READ_PROPERTY(GetLandspace) TESObjectLAND* Landspace;
				READ_PROPERTY(GetGridX) int32_t GridX;
				READ_PROPERTY(GetGridY) int32_t GridY;
				READ_PROPERTY(GetLighting) LightingData* Lighting;
			public:
				virtual ~TESObjectCELL() = default;
			};
		}
	}
}

#pragma pack(pop)