// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/BGSModelMaterialSwap.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/T/TESModel.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/T/TESModelTri.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSTextureSet;

				class BGSColorForm;

				class BGSListForm;

				// 1.11.240.0: size 0x1C0, vtable RVA 0x3036560, 124 virtual functions
				class BGSHeadPart :
					public TESForm,               // 000
					public TESFullName,           // 028
					public BGSModelMaterialSwap   // 040
				{
				public:
					virtual ~BGSHeadPart() = default;

					constexpr static std::uint8_t TYPE_ID = ftHeadPart;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSHeadPart[0];
				public:
					std::uint8_t flags; // 90 - REX::TEnumSet<Flag, std::uint8_t>
					std::uint8_t unk91[0x3]; // 91
					std::int32_t type; // 94 - REX::TEnumSet<HeadPartType, std::int32_t>
					BSTArray<void*> extraParts; // 98 - BSTArray<BGSHeadPart*>
					BGSTextureSet* textureSet; // B0
					TESModel ChargenModel; // B8
					TESModelTri morphs[3]; // F0
					BGSColorForm* colorForm; // 198
					BGSListForm* validRaces; // 1A0
					std::uint8_t unk1A8[0x18]; // 1A8
				};
				static_assert(sizeof(BGSHeadPart) == 0x1C0);
			}
		}
	}
}
