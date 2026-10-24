// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSLight;

			class BSShaderPropertyLightData
			{
			public:
				/* 0x00 */ std::uint32_t LightListFence;
				/* 0x04 */ std::uint32_t ShadowAccumFlags;
				/* 0x08 */ std::uint32_t LightListChanged;
				/* 0x0C */ std::uint32_t pad0C;
				/* 0x10 */ BSTArray<BSLight*> LightList;
			};
			static_assert(sizeof(BSShaderPropertyLightData) == 0x28);
			static_assert(offsetof(BSShaderPropertyLightData, ShadowAccumFlags) == 0x4);
			static_assert(offsetof(BSShaderPropertyLightData, LightListChanged) == 0x8);
			static_assert(offsetof(BSShaderPropertyLightData, LightList) == 0x10);
		}
	}
}
