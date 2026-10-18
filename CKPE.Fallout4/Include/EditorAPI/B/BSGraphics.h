// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <d3d11.h>
#include <EditorAPI/B/BSGraphicsTypes.h>
#include <CKPE.Common.Relocation.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiCamera;
			}

			//https://github.com/hexabits/nifskope/blob/develop/src/data/niftypes.h

			namespace BSGraphics
			{
				struct Vertex
				{
					enum Attribute : std::uint8_t
					{
						VA_POSITION = 0x0,
						VA_TEXCOORD0,
						VA_TEXCOORD1,
						VA_NORMAL,
						VA_BINORMAL,
						VA_COLOR,
						VA_SKINNING,
						VA_LANDDATA,
						VA_EYEDATA,
						VA_COUNT
					};

					enum Flags : std::uint16_t
					{
						VF_VERTEX = 1 << VA_POSITION,
						VF_UV = 1 << VA_TEXCOORD0,
						VF_UV_2 = 1 << VA_TEXCOORD1,
						VF_NORMAL = 1 << VA_NORMAL,
						VF_TANGENT = 1 << VA_BINORMAL,
						VF_COLORS = 1 << VA_COLOR,
						VF_SKINNED = 1 << VA_SKINNING,
						VF_LANDDATA = 1 << VA_LANDDATA,
						VF_EYEDATA = 1 << VA_EYEDATA,
						VF_FULLPREC = 0x400
					};

					enum Masks : std::uint64_t
					{
						DESC_MASK_VERT = 0xFFFFFFFFFFFFFFF0,
						DESC_MASK_UVS = 0xFFFFFFFFFFFFFF0F,
						DESC_MASK_NBT = 0xFFFFFFFFFFFFF0FF,
						DESC_MASK_SKCOL = 0xFFFFFFFFFFFF0FFF,
						DESC_MASK_DATA = 0xFFFFFFFFFFF0FFFF,
						DESC_MASK_OFFSET = 0xFFFFFF0000000000,
						DESC_MASK_FLAGS = ~(DESC_MASK_OFFSET)
					};
				};

				class VertexDesc
				{
					std::uint64_t desc;
				public:
					[[nodiscard]] inline bool HasFlag(Vertex::Flags flag) const noexcept(true)
					{
						return ((desc >> 44) & flag) != 0;
					}

					inline void SetFlag(Vertex::Flags flag) noexcept(true)
					{
						desc |= (static_cast<std::uint64_t>(flag) << 44);
					}

					inline void ClearFlag(Vertex::Flags flag) noexcept(true)
					{
						desc &= ~(static_cast<std::uint64_t>(flag) << 44);
					}

					[[nodiscard]] inline std::uint32_t GetAttributeOffset(Vertex::Attribute attribute) const noexcept(true)
					{
						return (desc >> (4 * static_cast<std::uint8_t>(attribute) + 2)) & 0x3C;
					}

					void SetAttributeOffset(Vertex::Attribute attribute, std::uint32_t offset) noexcept(true)
					{
						if (attribute != Vertex::Attribute::VA_POSITION)
						{
							const std::uint64_t lhs = static_cast<std::uint64_t>(offset) << (4 * static_cast<std::uint8_t>(attribute) + 2);
							const std::uint64_t rhs = desc & ~static_cast<std::uint64_t>(15 << (4 * static_cast<std::uint8_t>(attribute) + 4));

							desc = lhs | rhs;
						}
					}

					inline void ClearAttributeOffsets() noexcept(true)
					{
						desc &= Vertex::Masks::DESC_MASK_OFFSET;
					}

					[[nodiscard]] Vertex::Flags GetFlags() const noexcept(true)
					{
						return static_cast<Vertex::Flags>((desc & Vertex::Masks::DESC_MASK_OFFSET) >> 44);
					}
					void SetFlags(Vertex::Flags flags) noexcept(true)
					{
						desc |= (static_cast<std::uint64_t>(flags) << 44) | (desc & Vertex::Masks::DESC_MASK_FLAGS);
					}

					[[nodiscard]] std::uint32_t GetSize() const noexcept(true)
					{
						std::uint32_t vertexSize = 0;
						auto          flags = GetFlags();

						if (flags & Vertex::VF_VERTEX)
							vertexSize += sizeof(float) << 2;

						if (flags & Vertex::VF_UV)
							vertexSize += sizeof(std::uint16_t) << 1;

						if (flags & Vertex::VF_UV_2)
							vertexSize += sizeof(std::uint16_t) << 1;

						if (flags & Vertex::VF_NORMAL)
						{
							vertexSize += sizeof(std::uint16_t) << 1;
							if (flags & Vertex::VF_TANGENT)
								vertexSize += sizeof(std::uint16_t) << 1;
						}

						if (flags & Vertex::VF_COLORS)
							vertexSize += sizeof(std::uint8_t) << 2;

						if (flags & Vertex::VF_SKINNED)
							vertexSize += (sizeof(std::uint16_t) << 2) + (sizeof(std::uint8_t) << 2);

						if (flags & Vertex::VF_EYEDATA)
							vertexSize += sizeof(float);

						return vertexSize;
					}
				};
				static_assert(sizeof(VertexDesc) == 0x8);

				//

				struct TriShape
				{
					struct IndexData
					{
						ID3D11Buffer* Buffer;
						std::uint16_t* RawIndexData;
					};

					struct VertexData
					{
						ID3D11Buffer* Buffer;
						std::uint8_t* RawVertexData;
					};

					BSGraphics::VertexDesc VertexDesc;	// 00
					VertexData* Vertexes;				// 08
					IndexData* Indexes;					// 10
					volatile std::uint32_t RefCount;	// 18
					std::uint32_t pad1C;				// 1C
				};
				static_assert(sizeof(TriShape::IndexData) == 0x10);
				static_assert(sizeof(TriShape::VertexData) == 0x10);
				static_assert(sizeof(TriShape) == 0x20);

				// bsgraphicsrendertargetmanager.cpp
				class RenderTargetManager
				{
				public:
					// Saves an acquired render target to a file (asserts "Render target not acquired to save it" when the target slot
					// is -1). FaceGen uses it for the head diffuse / normal / specular textures. Target indices are < 100 (asserted).
					// CK 1.11.246.0 0x142B72DB0.
					inline void SaveRenderTarget(std::uint32_t Target, const char* FileName, std::int32_t Format, bool Flag) noexcept(true)
					{
						using func_t = decltype(&RenderTargetManager::SaveRenderTarget);
						static CKPE::Common::Relocation<func_t> func{ CKPE::Common::ID{ 75665, 1589229 } };
						return func(this, Target, FileName, Format, Flag);
					}
				};

				class State
				{
				public:
					// Loads the camera matrices into the per-thread renderer state. The camera must not be null (it is dereferenced).
					// The call in the -MapMaker path of CK 1.11.246.0 0x140951020 + 0x92C passes the two float arguments.
					// CK 1.11.246.0 0x142B87920.
					inline void SetCameraData(NiAPI::NiCamera* Camera, bool Flag, float Arg4, float Arg5) noexcept(true)
					{
						using func_t = decltype(&State::SetCameraData);
						static CKPE::Common::Relocation<func_t> func{ CKPE::Common::ID{ 795781, 1648232 } };
						return func(this, Camera, Flag, Arg4, Arg5);
					}
				};
			}
		}
	}
}