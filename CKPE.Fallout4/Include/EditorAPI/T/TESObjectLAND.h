// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.EnumSet.h>
#include <CKPE.Stream.h>
#include <EditorAPI/N/NiNode.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/N/NiPointer.h>
#include <EditorAPI/B/BSTriShape.h>
#include <EditorAPI/T/TESLandTexture.h>
#include <EditorAPI/T/TESChildCell.h>
#include <array>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// size 0x50
				class TESObjectLAND : public TESForm, public TESChildCell
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

					struct OBJ_LAND
					{
					public:
						enum class Flag
						{
							kNone = 0,
							kVertexNormals_HeightMap = 1 << 0,
							kVertexColors = 1 << 1,
							kLayers = 1 << 2,
							kMPCD = 1 << 10
						};

						// members
						CKPE::TEnumSet<Flag, std::uint32_t> flags;  // 0
					};
					static_assert(sizeof(OBJ_LAND) == 0x4);

					struct CHAR_NORM
					{
						std::int8_t x, y, z;
					};

					// It real numbers [-288 : 288]
					// I don't know what it means if the index is negative, but I won't study it further.
					// Also, since there is no fractional part, the highest 2 bytes are used.
					// You can check it on the website https://www.h-schmidt.net/FloatConverter/IEEE754.html
					typedef float Height;

					struct PercentData
					{
						std::array<std::uint8_t, MAX_TEXTURE_COUNT> MixValues;
					};

					// size 0x6420
					struct Layers
					{
						struct TextureList
						{
							std::array<TESLandTexture*, MAX_TEXTURE_COUNT> Items;
						};

						std::array<NiAPI::NiNode*, PART_COUNT> Mesh;
						std::array<Height, TOTAL_INDEX_ALL_PARTS> Heights;			// size 0x1210 (bytes)
						std::array<PercentData, TOTAL_INDEX_ALL_PARTS> Percents;	// size 0x3630 (bytes)
						std::array<NiAPI::NiRGB, TOTAL_INDEX_ALL_PARTS> Colors;		// size 0xD8C (bytes)
						std::array<CHAR_NORM, TOTAL_INDEX_ALL_PARTS> VertexNormals;	// size 0xD8C (bytes)
						char pad6378[0x18];
						// what ???
						std::array<NiAPI::NiPointer<BSTriShape>, PART_COUNT> Geom;
						NiAPI::NiPointer<BSTriShape> border;
						NiAPI::NiPoint2 heightExtents;
						std::array<TESLandTexture*, PART_COUNT> LandTextureDefaultPart;
						// A strange pointer, where there are 4 arrays packed to capacity with "1".
						char pad63E0[0x8];
						std::array<TextureList*, PART_COUNT> LandTextureListPart;
						char pad6408[0x8];
						float heightOffset;
						char pad6414[0xC];

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
					OBJ_LAND		data;
					TESObjectCELL* _ParentCell;
					char pad40[0x8];
					Layers* _Layers;				// Can be nullptr if cell not loaded
				};
				static_assert(sizeof(TESObjectLAND) == 0x50);
				static_assert(sizeof(TESObjectLAND::CHAR_NORM) == 0x3);
				static_assert(sizeof(TESObjectLAND::PercentData) == 0xC);
				static_assert(sizeof(TESObjectLAND::Layers) == 0x6420);
				static_assert(sizeof(TESObjectLAND::Layers::TextureList) == 0x60);
				static_assert(offsetof(TESObjectLAND::Layers, Heights) == 0x20);
				static_assert(offsetof(TESObjectLAND::Layers, Percents) == 0x1230);
				static_assert(offsetof(TESObjectLAND::Layers, VertexNormals) == 0x55EC);
				static_assert(offsetof(TESObjectLAND::Layers, heightOffset) == 0x6410);
			}
		}
	}
}