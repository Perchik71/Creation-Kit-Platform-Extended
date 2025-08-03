// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/BSTriShape.h

#pragma once

#include "BSGeometry.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			class BSTriShape : public BSGeometry
			{
			public:
				virtual ~BSTriShape() = default;

				std::uint16_t m_TriangleCount;
				std::uint16_t m_VertexCount;
			};
			static_assert(sizeof(BSTriShape) == 0x160);
		}
	}
}
