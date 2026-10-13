// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/N/NiColor.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSTextureSet;

				struct SplineData
				{
					float numTiles; // 00
					std::uint16_t numSlices; // 04
					std::uint16_t flags; // 06
					NiAPI::NiColor color; // 08
					float windSensibility; // 14
					float flexibility; // 18
				};
				static_assert(sizeof(SplineData) == 0x1C);

				// 1.11.240.0: size 0xB8, vtable RVA 0x2FBD638, 154 virtual functions
				class BGSBendableSpline :
					public TESBoundObject   // 000
				{
				public:
					virtual ~BGSBendableSpline() = default;

				public:
					SplineData data; // 90
					std::uint32_t unkAC; // AC
					BGSTextureSet* textureSet; // B0
				};
				static_assert(sizeof(BGSBendableSpline) == 0xB8);
			}
		}
	}
}
