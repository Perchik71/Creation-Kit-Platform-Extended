// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/N/NiNode.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESForm;
				class TESNPC;
			}

			// CommonLibF4: BSFaceGenUtils (shared FaceGen code, bsfacegenutils.cpp).
			namespace BSFaceGenUtils
			{
				// Applies the NPC hair colour (colour form remapping index -> BSLightingShaderMaterialBase::lookupScale), the face tint
				// textures and the skin tint to the geometry of one head part.
				// CommonLibF4: BSFaceGenManager SetupHeadPartTexture / PrepareHeadPartForShaders (game 1.11.240.0 0x1406EEA90).
				// CK 1.11.240.0 0x140ED9A90 (address ID 1426540 in 1.10.982.3 - 1.11.240.0, 685825 in 1.10.162.0 = 0x140FD1BC0). Parameters: the head scene root, the BGSHeadPart
				// (its TESForm::GetEntryEditID() is the geometry name) and the TESNPC. Set by the patches that detour it.
				inline static void(*pointer_SetupHeadPartTexture)(NiAPI::NiNode*, Forms::TESForm*, Forms::TESNPC*) { nullptr };

				inline void SetupHeadPartTexture(NiAPI::NiNode* Scene, Forms::TESForm* HeadPart, Forms::TESNPC* NPC) noexcept(true)
				{
					pointer_SetupHeadPartTexture(Scene, HeadPart, NPC);
				}
			}
		}
	}
}
