// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/T/TESTexture.h>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x78, vtable RVA 0x30A6A90, 124 virtual functions
				class BGSShaderParticleGeometryData :
					public TESForm   // 000
				{
				public:
					virtual ~BGSShaderParticleGeometryData() = default;

				public:
					BSTArray<void*> data; // 28 - BSTArray<SETTING_VALUE>
					TESTexture particleTexture; // 40
					BSFixedString materialName; // 68
					std::uint32_t unk70; // 70
					std::uint32_t unk74; // 74
				};
				static_assert(sizeof(BGSShaderParticleGeometryData) == 0x78);
			}
		}
	}
}
