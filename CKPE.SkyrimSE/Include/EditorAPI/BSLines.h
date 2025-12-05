// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BSGeometry.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			// 160
			// Same BSTriShape.
			class BSLines : public BSGeometry
			{
				std::uint16_t Triangles;	// 158
				std::uint16_t Vertices;		// 15A
			public:
				[[nodiscard]] inline std::uint16_t GetTriangles() const noexcept(true) { return Triangles; }
				[[nodiscard]] inline std::uint16_t GetVertices() const noexcept(true) { return Vertices; }
			};
			static_assert(sizeof(BSLines) == 0x160);
		}
	}
}