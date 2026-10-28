// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSSoundDescriptorForm;

				// 1.11.240.0: size 0xA8, vtable RVA 0x307CB28, 154 virtual functions
				class TESSound :
					public TESBoundAnimObject   // 000
				{
				public:
					virtual ~TESSound() = default;

					constexpr static std::uint8_t TYPE_ID = ftSoundMarker;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESSound[0];
				public:
					BGSSoundDescriptorForm* descriptor; // 90
					std::uint64_t unk98; // 98
					std::uint8_t unkA0; // A0
					std::uint8_t unkA1[0x7]; // A1
				};
				static_assert(sizeof(TESSound) == 0xA8);
			}
		}
	}
}
