// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSGeometry.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// Shader data loaded from a material file (BGSM). CommonLibF4: BSShaderData.
			// Only the members used by CKPE are declared; the layout is not decomposed.
			class BSShaderData
			{
			public:
				// CommonLibF4: BSShaderData::ApplyMaterialData (CK 1.11.240.0 0x142BCC550; address ID 1666424 in 1.10.982.3 - 1.11.240.0, 797024 in 1.10.162.0 = 0x142AB67E0).
				// Rebuilds the BSShaderProperty material of the geometry from the material file, which resets every material member
				// (including BSLightingShaderMaterialBase::lookupScale, the colour remapping index) to the values stored in the file.
				// Set by the patches that detour it to the original function.
				inline static void(*pointer_ApplyMaterialData)(BSShaderData*, BSGeometry*, bool) { nullptr };

				inline void ApplyMaterialData(BSGeometry* Geometry, bool Flag) noexcept(true)
				{
					pointer_ApplyMaterialData(this, Geometry, Flag);
				}
			};
		}
	}
}
