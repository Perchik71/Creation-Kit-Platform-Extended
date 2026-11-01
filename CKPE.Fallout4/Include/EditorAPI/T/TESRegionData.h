// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <cstddef>
#include <Windows.h>
#include <EditorAPI/T/TESRegionDataManager.h>
#include <EditorAPI/T/TESRegionObject.h>
#include <EditorAPI/T/TESWeatherList.h>
#include <EditorAPI/T/TESTexture.h>
#include <EditorAPI/B/BGSLocalizedString.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSMusicType;
			}

			// 1.11.246.0: size 0x10, vtable RVA 0x302F0C8, 13 virtual functions. One RDAT block of a region (objects, weather, map,
			// landscape, grass, sound); TESRegion::dataList holds them. Every type has a game class and an *Editor subclass (the
			// editor builds the *Editor ones, see TESRegionDataManagerEditor::CreateRegionData) that adds the dialog handler.
			class TESRegionData
			{
			public:
				virtual ~TESRegionData() = default; // 00

				// Writes the RDAT chunk (type, flags and priority); the derived classes add their own chunks
				virtual void Save() const; // 01
				// Reads flags and priority from an RDAT chunk (the priority is capped at 100); false when there is no chunk
				virtual bool LoadHeader(const void* RDAT); // 02
				// Resolves the loaded form ids through the file's master list (TESRegionDataSound only)
				virtual void ResolveFormIDs(const void* File); // 03
				// Removes what refers to Form
				virtual void RemoveReferencesTo(Forms::TESForm* Form); // 04
				[[nodiscard]] virtual bool CanCheckIn(const Forms::CheckInFormList* Forms, Forms::TESForm* ReferencingForm) const; // 05 - true
				virtual void BuildCheckInDependencyMessage(const Forms::CheckInFormList* Forms, BSString& Message, Forms::TESForm* ReferencingForm) const; // 06

			protected:
				virtual void sub_038(); // 07 - no-op in every class

			public:
				[[nodiscard]] virtual TESRegionDataType GetType() const = 0; // 08
				// Allocates a new data object of the same type and copies this one into it
				[[nodiscard]] virtual TESRegionData* Clone() const = 0; // 09
				// Clones this object, then merges Other into the clone with mode 2
				[[nodiscard]] virtual TESRegionData* CloneMerged(const TESRegionData* Other) const = 0; // 0A
				// Merges Other (same type) into this object
				virtual void Merge(const TESRegionData* Other, std::int32_t Mode) = 0; // 0B
				// Whether the object holds data that must be saved
				[[nodiscard]] virtual bool HasData() const = 0; // 0C

			public:
				std::uint8_t flags; // 08 - RDAT flags (override)
				std::uint8_t unk09; // 09
				std::uint8_t priority; // 0A - 0-100
				std::uint8_t pad0B[0x5]; // 0B
			};
			static_assert(offsetof(TESRegionData, priority) == 0xA);
			static_assert(sizeof(TESRegionData) == 0x10);

			// 1.11.246.0: size 0x20, vtable RVA 0x302F388, 13 virtual functions
			class TESRegionDataMap :
				public TESRegionData   // 00
			{
			public:
				BGSLocalizedString mapName; // 10
			};
			static_assert(sizeof(TESRegionDataMap) == 0x20);

			// 1.11.246.0: size 0x20, vtable RVA 0x31131F0, one more virtual function
			class TESRegionDataMapEditor :
				public TESRegionDataMap   // 00
			{
			public:
				virtual bool HandleDialogMessage(HWND Dialog, std::uint32_t Message, std::uint64_t WParam, std::int64_t LParam); // 0D
			};
			static_assert(sizeof(TESRegionDataMapEditor) == 0x20);

			// 1.11.246.0: size 0x18, vtable RVA 0x302F228, 16 virtual functions
			class TESRegionDataLandscape :
				public TESRegionData   // 00
			{
			public:
				// Reads the ICON chunk into the texture; false when there is no chunk
				virtual bool LoadChunk(const void* Chunk); // 0D
				// Writes the texture's file name to Name
				virtual void GetTextureName(BSString& Name) const; // 0E
				// Sets the texture's file name, false when it did not change
				virtual bool SetTextureName(const char* const* Name); // 0F

			public:
				TESTexture* landscapeTexture; // 10
			};
			static_assert(sizeof(TESRegionDataLandscape) == 0x18);

			// 1.11.246.0: size 0x18, vtable RVA 0x3113130, one more virtual function
			class TESRegionDataLandscapeEditor :
				public TESRegionDataLandscape   // 00
			{
			public:
				virtual bool HandleDialogMessage(HWND Dialog, std::uint32_t Message, std::uint64_t WParam, std::int64_t LParam); // 10
			};
			static_assert(sizeof(TESRegionDataLandscapeEditor) == 0x18);

			// 1.11.246.0: size 0x18, vtable RVA 0x302F150, 15 virtual functions
			class TESRegionDataGrass :
				public TESRegionData   // 00
			{
			public:
				// Reads an RDGS chunk (grass type and land texture pairs); false when it is not one
				virtual bool LoadChunk(const void* Chunk); // 0D
				[[nodiscard]] virtual TESRegionGrassObjectList* GetGrassList() const; // 0E

			public:
				TESRegionGrassObjectList* grassList; // 10
			};
			static_assert(sizeof(TESRegionDataGrass) == 0x18);

			// 1.11.246.0: size 0x20, vtable RVA 0x3112F00, two more virtual functions
			class TESRegionDataGrassEditor :
				public TESRegionDataGrass   // 00
			{
			public:
				virtual bool HandleDialogMessage(HWND Dialog, std::uint32_t Message, std::uint64_t WParam, std::int64_t LParam); // 0F
				// Fills a tree view (TVM_INSERTITEM) with one item per land texture of the data handler's region data list plus an
				// "Everywhere" root, and places the grass objects of Objects under their texture (item handle of the root kept at 0x18)
				virtual std::int64_t PopulateTextureTree(HWND TreeView, const void* Objects); // 10

			public:
				void* everywhereItem; // 18 - HTREEITEM of the "Everywhere" tree item, set by PopulateTextureTree
			};
			static_assert(sizeof(TESRegionDataGrassEditor) == 0x20);

			// 1.11.246.0: size 0x20, vtable RVA 0x302F410, 18 virtual functions
			class TESRegionDataObjects :
				public TESRegionData   // 00
			{
			public:
				// Reads an RDOB chunk (24 byte records); false when it is not one
				virtual bool LoadObjectsChunk(const void* Chunk); // 0D
				// Reads an RDOT chunk (52 byte records, TESRegionObject); false when it is not one
				virtual bool LoadChunk(const void* Chunk); // 0E
				[[nodiscard]] virtual TESRegionObjectList* GetObjectList() const; // 0F
				[[nodiscard]] virtual std::uint8_t GetUnk18() const; // 10
				virtual void SetUnk18(std::uint8_t Value); // 11

			public:
				TESRegionObjectList* objectList; // 10
				std::uint8_t unk18; // 18
				std::uint8_t pad19[0x7]; // 19
			};
			static_assert(sizeof(TESRegionDataObjects) == 0x20);

			// 1.11.246.0: size 0x20, vtable RVA 0x31133F0, one more virtual function
			class TESRegionDataObjectsEditor :
				public TESRegionDataObjects   // 00
			{
			public:
				virtual bool HandleDialogMessage(HWND Dialog, std::uint32_t Message, std::uint64_t WParam, std::int64_t LParam); // 12
			};
			static_assert(sizeof(TESRegionDataObjectsEditor) == 0x20);

			// 1.11.246.0: size 0x30, vtable RVA 0x302F6D8, 14 virtual functions
			class TESRegionDataSound :
				public TESRegionData   // 00
			{
			public:
				// Reads an RDSA (sound list) or RDMO (music type) chunk
				virtual bool LoadChunk(const void* Chunk); // 0D

			public:
				Forms::BGSMusicType* music; // 10 - resolved by ResolveFormIDs
				// 12 byte entries written to the RDSA chunk as they are in memory (Save writes 12 bytes per entry); the form id is resolved in place by ResolveFormIDs
				struct SoundEntry
				{
					std::uint32_t sound; // 00 - form id
					std::uint32_t flags; // 04
					float chance; // 08
				};
				static_assert(sizeof(SoundEntry) == 0xC);

				BSTArray<SoundEntry> sounds; // 18
			};
			static_assert(sizeof(TESRegionDataSound) == 0x30);

			// 1.11.246.0: size 0x30, vtable RVA 0x3113BE8, one more virtual function
			class TESRegionDataSoundEditor :
				public TESRegionDataSound   // 00
			{
			public:
				virtual bool HandleDialogMessage(HWND Dialog, std::uint32_t Message, std::uint64_t WParam, std::int64_t LParam); // 0E
			};
			static_assert(sizeof(TESRegionDataSoundEditor) == 0x30);

			// 1.11.246.0: size 0x70, vtable RVA 0x302F998, 14 virtual functions
			class TESRegionDataWeather :
				public TESRegionData   // 00
			{
			public:
				// Reads the weather list chunk; Region is only used in the warning logged when entries were dropped
				virtual void LoadChunk(const void* Chunk, Forms::TESForm* Region); // 0D

			public:
				TESWeatherList weatherList; // 10
				TESWeatherList unk40; // 40
			};
			static_assert(sizeof(TESRegionDataWeather) == 0x70);

			// 1.11.246.0: size 0x70, vtable RVA 0x3113CF0, one more virtual function
			class TESRegionDataWeatherEditor :
				public TESRegionDataWeather   // 00
			{
			public:
				virtual bool HandleDialogMessage(HWND Dialog, std::uint32_t Message, std::uint64_t WParam, std::int64_t LParam); // 0E
			};
			static_assert(sizeof(TESRegionDataWeatherEditor) == 0x70);
		}
	}
}
