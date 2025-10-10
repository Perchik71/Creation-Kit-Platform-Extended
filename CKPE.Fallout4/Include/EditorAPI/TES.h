// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Singleton.h>
#include <EditorAPI/BSSettings.h>
#include <EditorAPI/NiAPI/NiNode.h>
#include <EditorAPI/NiAPI/NiFlags.h>
#include <EditorAPI/Forms/TESObjectCELL.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class Clouds
			{
				char pad08[0x50A];
				NiAPI::NiTFlags<std::uint16_t, Clouds> flags;
			public:
				virtual ~Clouds();
			};
			static_assert(sizeof(Clouds) == 0x518);

			// 448
			class Sky : public NiAPI::NiRefObject
			{
				NiAPI::NiNode* unk_node0;					// BSMultiBoundNode
				NiAPI::NiNode* unk_node1;					// NiNode
				char pad20[0x28];
				void* ambientLight;							// NiAmbientLight
				char pad50[0x30];
				void* atmosphere;							// Atmosphere
				void* stars;								// Stars
				void* sun;									// Sun
				Clouds* clouds;								// Clouds
				char padA0[0x2DC];
				NiAPI::NiTFlags<std::uint32_t, Sky> flags;
			public:
				virtual ~Sky();

				inline void SetFog() noexcept(true) { flags.Set(1); }
				inline void SetWeather() noexcept(true) { flags.Set(2); }
				inline void UnsetFog() noexcept(true) { flags.Unset(1); }
				inline void UnsetWeather() noexcept(true) { flags.Unset(2); }
				[[nodiscard]] inline bool HasFog() const noexcept(true) { return flags.Has(1); }
				[[nodiscard]] inline bool HasWeather() const noexcept(true) { return flags.Has(2); }
				[[nodiscard]] inline Clouds* GetClouds() const noexcept(true) { return clouds; }

				inline static ISingleton<Setting> Setting_SkyView;
				inline static ISingleton<Setting> Setting_FogEnabled;
			};
			static_assert(sizeof(Sky) == 0x380);

			// 138
			class TES : public NiAPI::NiRefObject
			{
				class GridCellArray* gridCellArray;
				NiAPI::NiNode* unk_node0;
				NiAPI::NiNode* unk_node1;
				char pad28[8];
				class BSTempNodeManager* tempMgr;
				class NiDirectionalLight* directionalLight;
				class BSFogProperty* fogProperty;
				Forms::TESObjectCELL::CellCoordinates gridCell[2];
				Forms::TESObjectCELL* interiorCell;						// unload when it loads exterior
				Forms::TESObjectCELL* exteriorCell;						// loaded always
				Forms::TESObjectCELL** loadedCells;
				char pad70[8];
				Forms::TESObjectCELL::CellCoordinates earlyGridCell;	// 0x7FFFFFFF sets if exterior loaded or empty
				char pad80[0x18];
				Sky* sky;
				char padA0[0x38];
				Forms::TESForm* worldSpace;
				char padD0[0x40];
				std::uint32_t unk;										// maybe flags ??
				Forms::TESForm* navMesh;
				char pad130[8];
			public:
				virtual ~TES();

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
				[[nodiscard]] inline Sky* GetSky() const noexcept(true) { return sky; }

				[[nodiscard]] inline Forms::TESObjectCELL* GetCurrentRendererCell() const noexcept(true)
				{ 
					if (Empty()) return nullptr;
					return HasLoadedInterior() ? interiorCell : exteriorCell;
				}

				Forms::TESObjectCELL::CellCoordinates GetCurrentRendererCellCoordinates() const noexcept(true) { return gridCell[0]; }

				inline static ISingleton<TES> Singleton;
			};
			static_assert(sizeof(TES) == 0x138);
		}
	}
}