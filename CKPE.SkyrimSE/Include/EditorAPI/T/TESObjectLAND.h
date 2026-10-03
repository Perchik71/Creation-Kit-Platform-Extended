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
#include <EditorAPI/T/TESChildCell.h>
#include <EditorAPI/T/TESLandTexture.h>
#include <array>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// size 0x50
				// func 101
				class TESObjectLAND : public TESForm, public TESChildCell
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftLandspace;
					constexpr static std::uint32_t PART_COUNT = 4;
					constexpr static std::uint32_t MAX_TEXTURE_COUNT = 6;

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
						TEnumSet<Flag, std::uint32_t> flags;  // 0
					};
					static_assert(sizeof(OBJ_LAND) == 0x4);

					struct CHAR_NORM
					{
						std::uint8_t x, y, z;
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

					// size 0x4900
					struct LoadedLandData
					{
						struct TextureList
						{
							std::array<TESLandTexture*, MAX_TEXTURE_COUNT> Items;
						};

						std::array<NiAPI::NiNode*, PART_COUNT> Mesh;
						std::array<Height, TOTAL_INDEX_ALL_PARTS> Heights;			// size 0x1210 (bytes)
						std::array<PercentData, TOTAL_INDEX_ALL_PARTS> Percents;	// size 0x1B18 (bytes)
						std::array<NiAPI::NiRGB, TOTAL_INDEX_ALL_PARTS> Colors;		// size 0xD8C (bytes)
						std::array<CHAR_NORM, TOTAL_INDEX_ALL_PARTS> VertexNormals;	// size 0xD8C (bytes)
						std::array<NiAPI::NiPointer<BSTriShape>, PART_COUNT> Geom;
						NiAPI::NiPointer<BSTriShape> border;
						NiPoint2 heightExtents;
						std::array<TESLandTexture*, PART_COUNT> LandTextureDefaultPart;
						// A strange pointer, where there are 4 arrays packed to capacity with "1".
						char pad48B0[0x8];
						std::array<TextureList*, PART_COUNT> LandTextureListPart;
						class hkpMoppCode* _MoppCode;
						char pad48E0[0x20];

						// Normalizing normals
						inline static void HKNormalize(NiAPI::NiPoint3* Normals, std::int32_t Size, 
							std::int32_t Offset) noexcept(true)
						{
							// In versions CK 1.6 and newer, normalization is incorrect
							// Offset == 0xC == sizeof(NiAPI::NiPoint3)

							for (std::int32_t i = 0; i < Size; i++)
								Normals[i].Normalize();
						}
					};
				public:
					virtual ~TESObjectLAND() = default;

					struct RecordFlags
					{
						enum RecordFlag : std::uint32_t
						{
							kDeleted = 1 << 5,
							kIgnored = 1 << 12,
							kCompressed = 1 << 18
						};
					};

					inline TESObjectCELL* GetParentCell() const noexcept(true) { return _ParentCell; }
					inline TESObjectLAND::LoadedLandData* GetLandData() const noexcept(true) { return _LandData; }
					inline bool HasLoaded() const noexcept(true) { return _LandData != nullptr; }

					// This function is just for exploring the data. (DEBUG)
					inline void DumpLayers(const char* fname) const
					{
						if (!_LandData) return;

						try
						{
							FileStream fstm(fname, FileStream::fmCreate);
							fstm.Write(_LandData, (std::uint32_t)sizeof(TESObjectLAND::LoadedLandData));
						}
						catch (const std::exception& e)
						{
							_ERROR("FileStream: %s", e.what());
						}
					}
				private:
					OBJ_LAND		data;
					std::uint32_t	pad2C;
					TESObjectCELL*	_ParentCell;
					char pad40[0x8];
					LoadedLandData* _LandData;				// Can be nullptr if cell not loaded
				};
				static_assert(sizeof(TESObjectLAND) == 0x50);
				static_assert(sizeof(TESObjectLAND::LoadedLandData) == 0x4900);
			}
		}
	}
}