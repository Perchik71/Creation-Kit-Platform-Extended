// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/BSTriShape.h

#pragma once

#include "BSGeometry.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			class BSTriShape : public BSGeometry
			{
			public:
				virtual ~BSTriShape() = default;

				uint16_t m_TriangleCount;
				uint16_t m_VertexCount;
			};
			static_assert(sizeof(BSTriShape) == 0x160);
		}
	}
}
