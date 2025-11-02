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
			// Same BSTriShape but for SSE. For FO4 BSTriShape more num **std::uint32_t**.
			class BSLines : public BSGeometry
			{
			public:
				[[nodiscard]] inline std::uint16_t GetTriangles() const noexcept(true) { return Triangles; }
				[[nodiscard]] inline std::uint16_t GetVertices() const noexcept(true) { return Vertices; }
			private:
				std::uint16_t Triangles;	// 160
				std::uint16_t Vertices;		// 162
				char Pad168[12];			// 164
			};
			static_assert(sizeof(BSLines) == 0x170);
		}
	}
}