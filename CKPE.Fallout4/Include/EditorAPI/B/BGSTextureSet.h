// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSShaderData.h>
#include <EditorAPI/B/BGSImpactData.h>
#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/B/BSTextureSet.h>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/T/TESTexture.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{

				// 1.11.240.0: size 0x3D8, vtable RVA 0x3064968, 154 virtual functions
				class BGSTextureSet :
					public TESBoundObject,  // 000
					public BSTextureSet     // 090
				{
				public:
					virtual ~BGSTextureSet() = default;

					constexpr static std::uint8_t TYPE_ID = ftTextureSet;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSTextureSet[0];
				public:
					BSFixedString materialName; // A0
					TESTexture textures[8]; // A8
					DecalData* decalData; // 1E8 - 0x24 byte heap object freed by ClearData
					std::uint16_t flags; // 1F0
					std::uint8_t pad1F2[0x6]; // 1F2
					// The CK keeps the material data inline (the game's textureFileIDs array is absent); same 0x1E0 byte object as the game's.
					BSShaderData materialData; // 1F8
				};
				static_assert(sizeof(BGSTextureSet) == 0x3D8);
			}
		}
	}
}
