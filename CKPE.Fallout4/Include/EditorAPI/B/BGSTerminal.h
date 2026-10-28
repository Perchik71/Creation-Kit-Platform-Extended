// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESFurniture.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BGSLocalizedString.h>
#include <EditorAPI/T/TESTexture.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x330, vtable RVA 0x2FBF938, 154 virtual functions
				class BGSTerminal :
					public TESFurniture   // 000
				{
				public:
					virtual ~BGSTerminal() = default;

					constexpr static std::uint8_t TYPE_ID = ftTerminal;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSTerminal[0];
				public:
					BSTArray<void*> bodyTextItems; // 2A0 - BSTArray<BodyTextItem>
					BSTArray<void*> menuItems; // 2B8 - BSTArray<MenuItem>
					BGSLocalizedString headerTextOverride; // 2D0
					BGSLocalizedString welcomeText; // 2E0
					std::uint64_t unk2F0; // 2F0
					std::uint64_t unk2F8; // 2F8
					std::uint16_t unk300; // 300
					std::uint16_t unk302; // 302
					std::uint32_t unk304; // 304
					TESTexture unk308; // 308
				};
				static_assert(sizeof(BGSTerminal) == 0x330);
			}
		}
	}
}
