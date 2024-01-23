// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once
#pragma pack(push, 1)

#include "TESObjectCELL.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			//
			// The main class of the editor
			//
			class TES
			{
			public:
				class SkyM
				{
				private:
					enum FlagType
					{
						ftSkyInInterior = 1,	// maybe without weather
						ftSkyInExterior = 3,	//
					};
				public:
					class CloudsM
					{
					public:
						virtual ~CloudsM() = default;

						inline bool HasEnabled() const { return flag != 0; }
						inline void SetEnabled(bool val) { flag = (uint16_t)val; }

						PROPERTY(HasEnabled, SetEnabled) bool Enabled;
					private:
						char pad08[0x50A];
						uint16_t flag;
					};
				public:
					virtual ~SkyM() = default;

					void Disable();
					void Enable();
					inline bool HasEnabled() const { return (flag == ftSkyInInterior) || (flag == ftSkyInExterior); }
					inline CloudsM* GetClouds() const { return lpClouds; }

					READ_PROPERTY(GetClouds) CloudsM* Clouds;
				private:
					bool DrawCoulds;
					char pad08[0x8F];
					CloudsM* lpClouds;
					char padA0[0x2DC];
					uint32_t flag;
				};
			public:
				virtual ~TES() = default;

				inline bool HasInterior() const { return lpInteriorCell != nullptr; }
				inline SkyM* GetSky() const { return lpSky; }
				inline TESObjectCELL* GetInterior() const { return lpInteriorCell; }
				inline TESForm* GetWorldspace() const { return lpWorldspaceForm; }

				inline bool IsEmpty(VOID) const 
				{
					return (GridCell[0].X == 0x7FFFFFFF) &&
						   (GridCell[0].Y == 0x7FFFFFFF) &&
					       (GridCell[1].X == 0x7FFFFFFF) && 
						   (GridCell[1].Y == 0x7FFFFFFF);
				}

				READ_PROPERTY(GetSky) SkyM* Sky;
				READ_PROPERTY(GetInterior) TESObjectCELL* Interior;
				READ_PROPERTY(GetWorldspace) TESForm* Worldspace;
			private:
				char pad08[0x8];
				class GridCellArray* lpGridCellArray;
				char pad18[0x20];
				class NiDirectionalLight* lpDirectionalLight;
				class BSFogProperty* lpFogProperty;
				struct GridCellTag { int32_t X, Y; } GridCell[2];
				TESObjectCELL* lpInteriorCell;
				char pad60[0x8];
				TESObjectCELL** lpArrayCell;
				char pad70[0x28];
				SkyM* lpSky;
				char padA0[0x38];
				TESForm* lpWorldspaceForm;
				char padE0[0x48];
				TESForm* lpNavMesh;					// 0x3E type NAVI (idk)
				char pad130[0xE0];

				class bhkWorldM {
				public:
					virtual ~bhkWorldM() = default;
				private:
					char pad08[0x60];
				} World;

				// And something else much more
			};
			static_assert(sizeof(TES) == 0x278);
			static_assert(sizeof(TES::SkyM) == 0x380);

			//
			// An unknown structure containing a lot of useful things, we can say the main one
			//
			struct TESUnknown
			{
				TES* Main;

				inline static TESUnknown* Instance;
			};
		}
	}
}

#pragma pack(pop)