// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "TESLandTexture.h"
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

				// It real numbers [-288 : 288]
				// I don't know what it means if the index is negative, but I won't study it further.
				// Also, since there is no fractional part, the highest 2 bytes are used.
				// You can check it on the website https://www.h-schmidt.net/FloatConverter/IEEE754.html
				typedef float AlphaIndex;

				struct AlphaData
				{
					uint8_t MixValues[MAX_TEXTURE_COUNT];
				};

				// size 0x4900
				struct Layers
				{
					struct TextureList
					{
						TESLandTexture* Items[MAX_TEXTURE_COUNT];
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
					TESLandTexture* LandTextureDefaultPart1;
					TESLandTexture* LandTextureDefaultPart2;
					TESLandTexture* LandTextureDefaultPart3;
					TESLandTexture* LandTextureDefaultPart4;
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

				inline TESObjectCELL* GetParentCell() const { return _ParentCell; }
				inline TESObjectLAND::Layers* GetLayers() const { return _Layers; }
				inline bool HasLoaded() const { return _Layers != nullptr; }

				// This function is just for exploring the data. (DEBUG)
				inline void DumpLayers(const char* fname) const
				{
					if (!_Layers) return;
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
				Layers* _Layers;				// Can be nullptr if cell not loaded
			};
			static_assert(sizeof(TESObjectLAND) == 0x50);
			static_assert(sizeof(TESObjectLAND::Layers) == 0x4900);
		}
	}
}

#pragma pack(pop)