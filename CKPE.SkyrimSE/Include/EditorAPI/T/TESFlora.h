// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/T/TESObjectACTI.h>
#include <EditorAPI/B/TESProduceForm.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x158, vtable RVA 0x30F3A28
				class TESFlora :
					public TESObjectACTI,	// 00
					public TESProduceForm	// 138
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftFlora;

					virtual ~TESFlora() = default;


				};
				static_assert(sizeof(TESFlora) == 0x158);
			}
		}
	}
}