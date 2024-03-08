// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "NiAPI/NiTypes.h"
#include "TESForm.h"
#include "BGSLocalizedString.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			class TESObjectLAND;

			// size 0x88
			// func 101
			class TESObjectCELL: public TESForm
			{
			public:
				constexpr static uint8_t TYPE_ID = ftCell;
				// 08
				struct CellCoordinates { int32_t X, Y; };
				// 8C
				struct LightingData 
				{
					/*00*/ NiAPI::NiRGBA	ambient;
					/*04*/ NiAPI::NiRGBA	directional;
					/*08*/ NiAPI::NiRGBA	fogNearColor;
					/*0C*/ float			fogNear;
					/*10*/ float			fogFar;
					/*14*/ uint32_t			Unk14;
					/*18*/ uint32_t			Unk18;
					/*1C*/ uint32_t			Unk1C;
					/*20*/ uint32_t			Unk20;
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

					/*40*/ uint32_t			Unk40;
					/*44*/ float			Unk44;
					/*48*/ NiAPI::NiRGBA	fogFarColor;
					/*4C*/ float			fogMax;
					/*50*/ uint32_t			Unk50;
					/*54*/ uint32_t			Unk54;
					/*58*/ uint32_t			Unk58;
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
				enum CellFlags : uint16_t 
				{
					cfInterior = 0x1,
					cfExterior = 0x2,
					cfInvertFastTravel = 0x4,						// interiors: can travel, exteriors: cannot travel
					cfFragment = 0x40,								// (exteriors only) Exists Ext01, ... ExtXX
				};

				enum CellProcessLevels : uint16_t 
				{
					cplNone = 0x0,									// default value
					cplLoaded = 0x4,								// loaded cells that are not processed
					cplActive = 0x8,								// current interior cell, or exterior cells within fixed radius of current exterior cell
				};
			public:
				virtual ~TESObjectCELL() = default;

				inline const char* GetFullName() const 
				{
					auto Ret = _FullName.c_str(); 
					return Ret ? Ret : "";
				}
				inline bool IsActive() const { return _cell_process_level == CellProcessLevels::cplActive; }
				inline bool IsLoaded() const { return _cell_process_level == CellProcessLevels::cplLoaded; }
				inline bool IsFastTravelEnabled() const 
				{
					return IsInterior() ? (CellFlags::cfInvertFastTravel & _cell_flags) :
						((CellFlags::cfInvertFastTravel & _cell_flags) != CellFlags::cfInvertFastTravel);
				}
				inline bool IsInterior() const { return _cell_flags & CellFlags::cfInterior; }
				inline bool IsExterior() const { return _cell_flags & CellFlags::cfExterior; }
				inline bool IsFragment() const { return _cell_flags & CellFlags::cfFragment; }
				inline int32_t GetGridX() const { return (IsExterior()) ? _CellData.Grid->X : 0; }
				inline int32_t GetGridY() const { return (IsExterior()) ? _CellData.Grid->Y : 0; }
				inline TESFormArray* GetNavMeshes() { return _NavMeshes; }
				inline uint32_t GetNavMeshesCount() const { return _NavMeshes->QSize(); }
				inline TESObjectLAND* GetLandspace() { return _Landspace; }
				inline LightingData* GetLighting() { return (IsInterior()) ? _CellData.Lighting : nullptr; }
			public:
				READ_PROPERTY(GetFullName) const char* FullName;
				READ_PROPERTY(GetLandspace) TESObjectLAND* Landspace;
				READ_PROPERTY(GetGridX) int32_t GridX;
				READ_PROPERTY(GetGridY) int32_t GridY;
				READ_PROPERTY(GetLighting) LightingData* Lighting;
			private:
				char pad28[0x8];
				BGSLocalizedString _FullName;
				char pad40[0x10];
				CellFlags _cell_flags;
				CellProcessLevels _cell_process_level;
				char _pad54[0x4];
				void* _ExtraData;
				char _pad60[0x8];
				CellData _CellData;
				TESObjectLAND* _Landspace;
				char _pad78[0x8];
				TESFormArray* _NavMeshes;
			};
			static_assert(sizeof(TESObjectCELL) == 0x88);
		}
	}
}