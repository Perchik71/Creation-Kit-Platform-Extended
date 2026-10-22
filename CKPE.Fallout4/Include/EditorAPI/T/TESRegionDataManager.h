// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESRegion.h>
#include <EditorAPI/T/TESFile.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class TESRegionData;

			// Region data (RDAT) type, TESRegionData virtual 08 (purecall in the base class)
			enum class TESRegionDataType : std::uint32_t
			{
				kObjects = 2,		// TESRegionDataObjects
				kWeather = 3,		// TESRegionDataWeather
				kMap = 4,			// TESRegionDataMap
				kLandscape = 5,		// TESRegionDataLandscape
				kGrass = 6,			// TESRegionDataGrass
				kSound = 7			// TESRegionDataSound
			};

			// 1.11.246.0: size 0x10, vtable RVA 0x3014A90, 10 virtual functions. Creates, casts and loads the RDAT data of a region.
			class TESRegionDataManager
			{
			public:
				virtual ~TESRegionDataManager() = default;

				// Returns the region being loaded (0x8BE40, a getter shared by several classes).
				[[nodiscard]] virtual Forms::TESRegion* GetCurrentRegion() const noexcept(true) { return currentRegion; } // 01
				// Allocates the data object of the given type: the base manager creates the game classes, the editor one
				// the *Editor classes (TESRegionDataObjectsEditor 0x20 bytes, Weather 0x70, Map 0x20, Landscape 0x18,
				// Grass 0x20, Sound 0x30). RVA 0x9602F0 (base), 0x104A330 (editor).
				[[nodiscard]] virtual TESRegionData* CreateRegionData(std::uint32_t Type) noexcept(true) { return nullptr; } // 02
				// Return Data when its type (TESRegionData virtual 08) matches, else nullptr.
				[[nodiscard]] virtual TESRegionData* AsObjects(TESRegionData* Data) noexcept(true) { return nullptr; } // 03 - RVA 0x9604D0
				[[nodiscard]] virtual TESRegionData* AsWeather(TESRegionData* Data) noexcept(true) { return nullptr; } // 04 - RVA 0x960510
				[[nodiscard]] virtual TESRegionData* AsMap(TESRegionData* Data) noexcept(true) { return nullptr; } // 05 - RVA 0x960550
				[[nodiscard]] virtual TESRegionData* AsLandscape(TESRegionData* Data) noexcept(true) { return nullptr; } // 06 - RVA 0x960590
				[[nodiscard]] virtual TESRegionData* AsGrass(TESRegionData* Data) noexcept(true) { return nullptr; } // 07 - RVA 0x9605D0
				[[nodiscard]] virtual TESRegionData* AsSound(TESRegionData* Data) noexcept(true) { return nullptr; } // 08 - RVA 0x960610
				// Reads one subrecord of the region's data (RDAT, RDOT, RDWT, RDMP, RDGS, RDSA...) from the file into the
				// region; returns false when File or Region is null. RVA 0x960650.
				virtual bool LoadRegionSubrecord(TESFile* File, Forms::TESRegion* Region) noexcept(true) { return false; } // 09

			public:
				Forms::TESRegion* currentRegion; // 08 - set by LoadRegionSubrecord
			};
			static_assert(sizeof(TESRegionDataManager) == 0x10);

			// 1.11.246.0: size 0x10, vtable RVA 0x3014B00. Overrides CreateRegionData with the *Editor data classes.
			class TESRegionDataManagerEditor :
				public TESRegionDataManager   // 000
			{
			public:
				virtual ~TESRegionDataManagerEditor() = default;
			};
			static_assert(sizeof(TESRegionDataManagerEditor) == 0x10);
		}
	}
}
