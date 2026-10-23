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
			class BSEffectShaderProperty : public BSShaderProperty
			{
			public:
				void* Unk70;										// 70
				NiAPI::NiColor* ExternalEmittance;					// 78
				std::uint32_t Unk80;									// 80
				std::uint32_t Pad84;									// 84
			};
			static_assert(sizeof(BSEffectShaderProperty) == 0x88);
			static_assert(offsetof(BSEffectShaderProperty, ExternalEmittance) == 0x78);
		}
	}
}
