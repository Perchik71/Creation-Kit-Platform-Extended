// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESRegionDataManager.h>
#include <EditorAPI/B/BGSLocalizedString.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/T/TESTexture.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class TESRegionObjectList;
			class TESRegionGrassObjectList;

			// 1.11.246.0: size 0x10, vtable RVA 0x302F0C8. One RDAT block of a region (objects, weather, map, landscape,
			// grass, sound); TESRegion::dataList holds them. The game classes below are what TESRegionDataManager::CreateRegionData builds
			// in the game; the editor builds the *Editor subclasses, which are bigger and add virtual functions.
			class TESRegionData
			{
			public:
				virtual ~TESRegionData() = default; // 00

				// Writes the RDAT chunk: type (4 bytes), flags and priority
				virtual void SaveHeader() const = 0; // 01
				// Reads flags and priority from an RDAT chunk (the priority is capped at 100); false when there is no chunk
				virtual bool LoadHeader(const void* RDAT) = 0; // 02

			protected:
				virtual void sub_018() = 0; // 03 - no-op
				virtual void sub_020() = 0; // 04 - no-op
				virtual void sub_028() = 0; // 05 - returns true
				virtual void sub_030() = 0; // 06 - no-op
				virtual void sub_038() = 0; // 07 - no-op

			public:
				[[nodiscard]] virtual TESRegionDataType GetType() const = 0; // 08
				// Allocates a new data object of the same type and copies this one into it
				[[nodiscard]] virtual TESRegionData* Clone() const = 0; // 09
				// Clones this object, then merges Other into the clone with mode 2
				[[nodiscard]] virtual TESRegionData* CloneMerged(const TESRegionData* Other) const = 0; // 0A
				// Merges Other (same type) into this object
				virtual void Merge(const TESRegionData* Other, std::int32_t Mode) = 0; // 0B

				std::uint8_t flags; // 08 - RDAT flags (override)
				std::uint8_t unk09; // 09
				std::uint8_t priority; // 0A - 0-100
				std::uint8_t pad0B[0x5]; // 0B
			};
			static_assert(offsetof(TESRegionData, priority) == 0xA);
			static_assert(sizeof(TESRegionData) == 0x10);

			// 1.11.246.0: size 0x20 (type 4, RDMP)
			class TESRegionDataMap :
				public TESRegionData   // 00
			{
			public:
				BGSLocalizedString mapName; // 10
			};
			static_assert(sizeof(TESRegionDataMap) == 0x20);

			// 1.11.246.0: size 0x18 (type 5, RDMD / landscape texture)
			class TESRegionDataLandscape :
				public TESRegionData   // 00
			{
			public:
				TESTexture* landscapeTexture; // 10 - 40 byte heap TESTexture, copied by Clone
			};
			static_assert(sizeof(TESRegionDataLandscape) == 0x18);

			// 1.11.246.0: size 0x18 (type 6, grass)
			class TESRegionDataGrass :
				public TESRegionData   // 00
			{
			public:
				TESRegionGrassObjectList* grassList; // 10 - copied by Clone
			};
			static_assert(sizeof(TESRegionDataGrass) == 0x18);

			// 1.11.246.0: size 0x20 (type 2, objects)
			class TESRegionDataObjects :
				public TESRegionData   // 00
			{
			public:
				TESRegionObjectList* objectList; // 10 - copied by Clone
				std::uint8_t unk18; // 18
				std::uint8_t pad19[0x7]; // 19
			};
			static_assert(sizeof(TESRegionDataObjects) == 0x20);

			// 1.11.246.0: size 0x30 (type 7, sound)
			class TESRegionDataSound :
				public TESRegionData   // 00
			{
			public:
				void* unk10; // 10 - pointer copied by Clone
				BSTArray<void*> sounds; // 18 - element type not identified yet
			};
			static_assert(sizeof(TESRegionDataSound) == 0x30);

			// 1.11.246.0: size 0x70 (type 3, weather)
			class TESRegionDataWeather :
				public TESRegionData   // 00
			{
			public:
				std::uint8_t unk10[0x30]; // 10 - sub-object, layout not resolved
				std::uint8_t unk40[0x30]; // 40 - sub-object, layout not resolved
			};
			static_assert(sizeof(TESRegionDataWeather) == 0x70);
		}
	}
}
