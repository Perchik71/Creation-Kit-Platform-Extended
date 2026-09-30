// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <CKPE.EnumSet.h>
#include <EditorAPI/NiAPI/NiColor.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			struct DecalData
			{
				enum Flag : std::uint8_t
				{
					kNone = 0,
					kParallax = 1 << 0,
					kAlphaBlending = 1 << 1,
					kAlphaTesting = 1 << 2,
					kNoSubtextures = 1 << 3
				};

				// members
				float decalMinWidth;
				float decalMaxWidth;
				float decalMinHeight;
				float decalMaxHeight;
				float depth;
				float shininess;
				float parallaxScale;
				std::int8_t parallaxPasses;
				TEnumSet<Flag, std::uint8_t> flags;
				std::uint16_t pad1E;
				NiAPI::NiRGB color;
			};
			static_assert(sizeof(DecalData) == 0x24);
		}
	}
}