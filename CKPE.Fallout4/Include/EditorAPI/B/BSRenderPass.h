// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSShader;
			class BSShaderProperty;
			class BSGeometry;
			class BSLight;

			class BSRenderPass
			{
			public:
				std::byte* CommandBuffer;							// 00
				BSShader* Shader;									// 08
				BSShaderProperty* ShaderProperty;					// 10
				BSGeometry* Geometry;								// 18
				void* Unk20;										// 20
				void* Unk28;										// 28
				BSLight** SceneLights;								// 30
				BSRenderPass* PropertyNext;							// 38
				BSRenderPass* BatchNext;								// 40
				void* Unk48;										// 48
				std::uint32_t PassEnum;								// 50
				std::uint8_t PassType;								// 54
				std::uint8_t Unk55;									// 55
				std::int8_t LODMode;									// 56
				std::uint8_t Unk57;									// 57
				std::uint8_t Unk58;									// 58
				std::uint8_t NumLights;								// 59
				std::uint8_t Pad5A[6];								// 5A
			};
			static_assert(sizeof(BSRenderPass) == 0x60);
			static_assert(offsetof(BSRenderPass, SceneLights) == 0x30);
			static_assert(offsetof(BSRenderPass, PropertyNext) == 0x38);
			static_assert(offsetof(BSRenderPass, PassEnum) == 0x50);
			static_assert(offsetof(BSRenderPass, NumLights) == 0x59);
		}
	}
}
