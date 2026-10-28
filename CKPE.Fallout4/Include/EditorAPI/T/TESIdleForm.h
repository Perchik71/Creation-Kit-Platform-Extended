// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/B/BSFixedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				struct IDLE_DATA
				{
					std::int8_t loopMin; // 00
					std::int8_t loopMax; // 01
					std::int8_t flags; // 02
					std::uint16_t replayDelay; // 04
				};
				static_assert(sizeof(IDLE_DATA) == 0x6);

				// 1.11.246.0: size 0x78, vtable RVA 0x3044AE8, 125 virtual functions
				class TESIdleForm :
					public TESForm   // 000
				{
				public:
					virtual ~TESIdleForm() = default;

					constexpr static std::uint8_t TYPE_ID = ftIdle;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESIdleForm[0];

					virtual void OpenIdleManager() noexcept(true) { return; } // 124
				public:
					std::uint8_t unk28[0x18]; // 28
					IDLE_DATA data; // 40
					std::uint16_t unk46; // 46
					std::uint64_t unk48; // 48
					TESIdleForm* parentIdle; // 50
					TESIdleForm* prevIdle; // 58
					BSFixedString behaviorGraphName; // 60
					BSFixedString animEventName; // 68
					BSFixedString animFileName; // 70
				};
				static_assert(sizeof(TESIdleForm) == 0x78);
			}
		}
	}
}
