// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESModel.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 0x20 bytes, layout from CommonLibF4 GRASS_DATA, offsets confirmed by the CK getters/setters (data at 0xC8)
				struct GRASS_DATA
				{
					std::int8_t density;					// 00
					std::int8_t minSlopeDegrees;			// 01
					std::int8_t maxSlopeDegrees;			// 02
					std::uint8_t pad03;						// 03
					std::uint16_t distanceFromWaterLevel;	// 04
					std::uint16_t pad06;					// 06
					std::int32_t underwater;				// 08 - GRASS_WATER_STATE
					float positionRange;					// 0C
					float heightRange;						// 10
					float colorRange;						// 14
					float wavePeriod;						// 18
					std::int8_t flags;						// 1C - 1 vertex lighting, 2 uniform scaling, 4 fit to slope
					std::uint8_t pad1D[3];					// 1D
				};
				static_assert(sizeof(GRASS_DATA) == 0x20);


				// 1.11.240.0: size 0xE8, vtable RVA 0x308BF10, 180 virtual functions
				class TESGrass :
					public TESBoundObject,  // 000
					public TESModel         // 090
				{
				public:
					virtual ~TESGrass() = default;

					constexpr static std::uint8_t TYPE_ID = ftGrass;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESGrass[0];

					virtual std::int8_t GetDensity() const noexcept(true) { return 0; } // 154 - RVA 0xC49EE0, return *(unsigned __int8 *)(a1 + 200);
					virtual bool SetDensity(std::int8_t nDensity) noexcept(true) { return false; } // 155 - RVA 0xC49840, if ( a2 > 0x64u ); return 0;; *(_BYTE *)(a1 + 200) = a2;; return 1;
					virtual std::int8_t GetMinSlopeDegrees() const noexcept(true) { return 0; } // 156 - RVA 0xC49F70, return *(unsigned __int8 *)(a1 + 201);
					virtual bool SetMinSlopeDegrees(std::int8_t nDegrees) noexcept(true) { return false; } // 157 - RVA 0x16031
					virtual std::int8_t GetMaxSlopeDegrees() const noexcept(true) { return 0; } // 158 - RVA 0xC49F40, return *(unsigned __int8 *)(a1 + 202);
					virtual bool SetMaxSlopeDegrees(std::int8_t nDegrees) noexcept(true) { return false; } // 159 - RVA 0xC49880, if ( a2 > 0x5Au || a2 < *(_BYTE *)(a1 + 201) ); return 0;; *(_BYTE *)(a1 + 202) = a2;; ret
					virtual float GetMinSlope() const noexcept(true) { return 0.0f; } // 160 - RVA 0xC49F50, return (float)*(unsigned __int8 *)(a1 + 201) * 0.017453292;
					virtual float GetMaxSlope() const noexcept(true) { return 0.0f; } // 161 - RVA 0xC49F20, return (float)*(unsigned __int8 *)(a1 + 202) * 0.017453292;
					virtual std::uint16_t GetDistanceFromWaterLevel() const noexcept(true) { return 0; } // 162 - RVA 0xC49EF0, return *(unsigned __int16 *)(a1 + 204);
					virtual void SetDistanceFromWaterLevel(std::uint16_t nDistance) noexcept(true) { return; } // 163 - RVA 0xC49FD0
					virtual std::uint32_t GetUnderwaterState() const noexcept(true) { return 0; } // 164 - RVA 0xC49F90, return *(unsigned int *)(a1 + 208);
					virtual void SetUnderwaterState(std::uint32_t nState) noexcept(true) { return; } // 165 - RVA 0xC4A010, *((_DWORD *)this + 52) = a2;
					virtual float GetPositionRange() const noexcept(true) { return 0.0f; } // 166 - RVA 0xC49F80, return *(float *)(a1 + 212);
					virtual bool SetPositionRange(float fRange) noexcept(true) { return false; } // 167 - RVA 0xC498A0, if ( a2 < 0.0 || a2 > 512.0 ); return 0;; *(float *)(a1 + 212) = a2;; return 1;
					virtual float GetHeightRange() const noexcept(true) { return 0.0f; } // 168 - RVA 0xC49F10, return *(float *)(a1 + 216);
					virtual bool SetHeightRange(float fRange) noexcept(true) { return false; } // 169 - RVA 0xC498D0, if ( a2 < 0.0 || a2 > 1.0 ); return 0;; *(float *)(a1 + 216) = a2;; return 1;
					virtual float GetColorRange() const noexcept(true) { return 0.0f; } // 170 - RVA 0xC49ED0, return *(float *)(a1 + 220);
					virtual bool SetColorRange(float fRange) noexcept(true) { return false; } // 171 - RVA 0xC49900, if ( a2 < 0.0 || a2 > 1.0 ); return 0;; *(float *)(a1 + 220) = a2;; return 1;
					virtual float GetWavePeriod() const noexcept(true) { return 0.0f; } // 172 - RVA 0xC49FC0, return *(float *)(a1 + 224);
					virtual bool SetWavePeriod(float fPeriod) noexcept(true) { return false; } // 173 - RVA 0xC49930, if ( a2 <= 0.0 ); return 0;; *(float *)(a1 + 224) = a2;; return 1;
					virtual bool GetVertexLighting() const noexcept(true) { return false; } // 174 - RVA 0xC49FB0, return *(_BYTE *)(a1 + 228) & 1;
					virtual void SetVertexLighting(bool bFlag) noexcept(true) { return; } // 175 - RVA 0xC4A050, v2 = *(_BYTE *)(a1 + 228);; v3 = v2 | 1;; result = v2 & 0xFE;; if ( !a2 ); v3 = result;; *
					virtual bool GetUniformScaling() const noexcept(true) { return false; } // 176 - RVA 0xC49FA0, return (*(_BYTE *)(a1 + 228) & 2) != 0;
					virtual void SetUniformScaling(bool bFlag) noexcept(true) { return; } // 177 - RVA 0xC4A020, v2 = *(_BYTE *)(a1 + 228);; v3 = v2 | 2;; result = v2 & 0xFD;; if ( !a2 ); v3 = result;; *
					virtual bool GetFitToSlope() const noexcept(true) { return false; } // 178 - RVA 0xC49F00, return (*(_BYTE *)(a1 + 228) & 4) != 0;
					virtual void SetFitToSlope(bool bFlag) noexcept(true) { return; } // 179 - RVA 0xC49FE0, v2 = *(_BYTE *)(a1 + 228);; v3 = v2 | 4;; result = v2 & 0xFB;; if ( !a2 ); v3 = result;; *

				public:
					GRASS_DATA data; // C8
				};
				static_assert(sizeof(TESGrass) == 0xE8);
			}
		}
	}
}
