// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "TESForm.h"
#include "NiAPI/NiNode.h"

#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			class TESObjectCELL;

			// size 0x50
			// func 101
			class TESObjectLAND : public TESForm
			{
			public:
				constexpr static uint8_t TYPE_ID = ftLandspace;		
				constexpr static uint32_t PART_COUNT = 4;
				constexpr static uint32_t MAX_TEXTURE_COUNT = 6;

				// In the titles, I will stick to the terminology of SSEEdit
				// After studying the dump, I concluded that there are only 289 (0x121) elements
				constexpr static uint32_t TOTAL_INDEX = 289;
				constexpr static uint32_t TOTAL_INDEX_ALL_PARTS = TOTAL_INDEX * PART_COUNT;
				
				struct VertexNormalsT
				{
					uint8_t x, y, z;
				};

				struct AlphaIndex
				{
					// In fact, only 2 bytes are used, but in algorithms it is used as 4 bytes. 
					// This led to an error in ver. 1.6. Incorrect typing and ignoring compiler warnings. 
					// In some places, 16-bit registers are used, and where 32-bit ones are used. 
					// I consider 32 bits to be the standard, since it is in this form that they are stored in memory.
					union 
					{
						//struct { uint16_t Pad, _16Index; };
						//uint32_t Index;

						// It turned out to be real numbers 0-288
						float Index;
					};
				};

				struct AlphaData
				{
					uint8_t MixValues[MAX_TEXTURE_COUNT];
				};

				// size 0x4900
				struct Layers
				{
					struct TextureList
					{
						TESForm* Items[MAX_TEXTURE_COUNT];
					};

					// Parts with tex ??? (if the location is not loaded, these are all nullptr)
					NiAPI::NiNode* Parts[PART_COUNT];
					// size 0x1210 (bytes)
					AlphaIndex AlphaIndexes[TOTAL_INDEX_ALL_PARTS];
					// size 0x1B18 (bytes)
					AlphaData AlphaData[TOTAL_INDEX_ALL_PARTS];
					// size 0xD8C (bytes)
					NiAPI::NiRGB VertexColors[TOTAL_INDEX_ALL_PARTS];
					// size 0xD8C (bytes)
					VertexNormalsT VertexNormals[TOTAL_INDEX_ALL_PARTS];
					char pad4860[0x30];
					TESForm* LandTextureDefaultPart1;
					TESForm* LandTextureDefaultPart2;
					TESForm* LandTextureDefaultPart3;
					TESForm* LandTextureDefaultPart4;
					// A strange pointer, where there are 4 arrays packed to capacity with "1".
					char pad48B0[0x8];
					TextureList* _LandTextureListPart1;
					TextureList* _LandTextureListPart2;
					TextureList* _LandTextureListPart3;
					TextureList* _LandTextureListPart4;
					class hkpMoppCode* _MoppCode;
					char pad48E0[0x20];

					// Normalizing normals
					inline static void HKNormalize(NiAPI::NiPoint3* Normals, int32_t Size, int32_t Offset)
					{
						// In versions CK 1.6 and newer, normalization is incorrect
						// Offset == 0xC == sizeof(NiAPI::NiPoint3)

						for (int32_t i = 0; i < Size; i++)
							Normals[i].FastNormalize();
					}
				};
			public:
				virtual ~TESObjectLAND() = default;

				TESObjectCELL* GetParentCell() const { return _ParentCell; }
				TESObjectLAND::Layers* GetLayers() const { return _Layers; }

				// This function is just for exploring the data. (DEBUG)
				inline void DumpLayers(const char* fname) const
				{
					FILE* f = _fsopen(fname, "wb+", _SH_DENYWR);
					if (f)
					{
						Utils::ScopeFileStream Stream(f);
						fwrite(_Layers, 1, voltek::scalable_msize(_Layers), f);
					}
				}
			private:
				char pad28[0x10];
				TESObjectCELL* _ParentCell;
				char pad40[0x8];
				Layers* _Layers;
			};
			static_assert(sizeof(TESObjectLAND) == 0x50);
			static_assert(sizeof(TESObjectLAND::Layers) == 0x4900);
		}
	}
}

#pragma pack(pop)