// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BSTHashMap.h>
#include <EditorAPI/T/TESFormIDListView.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESTexture.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESObjectCELL;
				class BGSLightingTemplate;

				class TESWaterForm;

				class BGSEncounterZone;

				class BGSLocation;

				class BGSMusicType;

				class TESClimate;

				struct WORLD_MAP_DATA
				{
					std::uint32_t usableWidth; // 00
					std::uint32_t usableHeight; // 04
					std::int16_t nwCellX; // 08
					std::int16_t nwCellY; // 0A
					std::int16_t seCellX; // 0C
					std::int16_t seCellY; // 0E
				};
				static_assert(sizeof(WORLD_MAP_DATA) == 0x10);

				struct WORLD_MAP_OFFSET_DATA
				{
					float mapScale; // 00
					float mapOffsetX; // 04
					float mapOffsetY; // 08
					float mapOffsetZ; // 0C
				};
				static_assert(sizeof(WORLD_MAP_OFFSET_DATA) == 0x10);

				// 1.11.240.0: size 0x2B8, vtable RVA 0x30BABA8, 126 virtual functions
				class TESWorldSpace :
					public TESFormIDListView,  // 000
					public TESFullName,        // 028
					public TESTexture          // 040
				{
				public:
					virtual ~TESWorldSpace() = default;

				public:
					// BSTHashMap<std::int32_t, TESObjectCELL*> in the CK's 0x38 byte table layout: the constructor puts the end of chain marker at 80,
					// the capacity/free/good counters at 74/78/7C, the modification count at 88 and the entries (24 bytes each) at 98. The position is
					// CommonLibF4's cellMap (0x40) shifted by the TESTexture base (+0x28).
					BSTHashMap38<std::int32_t, TESObjectCELL*> cellMap; // 68
					void* persistentCell; // A0 - TESObjectCELL*
					void* terrainManager; // A8 - BGSTerrainManager*
					TESClimate* climate; // B0
					std::int8_t flags; // B8 - DATA
					std::uint8_t padB9; // B9
					std::uint16_t parentUseFlags; // BA - REX::TEnumSet<PARENT_USE_FLAG, std::uint16_t>
					std::int8_t worldFlags; // BC
					std::uint8_t padBD; // BD
					std::int16_t fixedCenterX; // BE - ShortPoint fixedCenter
					std::int16_t fixedCenterY; // C0
					std::uint8_t padC2[0x6]; // C2
					std::uint64_t unkC8; // C8
					std::uint64_t unkD0; // D0
					std::uint64_t unkD8; // D8
					std::uint64_t unkE0; // E0
					std::uint64_t unkE8; // E8
					TESWorldSpace* parentWorld; // F0
					BGSLightingTemplate* lightingTemplate; // F8
					TESWaterForm* worldWater; // 100
					TESWaterForm* lodWater; // 108
					float lodWaterHeight; // 110
					WORLD_MAP_DATA worldMapData; // 114
					WORLD_MAP_OFFSET_DATA worldMapOffsetData; // 124
					std::uint8_t unk134[0x4]; // 134
					BGSMusicType* musicType; // 138
					NiAPI::NiPoint2 minimumCoords; // 140
					NiAPI::NiPoint2 maximumCoords; // 148
					float defaultLandHeight; // 150
					float defaultWaterHeight; // 154
					float distantLODMult; // 158
					std::uint32_t unk15C; // 15C
					BGSEncounterZone* encounterZone; // 160
					BGSLocation* location; // 168
					std::uint64_t unk170; // 170
					TESTexture canopyShadowTexture; // 178
					TESTexture waterEnvMap; // 1A0
					TESTexture unk1C8; // 1C8
					TESTexture unk1F0; // 1F0
					// A second hash map in the CK's 0x38 byte layout (end of chain marker at 230, entries at 248); key and value types are not identified.
					std::uint8_t unk218[0x38]; // 218
					BSTArray<void*> unk250; // 250 - element type not identified yet
					std::uint8_t largeRefData[0x38]; // 268 - BGSLargeRefData (0x38 in the CK)
					std::uint32_t unk2A0; // 2A0
					std::uint8_t lvlDataCellStartX; // 2A4
					std::uint8_t lvlDataCellStartY; // 2A5
					std::uint8_t lvlDataCellWidthX; // 2A6
					std::uint8_t lvlDataCellWidthY; // 2A7
					char* lvlDataCellA; // 2A8
					char* maxHeightData; // 2B0
				};
				static_assert(offsetof(TESWorldSpace, cellMap) == 0x68);
				static_assert(offsetof(TESWorldSpace, unk218) == 0x218);
				static_assert(offsetof(TESWorldSpace, persistentCell) == 0xA0);
				static_assert(offsetof(TESWorldSpace, fixedCenterX) == 0xBE);
				static_assert(sizeof(TESWorldSpace) == 0x2B8);
			}
		}
	}
}
