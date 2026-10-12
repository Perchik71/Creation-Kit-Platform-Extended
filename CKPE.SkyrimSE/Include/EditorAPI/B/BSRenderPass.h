// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			class BSShaderProperty;
			class BSGeometry;

			// Identical in every supported Creation Kit build (1.5.73.0 - 1.7.99.0): BSShader::SetupGeometry
			// receives it as the second argument and reads the shader property at 0x8 and the geometry at 0x10.
			class BSRenderPass
			{
			public:
				static constexpr std::uint32_t MaxLightInArrayC = 16;

				void* pShader;								// 00
				BSShaderProperty* pShaderProperty;			// 08
				BSGeometry* pGeometry;						// 10
				std::uint32_t uiPassEnum;					// 18
				std::uint8_t ucAccumulationHint;			// 1C
				std::uint8_t ucExtraParam;					// 1D
				std::uint8_t ucLODMode;						// 1E (bits 0-6 index, bit 7 single level)
				std::uint8_t ucNumLights;					// 1F
				std::uint16_t usWord20;						// 20
				char _pad22[6];								// 22
				BSRenderPass* pNext;						// 28
				BSRenderPass* pPassGroupNext;				// 30
				void** ppSceneLights;						// 38
				std::uint32_t uiCachePoolId;				// 40
				char _pad44[4];								// 44
			};
			static_assert(sizeof(BSRenderPass) == 0x48);
			static_assert(offsetof(BSRenderPass, pShaderProperty) == 0x8);
			static_assert(offsetof(BSRenderPass, pGeometry) == 0x10);
		}
	}
}
