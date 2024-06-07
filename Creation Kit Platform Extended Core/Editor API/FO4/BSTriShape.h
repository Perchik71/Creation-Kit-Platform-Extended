// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "BSGeometry.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			// 170
			class BSTriShape : public BSGeometry
			{
			public:
				inline uint32_t GetTriangles() const { return Triangles; }
				inline uint32_t GetVertices() const { return Vertices; }
			private:
				uint32_t Triangles;	// 160
				uint32_t Vertices;	// 164
				char Pad168[8];		// 168
			};
			static_assert(sizeof(BSTriShape) == 0x170);
		}
	}
}