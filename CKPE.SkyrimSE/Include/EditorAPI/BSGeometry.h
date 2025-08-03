// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiCullingProcess.h

#pragma once

#include <CKPE.Asserts.h>

#include "NiAPI\NiPointer.h"
#include "NiAPI\NiSkinInstance.h"
#include "NiAPI\NiAVObject.h"
#include "NiAPI\NiProperty.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			using namespace NiAPI;

			class BSShaderProperty;

			enum
			{
				GEOMETRY_TYPE_GEOMETRY = 0x0,
				GEOMETRY_TYPE_PARTICLES = 0x1,
				GEOMETRY_TYPE_STRIP_PARTICLES = 0x2,
				GEOMETRY_TYPE_TRISHAPE = 0x3,
				GEOMETRY_TYPE_DYNAMIC_TRISHAPE = 0x4,
				GEOMETRY_TYPE_MESHLOD_TRISHAPE = 0x5,
				GEOMETRY_TYPE_LOD_MULTIINDEX_TRISHAPE = 0x6,
				GEOMETRY_TYPE_MULTIINDEX_TRISHAPE = 0x7,
				GEOMETRY_TYPE_SUBINDEX_TRISHAPE = 0x8,
				GEOMETRY_TYPE_SUBINDEX_LAND_TRISHAPE = 0x9,
				GEOMETRY_TYPE_MULTISTREAMINSTANCE_TRISHAPE = 0xA,
				GEOMETRY_TYPE_PARTICLE_SHADER_DYNAMIC_TRISHAPE = 0xB,
				GEOMETRY_TYPE_LINES = 0xC,
				GEOMETRY_TYPE_DYNAMIC_LINES = 0xD,
				GEOMETRY_TYPE_INSTANCE_GROUP = 0xE,
			};

			class BSGeometry : public NiAVObject
			{
			public:
				virtual ~BSGeometry();
				virtual class BSMultiIndexTriShape* IsMultiIndexTriShape();
				virtual class BSSkinnedDecalTriShape* IsSkinnedDecalTriShape();
				virtual void* IsUnknown();

				char _pad[0x10];
				NiPointer<NiProperty> spProperties[2];
				NiPointer<NiSkinInstance> spSkinInstance;
				void* pRendererData;
				char _pad2[0x8];
				std::uint64_t uiVertexDesc;
				char ucType;
				// bool Registered?

				inline NiProperty* QProperty(std::uint32_t Index) const
				{
					CKPE_ASSERT(Index < ARRAYSIZE(spProperties));

					return spProperties[Index];
				}

				inline NiAlphaProperty* QAlphaProperty() const { return static_cast<NiAlphaProperty*>(QProperty(0)); }
				inline NiShadeProperty* QShadeProperty() const { return reinterpret_cast<NiShadeProperty*>(QProperty(1)); }
				inline BSShaderProperty* QShaderProperty() const { return reinterpret_cast<BSShaderProperty*>(QProperty(1)); }

				NiSkinInstance* QSkinInstance() const
				{
					return spSkinInstance;
				}

				inline void* QRendererData() const { return pRendererData; }
				inline std::uint64_t GetVertexDesc() const { return uiVertexDesc; }
				inline std::uint32_t GetVertexAttributeOffset(std::uint32_t Index) const { return CalculateVertexAttributeOffset(uiVertexDesc, Index); }
				inline std::uint32_t GetVertexAttributeStream(std::uint32_t Index) const { return CalculateVertexAttributeStream(uiVertexDesc, Index); }
				inline std::uint32_t GetVertexSize() const { return CalculateVertexSize(uiVertexDesc); }
				inline std::uint32_t GetDynamicVertexSize() const { return CalculateDyanmicVertexSize(uiVertexDesc); }
				inline int QType() const { return ucType; }

				//
				// enum BSGraphics::VertexDescEntry
				// https://forum.niftools.org/topic/4514-bsvertexdata-bsvertexformat/
				// https://github.com/ousnius/BodySlide-and-Outfit-Studio/blob/4537c4fc66a79d8a54e72cc12a732dc00008d356/lib/NIF/VertexData.h#L45
				//
				static bool HasVertexAttribute(uint64_t VertexDesc, std::uint32_t AttributeIndex)
				{
					return (VertexDesc & (0x40100000000000i64 << AttributeIndex)) != 0;
				}

				static std::uint32_t CalculateVertexAttributeOffset(uint64_t VertexDesc, std::uint32_t Index)
				{
					return (VertexDesc >> (4 * Index + 2)) & 0x3C;
				}

				static std::uint32_t CalculateVertexAttributeStream(uint64_t VertexDesc, std::uint32_t Index)
				{
					if (VertexDesc & ((std::uint64_t)Index << 44))
						return 0;

					if (VertexDesc & ((std::uint64_t)Index << 54))
						return 1;

					return 0xFFFFFFFF;
				}

				static std::uint32_t CalculateVertexSize(std::uint64_t VertexDesc)
				{
					// Vertex buffer slot 0
					return (VertexDesc << 2) & 0x3C;
				}

				static std::uint32_t CalculateDyanmicVertexSize(std::uint64_t VertexDesc)
				{
					// Vertex buffer slot 1 (BSDynamicTriShape)
					return (VertexDesc >> 2) & 0x3C;
				}
			};
			static_assert(sizeof(BSGeometry) == 0x158);
		}
	}
}