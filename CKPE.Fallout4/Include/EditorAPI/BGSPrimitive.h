// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/NiAPI/NiPoint.h>
#include <EditorAPI/NiAPI/NiPointer.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BGSPrimitive
			{
			public:
				enum class PRIMITIVE_TYPE
				{
					kNone,
					kBox,
					kSphere,
					kPlane,
					kLine,
					kEllipsoid
				};

				virtual ~BGSPrimitive() = default;
			};
			static_assert(sizeof(BGSPrimitive) == 0x8);
		}
	}
}
