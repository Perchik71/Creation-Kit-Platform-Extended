// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BaseFormComponent.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESIdleForm;
			}

			// 1.11.240.0: size 0x20, vtable RVA 0x30196C0, 21 virtual functions
			class BGSIdleCollection :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~BGSIdleCollection() = default;

				constexpr static Common::ID VTABLE = IDs::VTABLE::BGSIdleCollection[0];
			public:
				std::int8_t idleFlags; // 08
				std::int8_t idleCount; // 09
				std::uint8_t pad0A[0x6]; // 0A
				Forms::TESIdleForm** idleArray; // 10
				float timerCheckForIdle; // 18
				std::uint32_t pad1C; // 1C
			};
			static_assert(sizeof(BGSIdleCollection) == 0x20);
		}
	}
}
