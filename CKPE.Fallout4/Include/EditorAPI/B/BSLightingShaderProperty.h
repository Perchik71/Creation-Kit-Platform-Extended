// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSShaderProperty.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSLightingShaderProperty : public BSShaderProperty
			{
			public:
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSLightingShaderProperty[0];

				NiAPI::NiColorA ProjectedUVParams;					// 70
				NiAPI::NiColorA ProjectedUVColor;						// 80
				RenderPassArray DepthMapRenderPassLists[3];			// 90
				BSRenderPass* DepthRenderPass;						// A8
				RenderPassArray ForwardPassList;						// B0
				NiAPI::NiColor* EmitColor;							// B8
				BSFixedString RootName;								// C0
				float EmitColorScale;								// C8
				float ForcedDarkness;								// CC
				float SpecularLODFade;								// D0
				float EnvmapLODFade;									// D4
				std::uint32_t BaseTechniqueID;						// D8
				std::uint32_t UnkDC;									// DC
				std::uint16_t DebugTintIndex;						// E0
				std::uint16_t UnkE2;									// E2
				std::uint32_t UnkE4;									// E4
			};
			static_assert(sizeof(BSLightingShaderProperty) == 0xE8);
			static_assert(offsetof(BSLightingShaderProperty, DepthMapRenderPassLists) == 0x90);
			static_assert(offsetof(BSLightingShaderProperty, EmitColor) == 0xB8);
			static_assert(offsetof(BSLightingShaderProperty, RootName) == 0xC0);
			static_assert(offsetof(BSLightingShaderProperty, BaseTechniqueID) == 0xD8);
		}
	}
}
