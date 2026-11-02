// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/T/TESObjectMISC.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x178, vtable RVA 0x30F5A80 (KEYM)
				class TESKey : public TESObjectMISC
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftKey;

					virtual ~TESKey() = default;
				};
				static_assert(sizeof(TESKey) == 0x178);
			}
		}
	}
}