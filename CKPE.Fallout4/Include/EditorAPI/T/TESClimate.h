// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESModel.h>
#include <EditorAPI/T/TESTexture.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0xF8, vtable RVA 0x30A8480, 124 virtual functions
				class TESClimate :
					public TESForm   // 000
				{
				public:
					virtual ~TESClimate() = default;

					constexpr static std::uint8_t TYPE_ID = ftClimate;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESClimate[0];
				public:
					TESModel nightSky; // 28
					std::uint8_t weatherList[0x18]; // 60 - TESWeatherList (WLST)
					std::uint8_t unk78[0x18]; // 78
					TESTexture skyObjects[2]; // 90 - FNAM/GNAM
					std::uint8_t data[0x6]; // E0 - TNAM
					std::uint16_t padE6; // E6
					std::uint64_t unkE8; // E8
					std::uint32_t unkF0; // F0
					std::uint32_t unkF4; // F4
				};
				static_assert(sizeof(TESClimate) == 0xF8);
			}
		}
	}
}
