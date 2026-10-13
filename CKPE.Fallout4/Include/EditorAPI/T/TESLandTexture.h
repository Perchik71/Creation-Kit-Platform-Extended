// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/B/BSTList.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSTextureSet;

				class BGSMaterialType;

				struct TEXTURE_HAVOK_DATA
				{
					std::int8_t friction; // 00
					std::int8_t restitution; // 01
				};
				static_assert(sizeof(TEXTURE_HAVOK_DATA) == 0x2);

				// 1.11.240.0: size 0x70, vtable RVA 0x30ABCD8, 124 virtual functions
				class TESLandTexture :
					public TESForm   // 000
				{
				public:
					virtual ~TESLandTexture() = default;

					constexpr static std::uint8_t TYPE_ID = ftLandTexture;

				public:
					BGSTextureSet* textureSet; // 28
					TEXTURE_HAVOK_DATA havokData; // 30
					std::uint8_t unk32[0x6]; // 32
					BGSMaterialType* materialType; // 38
					std::int8_t specularExponent; // 40
					std::uint8_t unk41[0x3]; // 41
					std::int32_t shaderTextureIndex; // 44
					BSSimpleList<void*> textureGrassList; // 48 - BSSimpleList<TESGrass*>
					std::uint64_t unk58; // 58
					std::uint64_t unk60; // 60
					std::uint32_t unk68; // 68
					std::uint32_t unk6C; // 6C
				};
				static_assert(sizeof(TESLandTexture) == 0x70);
			}
		}
	}
}
