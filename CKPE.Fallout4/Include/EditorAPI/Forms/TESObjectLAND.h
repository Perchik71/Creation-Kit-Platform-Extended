// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/BSDynamicTriShape.h>
#include <EditorAPI/NiAPI/NiTypes.h>
#include <EditorAPI/Forms/TESLandTexture.h>
#include <EditorAPI/Forms/TESObjectLAND.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESObjectCELL;

				// size 0x50
				class TESObjectLAND : public TESForm
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftLandspace;
					constexpr static std::uint32_t PART_COUNT = 4;
					// Carefully, CK displays supposedly 13 texture slots, but in fact only 12 are provided
					constexpr static std::uint32_t MAX_TEXTURE_COUNT = 12;

					// In the titles, I will stick to the terminology of SSEEdit
					// After studying the dump, I concluded that there are only 289 (0x121) elements
					constexpr static std::uint32_t TOTAL_INDEX = 289;
					constexpr static std::uint32_t TOTAL_INDEX_ALL_PARTS = TOTAL_INDEX * PART_COUNT;

					struct VertexNormalsT
					{
						std::uint8_t x, y, z;
					};

					// It real numbers [-288 : 288]
					// I don't know what it means if the index is negative, but I won't study it further.
					// Also, since there is no fractional part, the highest 2 bytes are used.
					// You can check it on the website https://www.h-schmidt.net/FloatConverter/IEEE754.html
					typedef float AlphaIndex;

					struct AlphaData
					{
						std::uint8_t MixValues[MAX_TEXTURE_COUNT];
					};

					// size 0x6420
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
						// size 0x3630 (bytes)
						AlphaData AlphaData[TOTAL_INDEX_ALL_PARTS];
						// size 0xD8C (bytes)
						NiAPI::NiRGB VertexColors[TOTAL_INDEX_ALL_PARTS];
						// size 0xD8C (bytes)
						VertexNormalsT VertexNormals[TOTAL_INDEX_ALL_PARTS];
						char pad6378[0x18];
						// what ???
						BSDynamicTriShape* TriShapeParts[PART_COUNT];
						char pad63B0[0x10];
						TESLandTexture* LandTextureDefaultPart[PART_COUNT];
						// A strange pointer, where there are 4 arrays packed to capacity with "1".
						char pad63E0[0x8];
						TextureList* _LandTextureListPart[PART_COUNT];
						char pad6408[0x18];

						// Normalizing normals
						inline static void HKNormalize(NiAPI::NiPoint3* Normals, std::int32_t Size, std::int32_t Offset)
						{
							// In versions CK 1.10.980.0+, normalization is incorrect
							// Offset == 0xC == sizeof(NiAPI::NiPoint3)

							for (std::int32_t i = 0; i < Size; i++)
								Normals[i].Normalize();
						}
					};
				private:
					char pad28[0x10];
					TESObjectCELL* _ParentCell;
					char pad40[0x8];
					Layers* _Layers;				// Can be nullptr if cell not loaded
				};
				static_assert(sizeof(TESObjectLAND) == 0x50);
				static_assert(sizeof(TESObjectLAND::VertexNormalsT) == 0x3);
				static_assert(sizeof(TESObjectLAND::AlphaData) == 0xC);
				static_assert(sizeof(TESObjectLAND::Layers) == 0x6420);
			}
		}
	}
}