// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BSGeometry.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// 170
			class BSTriShape : public BSGeometry
			{
			public:
				[[nodiscard]] inline std::uint32_t GetTriangles() const noexcept(true) { return Triangles; }
				[[nodiscard]] inline std::uint32_t GetVertices() const noexcept(true) { return Vertices; }
			private:
				std::uint32_t Triangles;	// 160
				std::uint32_t Vertices;		// 164
				char Pad168[8];				// 168
			};
			static_assert(sizeof(BSTriShape) == 0x170);
		}
	}
}