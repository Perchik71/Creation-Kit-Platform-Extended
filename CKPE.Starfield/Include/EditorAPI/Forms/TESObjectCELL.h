// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "TESObjectREFR.h"
#include "NiAPI\NiTypes.h"

#pragma once
#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Starfield
		{
			using namespace NiAPI;

			class TESObjectCELL : public TESForm
			{
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
				enum CellFlags : uint32_t
				{
					cfInterior = 0x1,
					cfExterior = 0x2,
					cfInvertFastTravel = 0x4,			// interiors: can travel, exteriors: cannot travel
				};
				enum CellProcessLevels : uint32_t
				{
					cplNone = 0x0,						// default value
					cplLoaded = 0x4,					// loaded cells that are not processed
					cplActive = 0x10,					// current interior cell, or exterior cells within fixed radius of current exterior cell
				};
			private:
				char pad8A[0xCE];
				BGSLocalizedString<BSEntryString> _FullName;
				CellFlags _cell_flags;
				CellProcessLevels _cell_process_level;
				void* _extradata;
				CellData _celldata;
			public:
				[[nodiscard]] inline bool IsActive() const noexcept(true) { return _cell_process_level == CellProcessLevels::cplActive; }
				[[nodiscard]] inline bool IsLoaded() const noexcept(true) { return _cell_process_level == CellProcessLevels::cplLoaded; }

				[[nodiscard]] inline bool IsFastTravelEnabled() const noexcept(true)
				{
					return IsInterior() ? (CellFlags::cfInvertFastTravel & _cell_flags) :
						((CellFlags::cfInvertFastTravel & _cell_flags) != CellFlags::cfInvertFastTravel);
				}
				[[nodiscard]] inline bool IsInterior() const noexcept(true) { return _cell_flags & CellFlags::cfInterior; }
				[[nodiscard]] inline bool IsExterior() const noexcept(true) { return _cell_flags & CellFlags::cfExterior; }
				[[nodiscard]] inline LightingData* GetLighting() const noexcept(true) { return (IsInterior()) ? _celldata.Lighting : nullptr; }
			public:
				virtual ~TESObjectCELL() = default;

				//BGSLocalizedString
			};
			static_assert(sizeof(TESObjectCELL) == 0x180);
		}
	}
}

#pragma pack(pop)