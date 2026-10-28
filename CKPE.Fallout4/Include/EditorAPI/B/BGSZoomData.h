// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESImageSpaceModifier;

				// 1.11.240.0: size 0x48, vtable RVA 0x2FC0920, 124 virtual functions
				class BGSZoomData :
					public TESForm   // 000
				{
				public:
					virtual ~BGSZoomData() = default;

					constexpr static std::uint8_t TYPE_ID = ftZoomData;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSZoomData[0];
				public:
					std::uint32_t unk28; // 28
					std::uint32_t unk2C; // 2C
					std::uint32_t unk30; // 30
					std::uint32_t unk34; // 34
					std::uint32_t unk38; // 38
					std::uint32_t unk3C; // 3C
					TESImageSpaceModifier* isMod; // 40
				};
				static_assert(sizeof(BGSZoomData) == 0x48);
			}
		}
	}
}
