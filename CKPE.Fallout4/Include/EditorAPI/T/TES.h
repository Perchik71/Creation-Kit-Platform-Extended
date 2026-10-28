// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Singleton.h>
#include <EditorAPI/B/BSSettings.h>
#include <EditorAPI/N/NiNode.h>
#include <EditorAPI/N/NiFlags.h>
#include <EditorAPI/T/TESObjectCELL.h>
#include <EditorAPI/B/BSTList.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/S/Sky.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// 1.11.246.0: size 0x138, vtable RVA 0x30120A8, the global pointer TES_Singleton is at 0x143D49150.
			// From 0xA0 on the layout and names follow CommonLibF4's TES (offsets, constructor defaults and uses checked in the CK).
			class TES : public NiAPI::NiRefObject
			{
			public:
				class GridCellArray* gridCellArray;							// 10
				NiAPI::NiNode* unk_node0;									// 18 - constructor argument; its vtable +0x1E0 receives the BSTempNodeManager
				NiAPI::NiNode* unk_node1;									// 20 - constructor argument
				void* unk28;												// 28 - constructor argument
				class BSTempNodeManager* tempMgr;							// 30
				class NiDirectionalLight* directionalLight;					// 38 - taken from the sky
				class BSFogProperty* fogProperty;							// 40
				Forms::TESObjectCELL::CellCoordinates gridCell[2];			// 48 - current grid (CommonLibF4: currentGrid), 50 - queued grid
				Forms::TESObjectCELL* interiorCell;							// 58 - unload when it loads exterior
				Forms::TESObjectCELL** exteriorCellBuffer;					// 60 - uGridsToLoad^2 loaded exterior cells (scanned for a cell by sub_140849AD0)
				Forms::TESObjectCELL** loadedCells;							// 68 - uGridsToLoad * (uGridsToLoad + 4) + 2 entries
				std::uint32_t tempInteriorBufferSize;						// 70
				std::uint32_t tempExteriorBufferSize;						// 74
				Forms::TESObjectCELL::CellCoordinates earlyGridCell;		// 78 - 0x7FFFFFFF sets if exterior loaded or empty (CommonLibF4: saveGrid)
				void* tacCallbackFunc;										// 80 - cell traversal (TAC) callback
				void* tacCallbackData;										// 88
				void* tacRegionFilter;										// 90 - TESRegion*
				Sky* sky;													// 98
				BSSimpleList<void*> activeImageSpaceModifiers;				// A0 - used by TESImageSpaceModifier to add/remove its instances
				std::uint32_t totalToLoad;									// B0
				std::uint32_t loaded;										// B4
				std::uint64_t cellCaptureStartTime;							// B8
				bool disablePercentageUpdate;								// C0
				bool showLANDborders;										// C1
				bool fadeWhenLoading;										// C2 - true on construction
				bool forceUpdateMultiBoundExterior;							// C3
				bool collisionBoxes;										// C4
				bool runningCellTests;										// C5
				bool runningCellTests2;										// C6
				bool runningGraphicsTests;									// C7
				bool runningCellListGraphicsTests;							// C8
				bool runningSubmergedObjectTest;							// C9
				bool allowUnusedPurge;										// CA - true on construction
				char padCB[5];												// CB
				float cellDeltaX;											// D0
				float cellDeltaY;											// D4
				Forms::TESForm* worldSpace;									// D8
				BSTArray<void*> deadCount;									// E0 - BSTArray<BSTTuple<TESActorBase*, std::uint16_t>> (16 byte elements)
				void* preloadedAddonNodes;									// F8 - NiPointer<QueuedFile>
				void* bloodDecalPreload1;									// 100 - NiPointer<NiTexture>
				void* preloadedForms;										// 108 - NiPointer<QueuedFile>
				void* preloadedDefaultModels;								// 110 - NiPointer<QueuedFile>
				void* particleCacheHead;									// 118 - ParticleObjectCache*
				std::uint32_t placeableWaterCount;							// 120
				std::uint32_t pad124;										// 124
				void* navMeshInfoMap;										// 128 - NavMeshInfoMap*
				void* loadedAreaBound;										// 130 - NiPointer<LoadedAreaBound>
			public:
				virtual ~TES();

				constexpr static Common::ID VTABLE = IDs::VTABLE::TES[0];

				inline static ISingleton<Setting> Setting_ShowMarkers;

				[[nodiscard]] inline bool Empty() const noexcept(true)
				{
					return	(gridCell[0].X == 0x7FFFFFFF) &&
							(gridCell[0].Y == 0x7FFFFFFF) &&
							(gridCell[1].X == 0x7FFFFFFF) &&
							(gridCell[1].Y == 0x7FFFFFFF);
				}

				[[nodiscard]] inline bool HasLoadedInterior() const noexcept(true) { return interiorCell != nullptr; }
				[[nodiscard]] inline bool HasLoadedExterior() const noexcept(true) { return interiorCell == nullptr; }
				[[nodiscard]] inline Forms::TESObjectCELL** GetLoadedCells() const noexcept(true) { return loadedCells; }
				[[nodiscard]] inline Forms::TESObjectCELL** GetExteriorCellBuffer() const noexcept(true) { return exteriorCellBuffer; }
				[[nodiscard]] inline Sky* GetSky() const noexcept(true) { return sky; }

				// The loaded interior cell; nullptr for exteriors (the exterior cells are in GetExteriorCellBuffer()).
				[[nodiscard]] inline Forms::TESObjectCELL* GetCurrentRendererCell() const noexcept(true)
				{ 
					if (Empty()) return nullptr;
					return interiorCell;
				}

				Forms::TESObjectCELL::CellCoordinates GetCurrentRendererCellCoordinates() const noexcept(true) { return gridCell[0]; }

				inline static ISingleton<TES> Singleton;
			};
			static_assert(sizeof(TES) == 0x138);
			static_assert(offsetof(TES, gridCell) == 0x48);
			static_assert(offsetof(TES, interiorCell) == 0x58);
			static_assert(offsetof(TES, earlyGridCell) == 0x78);
			static_assert(offsetof(TES, sky) == 0x98);
			static_assert(offsetof(TES, activeImageSpaceModifiers) == 0xA0);
			static_assert(offsetof(TES, worldSpace) == 0xD8);
			static_assert(offsetof(TES, deadCount) == 0xE0);
			static_assert(offsetof(TES, navMeshInfoMap) == 0x128);
		}
	}
}