// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESObjectACTI.h>
#include <EditorAPI/T/TESObjectREFR.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSVoiceType;

				// 1.11.240.0: size 0x1F8, vtable RVA 0x3086378, 155 virtual functions
				class BGSTalkingActivator :
					public TESObjectACTI   // 000
				{
				public:
					virtual ~BGSTalkingActivator() = default;

					constexpr static std::uint8_t TYPE_ID = ftTalkingActivator;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSTalkingActivator[0];

					virtual bool GetReflectedByAutoWater() noexcept(true) { return false; } // 154 - RVA 0xC2E650, return 0;
				public:
					TESObjectREFR* tempRef; // 1E8
					BGSVoiceType* voiceType; // 1F0
				};
				static_assert(sizeof(BGSTalkingActivator) == 0x1F8);
			}
		}
	}
}
